from libmahjong import start_game, EventType, EngineEvent, PieceType
from riichiroyale.game import Player, Match, process_event_queue, DialogManager
from .boardview import BoardView


class GameView(BoardView):
    def __init__(
        self,
        game_manager,
        screen,
        tile_dict,
        small_tile_dict,
        screen_width,
        screen_height,
        width_ratio,
        height_ratio,
        player_manager=None,
    ):
        super().__init__(
            "game",
            game_manager,
            DialogManager(),
            screen,
            tile_dict,
            small_tile_dict,
            screen_width,
            screen_height,
            width_ratio,
            height_ratio,
        )
        self.player_manager = player_manager
        self.ai_list = ["Player"] + ["AngryDiscardoBot"] * 3

    def on_match_init(self):
        self.match = None
        self.match = Match(self.ai_list, self.game_manager, self.player_manager, self.sound_manager)
        self.match.start()

    def on_pov_init(self):
        if self.player_manager is None:
            self.match_pov = 0

    def on_tile_pressed(self, owner, tile_hand_index):
        if owner.my_turn:
            tile = owner.hand[tile_hand_index]
            if (
                self.game_manager.board_manager.waiting_on_decision
                and len(owner.calls_avaliable) == 0
            ):
                self.match.play_clack()
                event = EngineEvent()
                event.type = EventType.Discard
                event.piece = int(tile.get_raw_value())
                event.player = owner.player_id
                event.decision = True
                self.player_manager.MakeDecision(event)

    def on_pon_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event.raw_event_b
        )
        return True

    def on_chi_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event.raw_event_b
        )
        return True

    def on_kan_button_pressed(self):
        if (
            self.game_manager.board_manager.last_decision_event.type
            == EventType.ConvertedKan
        ):
            self.match.player_manager.MakeDecision(
                self.game_manager.board_manager.last_decision_event
            )
        else:
            self.match.player_manager.MakeDecision(
                self.game_manager.board_manager.last_decision_event.raw_event_b
            )
        return True

    def on_tsumo_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event.raw_event_b
        )
        return True

    def on_riichi_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event
        )
        return True

    def on_ron_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event.raw_event_b
        )
        return True

    def on_skip_button_pressed(self):
        event = EngineEvent()
        event.type = EventType.Decline
        event.piece = -1
        event.player = -1
        event.decision = True

        self.match.player_manager.MakeDecision(event)            

        return True

    def on_dialogue_event_ending(self, event_name):
        if event_name == "round_end":
            self.game_manager.set_active_view("main_menu")
            self.game_manager.sound_manager.play_music("lobby")

    def _end_round_dialog(self):
        self.ai_game_active = False
        self.game_manager.board_manager.round_should_end = False
        self.dialogue_manager.register_dialog_event("round_end")
        self.dialogue_manager.append_dialog_event(
            "round_end", ["Round Complete! Now for the Results..."]
        )
        i = 0
        for score in self.match.scores:
            self.dialogue_manager.append_dialog_event(
                "round_end", ["Player {0} was awarded {1} points!".format(i + 1, score)]
            )
            i += 1
        self.dialogue_manager.append_dialog_event(
            "round_end",
            [
                "Thank you for playing a demo match of Riichi Royale! Press Next to return to the main menu."
            ],
        )
        self.dialogue_manager.start_event("round_end")
        self.player.calls_avaliable = []
        for button in self.buttons:
            self.buttons[button].hide()

    def update(self, time_delta):
        if self.game_manager.board_manager.round_should_end:
            self._end_round_dialog()

        super().update(time_delta)
