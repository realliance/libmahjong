use std::ffi::{c_char, c_int, CString};

use crate::settings::GameSettings;

use super::error::MahjongFFIError;

/// C-compatible GameSettings structure
#[repr(C)]
pub struct CGameSettings {
    pub seed: u64,
    pub seat_controllers: [*const c_char; 4],
    pub num_controllers: c_int,
}

fn try_string_to_cstring(s: &str) -> Result<*const c_char, MahjongFFIError> {
    CString::new(s)
        .map(|cstr| cstr.into_raw() as *const c_char)
        .map_err(|_| MahjongFFIError::FailedToCreateCString)
}

impl TryFrom<GameSettings> for CGameSettings {
    type Error = MahjongFFIError;

    fn try_from(value: GameSettings) -> Result<Self, Self::Error> {
        Ok(Self {
            seed: value.seed,
            seat_controllers: [
                try_string_to_cstring(&value.seat_controllers[0])?,
                try_string_to_cstring(&value.seat_controllers[1])?,
                try_string_to_cstring(&value.seat_controllers[2])?,
                try_string_to_cstring(&value.seat_controllers[3])?,
            ],
            num_controllers: 4,
        })
    }
}
