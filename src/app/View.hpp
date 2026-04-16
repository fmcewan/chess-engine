#pragma once

// Standard library includes
#include <vector>
#include <map>

// SDL2 includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Personal includes
#include "../state/Board.hpp"
#include "../state/pieces/Piece.hpp"

class View {

	private:
		
        SDL_Window* window;
		SDL_Renderer* renderer;

        std::vector< std::vector<SDL_Rect> > squares;
        std::vector<SDL_Rect> whitePieces; 
        std::vector<SDL_Rect> blackPieces;
        
        SDL_Rect selectedPiece; 
       
        // Texture caching methods
        std::map<int, SDL_Texture*> textureCache;
        int getPieceByID(Piece piece);
        void loadAllTextures();

        // Board methods
        void createBoard();
		void updateBoard(std::vector< std::vector<Piece> > board);
		void prepareBoard();
		void presentBoard();
        void renderSelectedPiece();

        // Game methods
		SDL_Texture* loadTexture(const char* filePath);
		SDL_Rect blit(SDL_Texture* texture, int x, int y);
        std::pair<int, int> boardPositionToPixel(int x, int y);
	
    public:

        View(const char* title, int height, int width);
        ~View();

        void initialiseBoard(std::vector < std::vector<Piece> > board);
        std::pair<int, int> gamePixelToBoardPosition(int pixelX, int pixelY);

        // Piece methods
        void pickupPiece(SDL_Point mousePosition, SDL_Point& clickOffset, Colour currentPlayColor);
        void dragPiece(SDL_Point mousePosition, SDL_Point clickOffset, const int initialPiecePositionX, const int initialPiecePositionY, std::vector< std::vector<Piece> > board);
        void placePiece(std::vector< std::vector<Piece> > board);

    };
