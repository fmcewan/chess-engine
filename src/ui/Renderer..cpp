// Standard
#include <iostream>
#include <vector>

// Third party
#include <SDL2/SDL.h>
#include <SDL_image.h>

// Generic
#include "Game.hpp"
#include "../board/Board.hpp"
#include "../pieces/Piece.hpp"

//// Constructor and Destructor

Game::Game(const char* title, int height, int width): window(nullptr), renderer(nullptr) {

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
				
	}
	else {
		std::cout << SDL_GetError() << std::endl;
	}

	SDL_RenderSetLogicalSize(renderer, 1000, 1000);	

}

Game::~Game() {

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

}

// Getters

std::vector<SDL_Rect> Game::getWhitePieces() {
    return this->whitePieces;
}

std::vector<SDL_Rect> Game::getBlackPieces() {
    return this->blackPieces;
}

SDL_Rect Game::getSelectedPiece() {
    return this->selectedPiece;
}

// Setters

void Game::addWhitePiece(SDL_Rect whitePiece) {
    this->whitePieces.push_back(whitePiece);
}

void Game::addBlackPiece(SDL_Rect blackPiece) {
    this->blackPieces.push_back(blackPiece);
}

void Game::setSelectedPiece(SDL_Rect setSelectedPiece) {
    this->selectedPiece = setSelectedPiece;
}

//// Helper functions

std::pair<int, int> Game::boardPositionToPixel(int x, int y) {
    
    return std::make_pair(120+(x*100), 120+(y*100));

}

std::pair<int, int> Game::gamePixelToBoardPosition(int pixelX, int pixelY) {
    
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

//// Game methods

SDL_Texture* Game::loadTexture(const char* filePath) {

	SDL_Texture* texture = nullptr;

	texture = IMG_LoadTexture(renderer, filePath);

	return texture;

}

SDL_Rect Game::blit(SDL_Texture* texture, int x, int y) {

	SDL_Rect destination;

	destination.x = x;
	destination.y = y;

	SDL_QueryTexture(texture, nullptr, nullptr, &destination.w, &destination.h);

	SDL_RenderCopy(renderer, texture, nullptr, &destination);
    
    return destination;
}

// Board methods

SDL_Texture* Game::loadPieceTextureFromBoard(Piece piece) {
    
    switch(piece.getType()) {
        case PAWN: {
            if (piece.getColor() == BLACK) {
                return loadTexture("../assets/BlackPawn.png"); 
            }
            else {
                 return loadTexture("../assets/WhitePawn.png");
            }
            break;
        }
        case ROOK: {
            if (piece.getColor() == BLACK) {
                return loadTexture("../assets/BlackRook.png");
            }

            else {
                return loadTexture("../assets/WhiteRook.png");
            }
            break;
        }
        case KNIGHT: {
            if (piece.getColor() == BLACK) {
                return loadTexture("../assets/BlackKnight.png");              
            }
            else {
                return loadTexture("../assets/WhiteKnight.png");              
            }
            break;
        }
        case BISHOP: {
            if (piece.getColor() == BLACK) {
                return loadTexture("../assets/BlackBishop.png");              
            }
            else {
                return loadTexture("../assets/WhiteBishop.png");              
            }
            break;
        }
        case QUEEN: {
            if (piece.getColor() == BLACK) {
                return loadTexture("../assets/BlackQueen.png");
            }
            else {
                 return loadTexture("../assets/WhiteQueen.png");
            }
            break;
        }
        case KING: {
            if (piece.getColor() == BLACK) {
                return loadTexture("../assets/BlackKing.png");
            }
            else {
                return loadTexture("../assets/WhiteKing.png");
            }
            break;
        }
        case EMPTY: {
            return nullptr;
            break;
        }
        default:
            break;
    }

}

void Game::createBoard() {
    
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

    SDL_SetRenderDrawColor(renderer, 48, 46, 43, 255);
}

void Game::initialiseBoard(std::vector< std::vector<Square> > board) {
  
    prepareBoard();
    createBoard();
    updateBoard(board);
    presentBoard();

}

void Game::updateBoard(std::vector< std::vector<Square> > board) {

    int x = 0; 
    int y = 0;
       
	for (int i=0; i<board.size(); i++) {
		for (int j=0; j<board[0].size(); j++) {
			Piece piece = board[i][j].getPiece();

            SDL_Texture* pieceTexture = loadPieceTextureFromBoard(piece);
            
            x = boardPositionToPixel(i, j).second;
            y = boardPositionToPixel(i, j).first;
   		    SDL_Rect pieceRect = blit(pieceTexture, x, y);

            if (piece.getColor() == WHITE) {
                addWhitePiece(pieceRect); 
            }
            else if (piece.getColor() == BLACK) {
                addBlackPiece(pieceRect);
            
            }

            pieceTexture = nullptr;
        }
	}
}

void Game::prepareBoard() {
    SDL_SetRenderDrawColor(renderer, 48, 46, 43, 255);
    SDL_RenderClear(renderer);
}

void Game::presentBoard() {
    SDL_SetRenderDrawColor(renderer, 48, 46, 43, 255);
	SDL_RenderPresent(renderer);
}

// Piece methods

void Game::pickupPiece(SDL_Point mousePosition, SDL_Point& clickOffset, Color currentPlayColor) {
    
    std::cout << "Entered pickupPiece" << "\n";

    // Calculating the click offset
    // (you don't always click at the top left hand corner of the piece, which is the case if a click offset isn't used)
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

void Game::dragPiece(SDL_Point mousePosition, SDL_Point clickOffset, const int initialPiecePositionX, const int initialPiecePositionY, std::vector< std::vector<Square> > board) {
   
    // Getting position of piece being moved in a usable form
    auto boardPosition = gamePixelToBoardPosition(initialPiecePositionX, initialPiecePositionY);
    int x = boardPosition.first; 
    int y = boardPosition.second;
   
    // Loading appropriate textures
    SDL_Texture* pieceTexture = loadPieceTextureFromBoard(board[y][x].getPiece());

    // Removing piece after it has been picked up 
    board[y][x].setPiece(Piece(EMPTY, NONE, 0));

    // Moving piece to where the mouse is 
    selectedPiece.x = mousePosition.x - clickOffset.x;
    selectedPiece.y = mousePosition.y - clickOffset.y; 


    // Loading piece where it now is on the screen
    SDL_RenderClear(renderer);
    createBoard(); 
    updateBoard(board);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_UpdateTexture(pieceTexture, &selectedPiece, NULL, 0);
    SDL_RenderCopy(renderer, pieceTexture, NULL, &selectedPiece);
    SDL_RenderPresent(renderer);

    pieceTexture = nullptr;

 }

void Game::placePiece(std::vector< std::vector<Square> > board) {
    
    // Loading the new screen
    SDL_RenderClear(renderer);
    createBoard(); 
    updateBoard(board);
    SDL_RenderPresent(renderer);

    std::cout << "Entered place piece" << "\n";
    
    // Solution for SDL_Rect having no NULL value
    SDL_Rect nullRectangle;

    nullRectangle.x = 0;
    nullRectangle.y = 0;
    nullRectangle.w = 0;
    nullRectangle.h = 0;

    // Setting the selected piece value to null
    setSelectedPiece(nullRectangle);  

}


