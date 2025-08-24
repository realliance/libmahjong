#[derive(Debug, Clone)]
pub struct GameSettings {
    pub seed: u64,
    pub seat_controllers: [String; 4],
}
