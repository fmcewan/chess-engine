// Standard
#include <iostream>
#include <vector>

// Third party
#include <SDL2/SDL.h>

// Generic
#include "View.hpp"

// Constructor and Destructor
View::View(const char* title, int height, int width): window(nullptr), renderer(nullptr) {

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

View::~View() {

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

}

//// Helper functions

std::pair<int, int> View::boardPositionToPixel(int x, int y) {
    return std::make_pair(120+(x*100), 120+(y*100));
}

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

//// Game methods

SDL_Texture*View::loadTexture(const char* filePath) {

	SDL_Texture* texture = nullptr;

	texture = IMG_LoadTexture(renderer, filePath);

	return texture;

}

SDL_Rect View::blit(SDL_Texture* texture, int x, int y) {

	SDL_Rect destination;

	destination.x = x;
	destination.y = y;

	SDL_QueryTexture(texture, nullptr, nullptr, &destination.w, &destination.h);

	SDL_RenderCopy(renderer, texture, nullptr, &destination);
    
    return destination;

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

    SDL_SetRenderDrawColor(renderer, 48, 46, 43, 255);
}

void View::initialiseBoard(std::vector< std::vector<Piece> > board) {
  
    prepareBoard();
    createBoard();
    updateBoard(board);
    presentBoard();

}

void View::updateBoard(std::vector< std::vector<Piece> > board) {

    int x = 0; 
    int y = 0;
       
	for (int i=0; i<board.size(); i++) {
		for (int j=0; j<board[0].size(); j++) {
			Piece piece = board[i][j];

            SDL_Texture* pieceTexture = loadPieceTextureFromBoard(piece);
            
            x = boardPositionToPixel(i, j).second;
            y = boardPositionToPixel(i, j).first;
   		    SDL_Rect pieceRect = blit(pieceTexture, x, y);

            if (piece.getColour() == WHITE) {
                addWhitePiece(pieceRect); 
            }
            else if (piece.getColour() == BLACK) {
                addBlackPiece(pieceRect);
            
            }

            pieceTexture = nullptr;
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

void View::dragPiece(SDL_Point mousePosition, SDL_Point clickOffset, const int initialPiecePositionX, const int initialPiecePositionY, std::vector< std::vector<Piece> > board) {
   
    // Getting position of piece being moved in a usable form
    auto boardPosition = gamePixelToBoardPosition(initialPiecePositionX, initialPiecePositionY);
    int x = boardPosition.first; 
    int y = boardPosition.second;
   
    // Loading appropriate textures
    SDL_Texture* pieceTexture = loadPieceTextureFromBoard(board[y][x]);

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
    SDL_UpdateTexture(pieceTexture, &selectedPiece, NULL, 0);
    SDL_RenderCopy(renderer, pieceTexture, NULL, &selectedPiece);
    SDL_RenderPresent(renderer);

    pieceTexture = nullptr;

 }

void View::placePiece(std::vector< std::vector<Piece> > board) {
    
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


