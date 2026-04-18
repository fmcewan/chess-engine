// Standard
#include <iostream>
#include <vector>

// Third party
#include <SDL2/SDL.h>

// Generic
#include "View.hpp"

// Constructor and Destructor
View::View(const char* title, int height, int width): window(nullptr), renderer(nullptr) {

    // SDL Init and IMG Init
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && IMG_Init(IMG_INIT_PNG)) {
		
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, height, width, SDL_WINDOW_SHOWN);
        SDL_SetWindowResizable(window, SDL_TRUE);

		if (window == nullptr) {
			std::cout << SDL_GetError();
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		if (renderer == nullptr) {
			std::cout << SDL_GetError() << std::endl;
		}	
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	}

	else {
		std::cout << SDL_GetError() << std::endl;
	}
    
    // TTF Init
    if (TTF_Init() == -1) {
        std::cout << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("assets/font.ttf", 36); 
    if (font == nullptr) {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
    }

	SDL_RenderSetLogicalSize(renderer, 1000, 1000);


    loadAllTextures();

}

View::~View() {

    for (auto const& [id, texture] : textureCache) {
        SDL_DestroyTexture(texture);
    }

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

    TTF_CloseFont(font);
    TTF_Quit();

	SDL_Quit();

}

// Helper functions
std::pair<int, int> View::gamePixelToBoardPosition(int pixelX, int pixelY) {
    
    SDL_Point mousePosition = {pixelX, pixelY};
    std::pair<int, int> boardPosition;

    for (int i=0; i<8; i++) {
        auto squareRow = squares[i];
        for (int j=0; j<8; j++) {
            auto square = squareRow[j];
            if (SDL_PointInRect(&mousePosition, &square)) {
                boardPosition = std::make_pair(i, j);
            }
        }
    }

    return boardPosition;

}

// Text drawing
void View::drawText(const std::string& text, int x, int y) {
    
    if (font == nullptr) return;

    SDL_Color textColor = {255, 255, 255, 255}; 
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect dest = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &dest);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

}


void View::drawPlayerInfo(int whiteTimeMs, int blackTimeMs, std::vector<std::vector<Piece>> grid) {
    
    auto formatTime = [](int ms) {
        if (ms < 0) ms = 0;
        int seconds = (ms / 1000) % 60;
        int minutes = (ms / 1000) / 60;
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
        return std::string(buffer);
    };

    SDL_Rect blackBox = {100, 20, 150, 60};
    SDL_Rect whiteBox = {100, 920, 150, 60};

    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255); 
    SDL_RenderFillRect(renderer, &blackBox);
    SDL_RenderFillRect(renderer, &whiteBox);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); 
    SDL_RenderDrawRect(renderer, &blackBox);
    SDL_RenderDrawRect(renderer, &whiteBox);

    drawText(formatTime(blackTimeMs), 120, 25); 
    drawText(formatTime(whiteTimeMs), 120, 925);

    int wP=0, wN=0, wB=0, wR=0, wQ=0, whiteScore=0;
    int bP=0, bN=0, bB=0, bR=0, bQ=0, blackScore=0;

    for (int y=0; y<8; y++) {
        for (int x=0; x<8; x++) {
            Piece p = grid[y][x];
            
            if (p.getType() == EMPTY) continue;

            if (p.getColour() == WHITE) {
                switch (p.getType()) {
                    case PAWN:   wP++; whiteScore += 1; break;
                    case KNIGHT: wN++; whiteScore += 3; break;
                    case BISHOP: wB++; whiteScore += 3; break;
                    case ROOK:   wR++; whiteScore += 5; break;
                    case QUEEN:  wQ++; whiteScore += 9; break;
                    default: break;
                }
            } 
            else if (p.getColour() == BLACK) {
                switch (p.getType()) {
                    case PAWN:   bP++; blackScore += 1; break;
                    case KNIGHT: bN++; blackScore += 3; break;
                    case BISHOP: bB++; blackScore += 3; break;
                    case ROOK:   bR++; blackScore += 5; break;
                    case QUEEN:  bQ++; blackScore += 9; break;
                    default: break;
                }
            }
        }
    }

    int whiteAdvance = std::max(0, whiteScore - blackScore);
    int blackAdvance = std::max(0, blackScore - whiteScore);
    
    if (blackAdvance > 0) drawText("+" + std::to_string(blackAdvance), 260, 45);
    if (whiteAdvance > 0) drawText("+" + std::to_string(whiteAdvance), 260, 945);

    auto drawTinyPieces = [&](int pieceID, int missingCount, int startX, int startY, int& currentOffset) {
        for (int i = 0; i < missingCount; i++) {
            SDL_Rect dest = {startX + currentOffset, startY, 35, 35}; 
            SDL_RenderCopy(renderer, textureCache[pieceID], nullptr, &dest);
            currentOffset += 25; 
        }
    };

    int blackOffset = 0;
    drawTinyPieces(WHITE*10+PAWN, 8-wP, 250, 20, blackOffset);
    drawTinyPieces(WHITE*10+KNIGHT, 2-wN, 250, 20, blackOffset);
    drawTinyPieces(WHITE*10+BISHOP, 2-wB, 250, 20, blackOffset);
    drawTinyPieces(WHITE*10+ROOK, 2-wR, 250, 20, blackOffset);
    drawTinyPieces(WHITE*10+QUEEN, 1-wQ, 250, 20, blackOffset);
   
    int whiteOffset = 0;
    drawTinyPieces(BLACK*10+PAWN, 8-bP, 250, 920, whiteOffset);
    drawTinyPieces(BLACK*10+KNIGHT, 2-bN, 250, 920, whiteOffset);
    drawTinyPieces(BLACK*10+BISHOP, 2-bB, 250, 920, whiteOffset);
    drawTinyPieces(BLACK*10+ROOK, 2-bR, 250, 920, whiteOffset);
    drawTinyPieces(BLACK*10+QUEEN, 1-bQ, 250, 920, whiteOffset);

}        


