// FFI (Foreign Function Interface) for Mahjong game controller

pub mod ffi;
pub mod observe;
pub mod settings;

#[cfg(test)]
mod tests {
    use crate::{ffi::gamestate::GameState, observe::StateFunctionType, settings::GameSettings};
    use std::{collections::VecDeque, fs::File, io::Write};

    #[test]
    fn can_run_match() {
        let settings = GameSettings {
            seed: 1,
            seat_controllers: [
                "AngryDiscardoBot".to_string(),
                "AngryDiscardoBot".to_string(),
                "AngryDiscardoBot".to_string(),
                "AngryDiscardoBot".to_string(),
            ],
        };

        let mut log = File::create("test_match.log").unwrap();

        log.write_fmt(format_args!(
            "Starting game with settings: {:?}\n",
            settings
        ))
        .unwrap();
        let mut game_state = GameState::new(settings).unwrap();

        let mut counter = 0;
        let mut history: VecDeque<_> = VecDeque::with_capacity(20);

        let mut last_observed = None;
        while let Some(observed) = game_state.observe() {
            log.write_fmt(format_args!(
                "Turn {}: {:?} -> {:?} -> {:?}\n",
                counter, observed.prev_state, observed.curr_state, observed.next_state
            ))
            .unwrap();

            let should_end = observed.curr_state == StateFunctionType::GameEnd;

            if history.len() == 20 {
                history.pop_front();
            }

            history.push_back((
                observed.prev_state,
                observed.curr_state,
                observed.next_state,
            ));

            last_observed = Some(observed);
            if should_end {
                log.write_fmt(format_args!("Game ended at turn {}\n", counter))
                    .unwrap();
                break;
            }

            game_state = game_state.advance().unwrap();

            counter += 1;
            if counter > 1000 {
                panic!(
                    "Game state did not end after 1000 turns. Observed is {:?}\nHistory: {:#?}",
                    last_observed, history
                );
            }
        }

        assert_eq!(
            last_observed.unwrap().current_state(),
            StateFunctionType::GameEnd
        );
    }
}
