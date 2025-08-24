use std::ffi::c_int;
use std::sync::{Arc, Mutex, MutexGuard};

use super::{error::MahjongFFIError, gamesettings::CGameSettings, observe::ObserveGameState};
use crate::observe::ObservedGameState;

/// Opaque type representing a GameState
#[repr(C)]
pub struct RawGameState {
    _data: [u8; 0],
    _marker: std::marker::PhantomData<(*mut u8, std::marker::PhantomPinned)>,
}

#[link(name = "mahjong")]
unsafe extern "C" {
    /// Start a game with the given settings
    pub fn StartGame(settings: *const CGameSettings, async_mode: bool) -> c_int;

    /// Exit a game by its ID
    pub fn ExitGame(game: c_int);

    /// Initialize a new game state
    pub fn InitGameState(settings: *const CGameSettings) -> *mut RawGameState;

    /// Advance the game state to the next state
    pub fn AdvanceGameState(state: *mut RawGameState) -> *mut RawGameState;

    /// Free a game state
    pub fn FreeGameState(state: *mut RawGameState);
}

/// Safe wrapper for GameState
pub struct GameState {
    ptr: Arc<Mutex<Option<*mut RawGameState>>>,
}

// Safe due to the use of mutexes
unsafe impl Send for GameState {}
unsafe impl Sync for GameState {}

impl GameState {
    /// Create a new game state from settings
    pub fn new<S: TryInto<CGameSettings, Error = MahjongFFIError>>(
        settings: S,
    ) -> Result<Self, MahjongFFIError> {
        let ptr = unsafe {
            let settings: CGameSettings = settings.try_into()?;
            Ok(InitGameState(&settings))
        }?;
        if ptr.is_null() {
            Err(MahjongFFIError::FailedToAllocateGameState)
        } else {
            Ok(Self {
                ptr: Arc::new(Mutex::new(Some(ptr))),
            })
        }
    }

    /// Advance the game state
    pub fn advance(self) -> Result<Self, MahjongFFIError> {
        let mut guard = self
            .ptr
            .lock()
            .map_err(|_| MahjongFFIError::MutexPoisoned)?;

        let take_ptr = guard.take();

        if let Some(ptr) = take_ptr {
            let new_ptr = unsafe { AdvanceGameState(ptr) };
            let _ = ptr; // Prevent double-free since C++ takes ownership

            if new_ptr.is_null() {
                Err(MahjongFFIError::GameEnded)
            } else {
                Ok(Self {
                    ptr: Arc::new(Mutex::new(Some(new_ptr))),
                })
            }
        } else {
            Err(MahjongFFIError::GameStateConsumed)
        }
    }

    /// Observe the current game state
    pub fn observe(&self) -> Option<ObservedGameState> {
        let guard = self.ptr.lock().ok()?;

        if let Some(ptr) = *guard {
            let c_observed = unsafe { ObserveGameState(ptr) };
            Some(c_observed.into())
        } else {
            None
        }
    }

    /// Get the raw pointer (sync version for internal use)
    pub fn as_ptr(
        &self,
    ) -> Result<
        MutexGuard<'_, Option<*mut RawGameState>>,
        std::sync::PoisonError<MutexGuard<'_, Option<*mut RawGameState>>>,
    > {
        self.ptr.lock()
    }
}

impl Drop for GameState {
    fn drop(&mut self) {
        // Now we can safely lock synchronously in Drop
        if let Ok(mut guard) = self.ptr.lock() {
            if let Some(ptr) = guard.take() {
                unsafe { FreeGameState(ptr) };
            }
        }
        // If the mutex is poisoned, we can't clean up safely
    }
}

/// Safe wrapper for game operations
pub fn start_game(settings: &CGameSettings, async_mode: bool) -> c_int {
    unsafe { StartGame(settings as *const CGameSettings, async_mode) }
}

pub fn exit_game(game_id: c_int) {
    unsafe { ExitGame(game_id) }
}
