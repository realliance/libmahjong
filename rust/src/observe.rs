use crate::ffi::observe::{CHand, CMeld, CMeldType, CObservedGameState, CStateFunctionType};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MeldType {
    Chi,
    Pon,
    Kan,
    ConcealedKan,
}

impl From<CMeldType> for MeldType {
    fn from(c_meld_type: CMeldType) -> Self {
        match c_meld_type {
            CMeldType::Chi => MeldType::Chi,
            CMeldType::Pon => MeldType::Pon,
            CMeldType::Kan => MeldType::Kan,
            CMeldType::ConcealedKan => MeldType::ConcealedKan,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum StateFunctionType {
    Error,
    GameStart,
    RoundStart,
    Draw,
    PlayerHand,
    Pon,
    Chi,
    Kan,
    ConcealedKan,
    ConvertedKan,
    KanDiscard,
    Replacement,
    Riichi,
    Discard,
    Exhaust,
    Ron,
    Tsumo,
    RoundEnd,
    GameEnd,
}

impl From<CStateFunctionType> for StateFunctionType {
    fn from(c_state_type: CStateFunctionType) -> Self {
        match c_state_type {
            CStateFunctionType::Error => StateFunctionType::Error,
            CStateFunctionType::GameStart => StateFunctionType::GameStart,
            CStateFunctionType::RoundStart => StateFunctionType::RoundStart,
            CStateFunctionType::Draw => StateFunctionType::Draw,
            CStateFunctionType::PlayerHand => StateFunctionType::PlayerHand,
            CStateFunctionType::Pon => StateFunctionType::Pon,
            CStateFunctionType::Chi => StateFunctionType::Chi,
            CStateFunctionType::Kan => StateFunctionType::Kan,
            CStateFunctionType::ConcealedKan => StateFunctionType::ConcealedKan,
            CStateFunctionType::ConvertedKan => StateFunctionType::ConvertedKan,
            CStateFunctionType::KanDiscard => StateFunctionType::KanDiscard,
            CStateFunctionType::Replacement => StateFunctionType::Replacement,
            CStateFunctionType::Riichi => StateFunctionType::Riichi,
            CStateFunctionType::Discard => StateFunctionType::Discard,
            CStateFunctionType::Exhaust => StateFunctionType::Exhaust,
            CStateFunctionType::Ron => StateFunctionType::Ron,
            CStateFunctionType::Tsumo => StateFunctionType::Tsumo,
            CStateFunctionType::RoundEnd => StateFunctionType::RoundEnd,
            CStateFunctionType::GameEnd => StateFunctionType::GameEnd,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Meld {
    pub meld_type: MeldType,
    pub start: i32,
}

impl From<CMeld> for Meld {
    fn from(c_meld: CMeld) -> Self {
        Self {
            meld_type: c_meld.meld_type.into(),
            start: c_meld.start,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Hand {
    pub live_pieces: Vec<i32>,
    pub melds: Vec<Meld>,
    pub discards: Vec<i32>,
    pub open: bool,
    pub riichi: bool,
    pub riichi_piece_discard: i32,
    pub riichi_round: i32,
}

impl From<CHand> for Hand {
    fn from(c_hand: CHand) -> Self {
        let live_pieces = c_hand.live_pieces[..c_hand.live_piece_count as usize].to_vec();

        let melds = c_hand.melds[..c_hand.meld_count as usize]
            .iter()
            .map(|&meld| meld.into())
            .collect();

        let discards = c_hand.discards[..c_hand.discard_count as usize].to_vec();

        Self {
            live_pieces,
            melds,
            discards,
            open: c_hand.open,
            riichi: c_hand.riichi,
            riichi_piece_discard: c_hand.riichi_piece_discard,
            riichi_round: c_hand.riichi_round,
        }
    }
}

impl Hand {
    /// Get the number of live pieces in the hand
    pub fn live_piece_count(&self) -> usize {
        self.live_pieces.len()
    }

    /// Get the number of melds in the hand
    pub fn meld_count(&self) -> usize {
        self.melds.len()
    }

    /// Get the number of discards in the hand
    pub fn discard_count(&self) -> usize {
        self.discards.len()
    }

    /// Add a piece to the live pieces (example method showing Vec usage)
    pub fn add_live_piece(&mut self, piece: i32) {
        self.live_pieces.push(piece);
    }

    /// Add a meld to the hand (example method showing Vec usage)
    pub fn add_meld(&mut self, meld: Meld) {
        self.melds.push(meld);
    }

    /// Add a discard to the hand (example method showing Vec usage)
    pub fn add_discard(&mut self, piece: i32) {
        self.discards.push(piece);
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ObservedGameState {
    pub current_player: i32,
    pub turn_num: i32,
    pub round_num: i32,
    pub riichi_sticks: i32,
    pub counters: i32,
    pub last_call: i32,
    pub last_caller: i32,
    pub concealed_kan: bool,
    pub seed: u64,
    pub pending_piece: i32,
    pub scores: [i32; 4],
    pub points: [i32; 4],
    pub has_ronned: [bool; 4],
    pub hands: [Hand; 4],
    pub prev_state: StateFunctionType,
    pub curr_state: StateFunctionType,
    pub next_state: StateFunctionType,
}

impl From<CObservedGameState> for ObservedGameState {
    fn from(c_state: CObservedGameState) -> Self {
        let hands = [
            c_state.hands[0].into(),
            c_state.hands[1].into(),
            c_state.hands[2].into(),
            c_state.hands[3].into(),
        ];

        Self {
            current_player: c_state.current_player,
            turn_num: c_state.turn_num,
            round_num: c_state.round_num,
            riichi_sticks: c_state.riichi_sticks,
            counters: c_state.counters,
            last_call: c_state.last_call,
            last_caller: c_state.last_caller,
            concealed_kan: c_state.concealed_kan,
            seed: c_state.seed,
            pending_piece: c_state.pending_piece,
            scores: c_state.scores,
            points: c_state.points,
            has_ronned: c_state.has_ronned,
            hands,
            prev_state: c_state.prev_state.into(),
            curr_state: c_state.curr_state.into(),
            next_state: c_state.next_state.into(),
        }
    }
}

impl ObservedGameState {
    /// Get the current player index
    pub fn current_player(&self) -> i32 {
        self.current_player
    }

    /// Get the current turn number
    pub fn turn_num(&self) -> i32 {
        self.turn_num
    }

    /// Get the current round number
    pub fn round_num(&self) -> i32 {
        self.round_num
    }

    /// Get the number of riichi sticks on the table
    pub fn riichi_sticks(&self) -> i32 {
        self.riichi_sticks
    }

    /// Get the honba counters
    pub fn counters(&self) -> i32 {
        self.counters
    }

    /// Get the seed used for the game
    pub fn seed(&self) -> u64 {
        self.seed
    }

    /// Get the current pending piece
    pub fn pending_piece(&self) -> i32 {
        self.pending_piece
    }

    /// Get the scores for all players
    pub fn scores(&self) -> &[i32; 4] {
        &self.scores
    }

    /// Get the points for all players
    pub fn points(&self) -> &[i32; 4] {
        &self.points
    }

    /// Get the hands for all players
    pub fn hands(&self) -> &[Hand; 4] {
        &self.hands
    }

    /// Get the current state function type
    pub fn current_state(&self) -> StateFunctionType {
        self.curr_state
    }

    /// Get the previous state function type
    pub fn previous_state(&self) -> StateFunctionType {
        self.prev_state
    }

    /// Get the next state function type
    pub fn next_state(&self) -> StateFunctionType {
        self.next_state
    }

    /// Check if concealed kan was just performed
    pub fn concealed_kan(&self) -> bool {
        self.concealed_kan
    }

    /// Get which players have declared ron this turn
    pub fn has_ronned(&self) -> &[bool; 4] {
        &self.has_ronned
    }
}
