#pragma once

#include <memory>
#include <vector>
#include <iostream>

#include "./View.hpp"
#include "../state/Board.hpp"
#include "../engine/MoveGenerator.hpp"
#include "../engine/MoveLegality.hpp"
#include "../engine/Search.hpp"

class Controller {

    private:

        std::unique_ptr<Board> board;
        std::unique_ptr<View> view;

        bool running;

        bool leftMouseButtonDown;
        SDL_Point clickOffset;
        
        int initialPiecePositionX;
        int initialPiecePositionY;

        bool isPromoting = false;
        Move pendingPromotionMove = Move(-1, -1, -1, -1);

        bool playAgainstAI = true;
        
        Uint32 lastFrameTime;
        int whiteTimeMs;
        int blackTimeMs;

        bool isGameOver = false;
        std::string gameOverText = "";

        void handleEvents();
        void render();
        void checkGameOver();

    public:

        Controller();
        void run();

};