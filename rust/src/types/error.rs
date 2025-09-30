use thiserror::Error;

#[derive(Debug, Error)]
pub enum MahjongError {
    #[error("Invalid piece value: {value:#04x}")]
    InvalidPiece { value: u8 },
}
