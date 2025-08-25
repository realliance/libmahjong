use super::gamestate::RawGameState;
use std::ffi::c_int;

/// C API constants for observed game state
pub const MAX_LIVE_HAND_SIZE: usize = 14;
pub const MAX_MELDS_PER_HAND: usize = 4;
pub const MAX_DISCARDS_PER_PLAYER: usize = 21;

/// Type aliases for C types
pub type CPiece = c_int;

/// C Meld Type enum
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CMeldType {
    Chi = 0,
    Pon = 1,
    Kan = 2,
    ConcealedKan = 3,
}

/// C State Function Type enum
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CStateFunctionType {
    Error = 0,
    GameStart = 1,
    RoundStart = 2,
    Draw = 3,
    PlayerHand = 4,
    Pon = 5,
    Chi = 6,
    Kan = 7,
    ConcealedKan = 8,
    ConvertedKan = 9,
    KanDiscard = 10,
    Replacement = 11,
    Riichi = 12,
    Discard = 13,
    Exhaust = 14,
    Ron = 15,
    Tsumo = 16,
    RoundEnd = 17,
    GameEnd = 18,
}

/// C Meld structure
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct CMeld {
    pub meld_type: CMeldType,
    pub start: CPiece,
}

/// C Hand structure
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct CHand {
    pub live_pieces: [CPiece; MAX_LIVE_HAND_SIZE],
    pub live_piece_count: c_int,
    pub melds: [CMeld; MAX_MELDS_PER_HAND],
    pub meld_count: c_int,
    pub discards: [CPiece; MAX_DISCARDS_PER_PLAYER],
    pub discard_count: c_int,
    pub open: bool,
    pub riichi: bool,
    pub riichi_piece_discard: c_int,
    pub riichi_round: c_int,
}

/// C Observed Game State structure
#[repr(C)]
#[derive(Debug, Clone)]
pub struct CObservedGameState {
    pub current_player: c_int,
    pub turn_num: c_int,
    pub round_num: c_int,
    pub riichi_sticks: c_int,
    pub counters: c_int,
    pub last_call: c_int,
    pub last_caller: c_int,
    pub concealed_kan: bool,
    pub seed: u64,
    pub pending_piece: CPiece,
    pub scores: [c_int; 4],
    pub points: [c_int; 4],
    pub has_ronned: [bool; 4],
    pub hands: [CHand; 4],
    pub prev_state: CStateFunctionType,
    pub curr_state: CStateFunctionType,
    pub next_state: CStateFunctionType,
}

impl Default for CMeld {
    fn default() -> Self {
        Self {
            meld_type: CMeldType::Chi,
            start: 0,
        }
    }
}

impl Default for CHand {
    fn default() -> Self {
        Self {
            live_pieces: [0; MAX_LIVE_HAND_SIZE],
            live_piece_count: 0,
            melds: [CMeld::default(); MAX_MELDS_PER_HAND],
            meld_count: 0,
            discards: [0; MAX_DISCARDS_PER_PLAYER],
            discard_count: 0,
            open: false,
            riichi: false,
            riichi_piece_discard: 0,
            riichi_round: 0,
        }
    }
}

impl Default for CObservedGameState {
    fn default() -> Self {
        Self {
            current_player: 0,
            turn_num: 0,
            round_num: 0,
            riichi_sticks: 0,
            counters: 0,
            last_call: 0,
            last_caller: 0,
            concealed_kan: false,
            seed: 0,
            pending_piece: 0,
            scores: [0; 4],
            points: [0; 4],
            has_ronned: [false; 4],
            hands: [CHand::default(); 4],
            prev_state: CStateFunctionType::Error,
            curr_state: CStateFunctionType::Error,
            next_state: CStateFunctionType::Error,
        }
    }
}

// FFI function declaration
#[link(name = "mahjong")]
unsafe extern "C" {
    /// Observe the current game state
    pub fn ObserveGameState(state: *mut RawGameState) -> CObservedGameState;
}