// Texture caching and loading methods
int View::getPieceID(Piece piece) {
    // Generates a unique int, e.g., White (0) * 10 + Pawn (1) = 1
    return piece.getColour() * 10 + piece.getType();
}

SDL_Texture* View::loadTexture(const char* filePath) {

	SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);

    if (texture == nullptr) {
        std::cout << "Failed to load texture: " << filePath << " | Error: " << IMG_GetError() << std::endl;
    }

	return texture;

}

void View::loadAllTextures() {

    textureCache[WHITE * 10 + PAWN] = loadTexture("assets/WhitePawn.png");
    textureCache[BLACK * 10 + PAWN] = loadTexture("assets/BlackPawn.png");
    textureCache[WHITE * 10 + ROOK] = loadTexture("assets/WhiteRook.png");
    textureCache[BLACK * 10 + ROOK] = loadTexture("assets/BlackRook.png");
    textureCache[WHITE * 10 + KNIGHT] = loadTexture("assets/WhiteKnight.png");
    textureCache[BLACK * 10 + KNIGHT] = loadTexture("assets/BlackKnight.png");
    textureCache[WHITE * 10 + BISHOP] = loadTexture("assets/WhiteBishop.png");
    textureCache[BLACK * 10 + BISHOP] = loadTexture("assets/BlackBishop.png");
    textureCache[WHITE * 10 + QUEEN] = loadTexture("assets/WhiteQueen.png");
    textureCache[BLACK * 10 + QUEEN] = loadTexture("assets/BlackQueen.png");
    textureCache[WHITE * 10 + KING] = loadTexture("assets/WhiteKing.png");
    textureCache[BLACK * 10 + KING] = loadTexture("assets/BlackKing.png");

}

// Game methods
SDL_Rect View::blit(SDL_Texture* texture, int x, int y) {

	SDL_Rect destination = {x, y, 100, 100};

    if (texture != nullptr) {
        SDL_QueryTexture(texture, nullptr, nullptr, &destination.w, &destination.h);
        SDL_RenderCopy(renderer, texture, nullptr, &destination);
    }

    return destination;

}

std::pair<int, int> View::boardPositionToPixel(int x, int y) {
    return std::make_pair(120+(x*100), 120+(y*100));
}

// Board methods

