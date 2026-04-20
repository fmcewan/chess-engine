#include "./Controller.hpp"

Controller::Controller() {

    board = std::make_unique<Board>();
    view  = std::make_unique<View>("Chess", 1000, 1000);

    blackTimeMs = 600000;

    view->initialiseBoard(board->getGrid());
    running = true;
    leftMouseButtonDown = false;

    whiteTimeMs = 600000;
    blackTimeMs = 600000;

    currentState = MENU;
    playAgainstAI = false;

    lastFrameTime = SDL_GetTicks();

}

// Game loop
void Controller::run() {

    while(running) {
        
        Uint32 currentFrameTime = SDL_GetTicks();
        Uint32 deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        if (currentState == GameState::IN_PLAY && !isPromoting) {
            
            if (board->getCurrentPlayer() == WHITE) {
                whiteTimeMs -= deltaTime;
                if (whiteTimeMs <= 0) {
                    whiteTimeMs = 0;
                    currentState = GameState::GAME_OVER;
                    gameOverText = "Flagged! Black Wins on Time.";
                }
            } 
            
            else {
                blackTimeMs -= deltaTime;
                if (blackTimeMs <= 0) {
                    blackTimeMs = 0;
                    currentState = GameState::GAME_OVER;
                    gameOverText = "Flagged! White Wins on Time.";
                }
            }

            if (playAgainstAI && board->getCurrentPlayer() == BLACK && currentState != GameState::GAME_OVER && !isPromoting) {
                
                std::cout << "AI is thinking..." << std::endl;
                
                Move aiMove = Search::getBestMove(*board, 3); 
                
                board->makeMove(aiMove);
                checkGameOver(); 
                view->clearLegalMoveHints();
            
            }

        }

        handleEvents();
        render();
        SDL_Delay(16);
    
    }

}

// Event handler
void Controller::handleEvents() {

    SDL_Event event;
    SDL_Point mousePosition;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT: {
                running = false;
                break;
            }

            case SDL_MOUSEMOTION: {

                mousePosition = {event.motion.x, event.motion.y};
                
                if (leftMouseButtonDown) { 
                    view->dragPiece(mousePosition, clickOffset, initialPiecePositionX, initialPiecePositionY, board->getGrid());
                }

                break;
            }                

            case SDL_MOUSEBUTTONUP: {
                
                if (!isPromoting && leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    mousePosition = {event.button.x, event.button.y};
                    
                    auto initialPosition = view->gamePixelToBoardPosition(initialPiecePositionX, initialPiecePositionY);
                    auto finalPosition = view->gamePixelToBoardPosition(mousePosition.x, mousePosition.y);
                    
                    Move move = Move(initialPosition.first, initialPosition.second, finalPosition.first, finalPosition.second); 
                    std::vector<Move> legalMoves = MoveGenerator::generateAllLegalMoves(*board);
                    
                    bool isLegal = false;
                    bool isPromotionAttempt = false; // NEW FLAG

                    for (Move& legalMove : legalMoves) {
                        if (move == legalMove) {
                            isLegal = true;
                            move = legalMove; 
                            
                            if (move.isPromotion) {
                                isPromotionAttempt = true;
                            }

                            break;
                        }
                    }

                    if (isLegal) {

                        if (isPromotionAttempt) {
                            isPromoting = true;
                            pendingPromotionMove = move; 
                            view->showPromotionMenu(board->getCurrentPlayer());
                        } 

                        else {
                            board->makeMove(move);
                            checkGameOver();
                        }

                    }

                    view->clearLegalMoveHints();

                }
                
                break;
            }

            case SDL_MOUSEBUTTONDOWN: {

               mousePosition = {event.button.x, event.button.y};

                // MAIN MENU 
                if (currentState == GameState::MENU && event.button.button == SDL_BUTTON_LEFT) {
                    
                    int action = view->handleMainMenuClick(mousePosition);
                    
                    if (action == 1) {
                        playAgainstAI = false;
                        currentState = GameState::IN_PLAY;
                    } 
                    
                    else if (action == 2) {
                        playAgainstAI = true;
                        currentState = GameState::IN_PLAY;
                    } 
                    
                    else if (action == 3) {
                        running = false;
                    }
                    
                    break;
                }

                // GAME OVER
                if (currentState == GameState::GAME_OVER && event.button.button == SDL_BUTTON_LEFT) {
                    int action = view->handleGameOverClick(mousePosition);
                    
                    if (action == 1) {
                        board = std::make_unique<Board>("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                        whiteTimeMs = 600000;
                        blackTimeMs = 600000;
                        currentState = GameState::MENU; 
                        leftMouseButtonDown = false;
                        isPromoting = false;
                        view->clearLegalMoveHints();
                    }

                    else if (action == 2) {
                        running = false;
                    }
                    
                    break; 
                }

                // PROMOTION CLICKS 
                if (currentState == GameState::IN_PLAY && isPromoting && event.button.button == SDL_BUTTON_LEFT) {
                    
                    PieceType selectedType = view->handlePromotionClick(mousePosition);
                    
                    if (selectedType != EMPTY) {
                        pendingPromotionMove.promotionPiece = selectedType;
                        board->makeMove(pendingPromotionMove); 
                        checkGameOver();
                        isPromoting = false;
                        view->hidePromotionMenu();
                    }
                    
                    break;
                
                }
                
                // --- PIECE PICKUP ---
                if (currentState == GameState::IN_PLAY && !leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    view->pickupPiece(mousePosition, clickOffset, board->getCurrentPlayer());

                    initialPiecePositionX = mousePosition.x;
                    initialPiecePositionY = mousePosition.y;
                
                    auto initialPosition = view->gamePixelToBoardPosition(initialPiecePositionX, initialPiecePositionY);
        
                    if (initialPosition.first != -1) { 
                        std::vector<Move> allLegalMoves = MoveGenerator::generateAllLegalMoves(*board);
                        std::vector<std::pair<int, int>> hintsForSelectedPiece;

                        for (Move& m : allLegalMoves) {
                            if (m.fromX == initialPosition.first && m.fromY == initialPosition.second) {
                                hintsForSelectedPiece.push_back({m.toX, m.toY});
                            }
                        }
                        view->setLegalMoveHints(hintsForSelectedPiece, initialPosition);
                    }
                } 
                
                break;
            }

            default: {
                break;
            }
            
        }
                    
    }

}

// Renderer
void Controller::render() {

    if (currentState == MENU) {
        view->drawMainMenu();
    }
    else {
        if (!leftMouseButtonDown) {
            view->drawFrame(board->getGrid());
            view->drawPlayerInfo(whiteTimeMs, blackTimeMs, board->getGrid());
            view->drawPromotionMenu();

            if (currentState == GAME_OVER) {
                view->drawGameOverlay(gameOverText);
            }

            view->presentBoard();
        }
    }

}

void Controller::checkGameOver() {
    
    std::vector<Move> nextMoves = MoveGenerator::generateAllLegalMoves(*board);
    
    if (nextMoves.empty()) {
        
        currentState = GAME_OVER;
        
        bool inCheck = MoveLegality::isInCheck(board->getCurrentPlayer(), *board);
        
        if (inCheck) {
            if (board->getCurrentPlayer() == WHITE) {
                gameOverText = "Checkmate! Black Wins.";
            } 
            else {
                gameOverText = "Checkmate! White Wins.";
            }
        } 
        
        else {
            gameOverText = "Stalemate! It's a draw.";
        }
    
    }

}