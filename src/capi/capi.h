#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/****
* C Game Types
*/

typedef uint8_t CPiece;
typedef uint8_t CWind;

typedef struct CEvent {
    uint8_t type;
    int player;
    int16_t piece;
    bool decision;
} CEvent;

typedef struct CGameSettings {
    unsigned int seed;
    char** seatControllers; // Array of controller names
    size_t numControllers;  // Number of controllers
    char* overrideWall;     // Optional override wall
} GameSettings;

// Opaque Types
typedef struct GameStateWrapper GameState;
typedef struct PlayerControllerWrapper PlayerController;

/***
* Game Control
*/

int StartGame(const GameSettings* settings, int async);
void ExitGame(int game);
GameState* InitGameState(const GameSettings* settings);
GameState* AdvanceGameState(GameState* state); 
void DestroyGameState(GameState* state);

/*****
* Game Controller
*/

// Callback function types for PlayerController interface
typedef void (*GameStartFunc)(void* userData, int playerId);
typedef void (*RoundStartFunc)(void* userData, CPiece* hand, int handSize, CWind seatWind, CWind prevalentWind);
typedef void (*ReceiveEventFunc)(void* userData, CEvent event);
typedef CEvent (*RetrieveDecisionFunc)(void* userData);
typedef const char* (*GetNameFunc)(void* userData);

// Controller registration structure
typedef struct ControllerCallbacks {
    GameStartFunc onGameStart;
    RoundStartFunc onRoundStart;
    ReceiveEventFunc onReceiveEvent;
    RetrieveDecisionFunc onRetrieveDecision;
    GetNameFunc getName;
    void* userData; // User data passed to callbacks
} ControllerCallbacks;

// Controller manager functions
char** GetAvailableControllers(size_t* count);
int RegisterController(const char* name, ControllerCallbacks callbacks);
PlayerController* CreateController(const char* name);

/**
* Destructors
*/

// Removing from External Controller Table
int UnregisterController(const char* name);
// Free Controller
void DestroyController(PlayerController* controller);
// Free Game Settings
void FreeGameSettings(GameSettings* settings);
// Free entire list
void FreeControllerList(char** list, size_t count);

#ifdef __cplusplus
}
#endif