void View::createBoard() {
    
    squares.clear();
    int colorInt = 0;
    std::vector<SDL_Rect> squaresRow;

    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
  
            SDL_Rect square;
        
            square.x = 100+100*i;
            square.y = 100+100*j;
            square.w = 100;
            square.h = 100;
            
            squaresRow.push_back(square);
            
            
            if (colorInt == 0) {
                SDL_SetRenderDrawColor(renderer, 234, 233, 213, 255);
                colorInt++;
            }
            else if (colorInt == 1) {
                SDL_SetRenderDrawColor(renderer, 90, 113, 147, 255);
                colorInt--;
            }

            SDL_RenderFillRect(renderer, &square);
        }

        squares.push_back(squaresRow);
        squaresRow.clear();

        if (colorInt == 0) {
                SDL_SetRenderDrawColor(renderer, 234, 233, 213, 255);
                colorInt++;
        }
        else if (colorInt == 1) {
                SDL_SetRenderDrawColor(renderer, 90, 113, 147, 255);
                colorInt--;
        }
    
    }

    if (selectedSquare.first != -1) {
        SDL_Rect originRect;
        originRect.x = 100 + 100 * selectedSquare.first;
        originRect.y = 100 + 100 * selectedSquare.second;
        originRect.w = 100;
        originRect.h = 100;
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 150, 100); 
        SDL_RenderFillRect(renderer, &originRect);
    }

    // Highlight legal moves
    for (auto& hint : legalMoveHints) {
        SDL_Rect hintRect;
        hintRect.x = 100 + 100 * hint.first;
        hintRect.y = 100 + 100 * hint.second;
        hintRect.w = 100;
        hintRect.h = 100;

        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 100); 
        SDL_RenderFillRect(renderer, &hintRect);
    }

    SDL_SetRenderDrawColor(renderer, 48, 46, 43, 255);

}

void View::initialiseBoard(std::vector< std::vector<Piece> > board) {
  
    prepareBoard();
    createBoard();
    updateBoard(board);
    presentBoard();

}

void View::updateBoard(std::vector< std::vector<Piece> > board) {

    this->whitePieces.clear();
    this->blackPieces.clear();

    for (int i=0; i<board.size(); i++) {
        for (int j=0; j<board[i].size(); j++) {

            Piece piece = board[i][j];

            if (piece.getType() == EMPTY) continue;

            SDL_Texture* pieceTexture = textureCache[getPieceID(piece)];

            int x = boardPositionToPixel(i, j).second;
            int y = boardPositionToPixel(i, j).first;
            SDL_Rect pieceRect = blit(pieceTexture, x, y);

            if (piece.getColour() == WHITE) {
                whitePieces.push_back(pieceRect);
            }
            else {
                blackPieces.push_back(pieceRect);
            }

        }
    }

}

void View::prepareBoard() {
    SDL_SetRenderDrawColor(renderer, 48, 46, 43, 255);
    SDL_RenderClear(renderer);
}

void View::presentBoard() {
    SDL_SetRenderDrawColor(renderer, 48, 46, 43, 255);
	SDL_RenderPresent(renderer);
}

// Piece methods
void View::pickupPiece(SDL_Point mousePosition, SDL_Point& clickOffset, Colour currentPlayColor) {
    
    if (currentPlayColor == WHITE) {
        for (auto piece : this->whitePieces) {
            if (SDL_PointInRect(&mousePosition, &piece)) {
                selectedPiece = piece;
                clickOffset.x = mousePosition.x - piece.x;
                clickOffset.y = mousePosition.y - piece.y;   
                break;
            }
        }
    }
    else if (currentPlayColor == BLACK) {
        for (auto piece : this->blackPieces) {
            if (SDL_PointInRect(&mousePosition, &piece)) {
                selectedPiece = piece;
                clickOffset.x = mousePosition.x - piece.x;
                clickOffset.y = mousePosition.y - piece.y;   
                break;
            }
        }

    }

}
 
