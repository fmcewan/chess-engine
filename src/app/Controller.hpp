#pragma once

#include <memory>

#include "./Renderer.hpp"
#include "../state/Board.hpp"

class Controller {

    private:

        std::unique_ptr<Board> board;
        std::unique_ptr<Renderer> controller;

        bool running;

        bool leftMouseButtonDown;
        SDL_Point clickOffset;
        
        int initialPiecePositionX;
        int initialPiecePositionY;

        void handleEvents();
        void render();

    public:

        Controller();
        void run();

};