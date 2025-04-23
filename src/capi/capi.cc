#include "capi.h"
#include "statefunctions/statecontroller.h"
#include "types/settings.h"
#include "types/gamestate.h"
#include "controllers/controllermanager.h"
#include "controllers/playercontroller.h"
#include <cstring>
#include <string>
#include <map>
#include <vector>

extern "C" {

/**
* Opaque Type Wrappers
*/

struct GameStateWrapper {
    std::unique_ptr<mahjong::GameState> state;
};

struct PlayerControllerWrapper {
    std::unique_ptr<mahjong::PlayerController> controller;
};

/**
* External Player Controller
*/

class ExternalPlayerController : public mahjong::PlayerController {
public:
    ExternalPlayerController(ControllerCallbacks callbacks)
        : callbacks(callbacks) {}

    static std::unique_ptr<mahjong::PlayerController> New() {
        return nullptr;
    }

    void GameStart(int player_id) override {
        if (callbacks.onGameStart)
            callbacks.onGameStart(callbacks.userData, player_id);
    }

    void RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seat_wind, mahjong::Wind prevalent_wind) override {
        if (callbacks.onRoundStart) {
            // Pieces to CPiece array
            std::vector<CPiece> cHand;
            cHand.reserve(hand.size());
            for (const auto& piece : hand) {
                cHand.push_back(piece.toUint8_t());
            }
            
            // Vector to CPiece array
            // Due to the rules of mahjong, clients will be able to make an assumption
            // for the length of this hand, so we don't need to send the array size
            // along with it
            CPiece* handCopy = new CPiece[cHand.size()];
            std::memcpy(handCopy, cHand.data(), cHand.size() * sizeof(CPiece));
            
            callbacks.onRoundStart(
                callbacks.userData,
                handCopy,
                static_cast<int>(cHand.size()),
                static_cast<CWind>(seat_wind),
                static_cast<CWind>(prevalent_wind)
            );
            
            // We pass handCopy is a piece pointer
            // The callback is responsible for copying the data if needed
            // We free the memory to avoid a leak
            delete[] handCopy;
        }
    }

    void ReceiveEvent(mahjong::Event e) override {
        if (callbacks.onReceiveEvent) {
            CEvent cEvent;
            cEvent.type = static_cast<int>(e.type);
            cEvent.player = e.player;
            cEvent.piece = e.piece;
            cEvent.decision = e.decision;
            
            callbacks.onReceiveEvent(callbacks.userData, cEvent);
        }
    }

    mahjong::Event RetrieveDecision() override {
        if (callbacks.onRetrieveDecision) {
            CEvent cEvent = callbacks.onRetrieveDecision(callbacks.userData);
            
            mahjong::Event e;
            e.type = static_cast<mahjong::Event::Type>(cEvent.type);
            e.player = cEvent.player;
            e.piece = cEvent.piece;
            e.decision = cEvent.decision;
            
            return e;
        }
        return mahjong::kDeclineEvent;
    }

    std::string Name() override {
        if (callbacks.getName)
            return callbacks.getName(callbacks.userData);
        return "External Controller";
    }

private:
    ControllerCallbacks callbacks;
};

/**
* Game Control
*/

static std::map<std::string, ControllerCallbacks> registeredControllers;

mahjong::GameSettings convertGameSettings(const GameSettings* settings) {
    mahjong::GameSettings cppSettings;
    
    cppSettings.seed = settings->seed;
    
    // Char arrays to vector
    if (settings->seatControllers && settings->numControllers > 0) {
        for (size_t i = 0; i < settings->numControllers; i++) {
            if (settings->seatControllers[i]) {
                cppSettings.seatControllers.push_back(settings->seatControllers[i]);
            }
        }
    }
    
    if (settings->overrideWall) {
        // We don't really have a great way to go from standard override wall to
        // C type, since we don't have an easy length indicator.
        // For now, treating it like a string, with each char being a piece
        std::string wallStr(settings->overrideWall);
        std::vector<mahjong::Piece> wallPieces;
        
        for (size_t i = 0; i < wallStr.length(); i++) {
            uint8_t pieceValue = static_cast<uint8_t>(wallStr[i]);
            wallPieces.push_back(mahjong::Piece(pieceValue));
        }
        
        cppSettings.overrideWall = wallPieces;
    }
    
    return cppSettings;
}

int StartGame(const GameSettings* settings, int async) {
    mahjong::GameSettings cppSettings = convertGameSettings(settings);
    return mahjong::StartGame(cppSettings, async != 0);
}

void ExitGame(int game) {
    mahjong::ExitGame(game);
}

GameState* InitGameState(const GameSettings* settings) {
    mahjong::GameSettings cppSettings = convertGameSettings(settings);
    auto state = mahjong::InitGameState(cppSettings);
    return new GameStateWrapper{ std::move(state) };
}

GameState* AdvanceGameState(GameState* state) {
    auto wrapper = static_cast<GameStateWrapper*>(state);
    auto newState = mahjong::AdvanceGameState(std::move(wrapper->state));
    delete wrapper;
    return new GameStateWrapper{ std::move(newState) };
}

void DestroyGameState(GameState* state) {
    auto wrapper = static_cast<GameStateWrapper*>(state);
    delete wrapper;
}

/**
* Controller Management
*/

char** GetAvailableControllers(size_t* count) {
    auto controllers = mahjong::ControllerManager::Instance().GetAvailableControllers();
    *count = controllers.size();
    
    char** result = (char**)malloc(sizeof(char*) * controllers.size());
    for (size_t i = 0; i < controllers.size(); i++) {
        result[i] = strdup(controllers[i].c_str());
    }
    
    return result;
}

// Registers callbacks for a given controller name
int RegisterController(const char* name, ControllerCallbacks callbacks) {
    std::string controllerName(name);
    registeredControllers[controllerName] = callbacks;
    
    auto factory = [controllerName]() -> std::unique_ptr<mahjong::PlayerController> {
        return std::make_unique<ExternalPlayerController>(registeredControllers[controllerName]);
    };
    
    return mahjong::ControllerManager::Instance().RegisterController(factory, controllerName);
}

// Creates a new controller instance, which points to the function callbacks
PlayerControllerWrapper* CreateController(const char* name) {
    try {
        auto controller = mahjong::ControllerManager::Instance().NewController(name);
        return new PlayerControllerWrapper { std::move(controller) };
    }
    catch (...) {
        return nullptr;
    }
}

int UnregisterController(const char* name) {
    std::string controllerName(name);
    auto it = registeredControllers.find(controllerName);
    if (it != registeredControllers.end()) {
        registeredControllers.erase(it);
        return mahjong::ControllerManager::Instance().UnregisterController(controllerName);
    }
    // Controller wasn't found
    return 0;
}

void FreeGameSettings(GameSettings* settings) {
    if (!settings) return;
    
    // Free seat controllers
    if (settings->seatControllers) {
        for (size_t i = 0; i < settings->numControllers; i++) {
            free(settings->seatControllers[i]);
        }
        free(settings->seatControllers);
        settings->seatControllers = nullptr;
    }
    
    // Free override wall
    if (settings->overrideWall) {
        free(settings->overrideWall);
        settings->overrideWall = nullptr;
    }
    
    settings->numControllers = 0;
}


void FreeControllerList(char** list, size_t count) {
    for (size_t i = 0; i < count; i++) {
        free(list[i]);
    }
    free(list);
}

void DestroyController(PlayerControllerWrapper* controller) {
    delete controller;
}

} // extern "C"