void View::dragPiece(SDL_Point mousePosition, SDL_Point clickOffset, const int initialPiecePositionX, const int initialPiecePositionY, std::vector< std::vector<Piece> > board) {
   
    // Getting position of piece being moved in a usable form
    auto boardPosition = gamePixelToBoardPosition(initialPiecePositionX, initialPiecePositionY);
    int x = boardPosition.first; 
    int y = boardPosition.second;
   
    // Loading appropriate textures
    SDL_Texture* pieceTexture = textureCache[getPieceID(board[y][x])];

    // Removing piece after it has been picked up 
    board[y][x] = Piece(EMPTY, NONE, 0);

    // Moving piece to where the mouse is 
    selectedPiece.x = mousePosition.x - clickOffset.x;
    selectedPiece.y = mousePosition.y - clickOffset.y; 

    // Loading piece where it now is on the screen
    SDL_RenderClear(renderer);
    createBoard(); 
    updateBoard(board);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, pieceTexture, NULL, &selectedPiece);
    SDL_RenderPresent(renderer);

    pieceTexture = nullptr;

 }

void View::drawFrame(std::vector< std::vector<Piece> > board) {
    
    SDL_RenderClear(renderer);
    createBoard(); 
    updateBoard(board);
    selectedPiece = {0, 0, 0, 0};

}

// Move hints

void View::setLegalMoveHints(std::vector< std::pair<int, int> > hints, std::pair<int, int> originalSquare) {

    this->legalMoveHints = hints;
    this->selectedSquare = originalSquare; 

}

void View::clearLegalMoveHints() {

    this->legalMoveHints.clear();
    this->selectedSquare = {-1, -1};

}

// Promotion menu
void View::showPromotionMenu(Colour colour) {
    
    showingPromotionMenu = true;
    promotionColour = colour;
    
    for(int i = 0; i < 4; i++) {
        promotionRects[i] = {300 + (i * 100), 450, 100, 100};
    }

}

void View::hidePromotionMenu() {

    showingPromotionMenu = false;

}

PieceType View::handlePromotionClick(SDL_Point mousePosition) {
    
    if (!showingPromotionMenu) return EMPTY;
    
    PieceType types[4] = {QUEEN, ROOK, BISHOP, KNIGHT};
    
    for (int i = 0; i < 4; i++) {
        if (SDL_PointInRect(&mousePosition, &promotionRects[i])) {
            return types[i]; 
        }
    }
    
    return EMPTY;

}

void View::drawPromotionMenu() {
    
    if (!showingPromotionMenu) return;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); 
    SDL_Rect fullScreen = {0, 0, 1000, 1000};
    SDL_RenderFillRect(renderer, &fullScreen);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 
    for (int i = 0; i < 4; i++) {
        SDL_RenderFillRect(renderer, &promotionRects[i]);
        
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderDrawRect(renderer, &promotionRects[i]);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 
    }

    PieceType types[4] = {QUEEN, ROOK, BISHOP, KNIGHT};
    for (int i = 0; i < 4; i++) {
        Piece tempPiece(types[i], promotionColour, 0);
        SDL_Texture* pieceTexture = textureCache[getPieceID(tempPiece)];
        
        SDL_Rect dest = promotionRects[i];
        SDL_RenderCopy(renderer, pieceTexture, nullptr, &dest);
    }

}

void View::drawGameOverlay(const std::string& text) {
   
    if (text.empty() || font == nullptr) return;
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); 
    SDL_Rect fullScreen = {0, 0, 1000, 1000};
    SDL_RenderFillRect(renderer, &fullScreen);

    SDL_Color textColor = {255, 215, 0, 255}; 
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect dest = {
        500 - (textSurface->w / 2), 
        500 - (textSurface->h / 2), 
        textSurface->w, 
        textSurface->h
    };
    
    SDL_RenderCopy(renderer, textTexture, nullptr, &dest);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    playAgainButton = {300, 550, 180, 60};
    SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255); // Green box
    SDL_RenderFillRect(renderer, &playAgainButton);
    drawText("Restart", 325, 560); 

    quitButton = {520, 550, 180, 60};
    SDL_SetRenderDrawColor(renderer, 150, 50, 50, 255); // Red box
    SDL_RenderFillRect(renderer, &quitButton);
    drawText("Quit", 565, 560);

}

int View::handleGameOverClick(SDL_Point mousePosition) {

    if (SDL_PointInRect(&mousePosition, &playAgainButton)) return 1; 
    if (SDL_PointInRect(&mousePosition, &quitButton)) return 2;   
    return 0;                                               

}