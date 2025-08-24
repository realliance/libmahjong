pub mod error;
pub mod gamesettings;
pub mod gamestate;
pub mod observe;

#[cfg(test)]
mod tests {
    use crate::ffi::gamestate::GameState;
    use crate::observe::StateFunctionType;
    use crate::settings::GameSettings;
    use futures::future::join_all;
    use tokio::time::{sleep, Duration};

    #[test]
    fn can_create_game() -> anyhow::Result<()> {
        let settings = GameSettings {
            seed: 12345,
            seat_controllers: [
                "AlphabeticalBot".to_string(),
                "AlphabeticalBot".to_string(),
                "AlphabeticalBot".to_string(),
                "AlphabeticalBot".to_string(),
            ],
        };

        let game_state = GameState::new(settings)?;

        assert!(
            game_state
                .as_ptr()
                .map_err(|e| anyhow::anyhow!("Failed to lock game state: {}", e))?
                .is_some(),
            "Game state pointer should not be null"
        );

        Ok(())
    }

    #[test]
    fn can_run_game() -> anyhow::Result<()> {
        let settings = GameSettings {
            seed: 12345,
            seat_controllers: [
                "AlphabeticalBot".to_string(),
                "AlphabeticalBot".to_string(),
                "AlphabeticalBot".to_string(),
                "AlphabeticalBot".to_string(),
            ],
        };

        let mut game_state = GameState::new(settings)?;

        let mut counter = 0;
        loop {
            if counter < 5 {
                counter += 1;
                game_state = game_state.advance()?;
            } else {
                break;
            }
        }

        assert!(
            game_state
                .as_ptr()
                .map_err(|e| anyhow::anyhow!("Failed to lock game state: {}", e))?
                .is_some(),
            "Game state pointer shouldnt be null"
        );

        Ok(())
    }

    #[test]
    fn can_observe_game_state() -> anyhow::Result<()> {
        let settings = GameSettings {
            seed: 12345,
            seat_controllers: [
                "AlphabeticalBot".to_string(),
                "AlphabeticalBot".to_string(),
                "AlphabeticalBot".to_string(),
                "AlphabeticalBot".to_string(),
            ],
        };

        let game_state = GameState::new(settings)?;

        // Test that we can observe the game state
        let observed = game_state
            .observe()
            .ok_or(anyhow::anyhow!("Failed to observe game state"))?;

        // Basic sanity checks - newly created game state will have initial values
        assert_eq!(observed.seed(), 12345);
        assert_eq!(observed.current_player(), -1);
        assert_eq!(observed.current_state(), StateFunctionType::Error);

        // Check that we have correct array sizes
        assert_eq!(observed.hands().len(), 4);
        assert_eq!(observed.scores().len(), 4);
        assert_eq!(observed.points().len(), 4);
        assert_eq!(observed.has_ronned().len(), 4);

        // Advance the game state
        let current_gs = game_state.advance()?;
        let observed = current_gs.observe().ok_or(anyhow::anyhow!(
            "Failed to observe game state after advance"
        ))?;

        // Observe that the next game state is what's expected
        assert_eq!(observed.current_state(), StateFunctionType::GameStart);

        Ok(())
    }

    #[tokio::test(flavor = "multi_thread")]
    async fn can_run_game_async() -> anyhow::Result<()> {
        let mut game_states = vec![];
        for i in 0..25 {
            let settings = GameSettings {
                seed: 12345 + i,
                seat_controllers: [
                    "AlphabeticalBot".to_string(),
                    "AlphabeticalBot".to_string(),
                    "AlphabeticalBot".to_string(),
                    "AlphabeticalBot".to_string(),
                ],
            };
            game_states.push(GameState::new(settings)?);
        }

        let futures = game_states.into_iter().map(|gs| {
            tokio::spawn(async move {
                let mut current_gs = gs;
                for _ in 0..3 {
                    sleep(Duration::from_millis(10)).await;
                    current_gs = current_gs.advance()?;
                }
                Ok::<_, anyhow::Error>(current_gs)
            })
        });

        let results = join_all(futures).await;

        for result in results {
            let game_state = result??;
            let observed = game_state
                .observe()
                .ok_or(anyhow::anyhow!("Failed to observe game state"))?;
            assert_ne!(observed.current_state(), StateFunctionType::Error);
            assert_ne!(observed.current_state(), StateFunctionType::GameStart);
        }

        Ok(())
    }

    #[test]
    fn invalid_controllers_noabort() {
        let settings = GameSettings {
            seed: 12345,
            seat_controllers: [
                "FakeBot".to_string(),
                "FakeBot".to_string(),
                "FakeBot".to_string(),
                "FakeBot".to_string(),
            ],
        };

        let state = GameState::new(settings);
        assert!(
            state.is_err(),
            "Game should not start with invalid controllers"
        );
    }
}
