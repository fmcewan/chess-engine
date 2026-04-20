#pragma once

// Standard library includes
#include <vector>
#include <map>
#include <string>

// SDL2 includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
        std::pair<int, int> selectedSquare = {-1, -1};

        std::map<int, SDL_Texture*> textureCache;

        std::vector<std::pair<int,int>> legalMoveHints;

        // Buttons
        SDL_Rect playAgainButton;
        SDL_Rect quitButton;
        
        SDL_Rect pvpButton;
        SDL_Rect pveButton;
        SDL_Rect menuQuitButton;

        // Fonts
        TTF_Font* font;

        // Text drawing
        void drawText(const std::string& text, int x, int y);
        
        // Texture caching and loading methods
        int getPieceID(Piece piece);
		SDL_Texture* loadTexture(const char* filePath);
        void loadAllTextures();

        // Board methods
        void createBoard();
		void updateBoard(std::vector< std::vector<Piece> > board);
		void prepareBoard();
        void renderSelectedPiece();

        // Game methods
		SDL_Rect blit(SDL_Texture* texture, int x, int y);
        std::pair<int, int> boardPositionToPixel(int x, int y);

        // Promotion menu 
        bool showingPromotionMenu = false;
        Colour promotionColour = NONE;
        SDL_Rect promotionRects[4];

    public:

        View(const char* title, int height, int width);
        ~View();

        void drawPlayerInfo(int whiteTimeMS, int blackTimeMS, std::vector<std::vector<Piece>> grid);
        void drawGameOverlay(const std::string& text); 
        void drawMainMenu();

        int handleGameOverClick(SDL_Point mousePosition);
        int handleMainMenuClick(SDL_Point mousePosition);

        void presentBoard();

        void initialiseBoard(std::vector < std::vector<Piece> > board);
        std::pair<int, int> gamePixelToBoardPosition(int pixelX, int pixelY);

        // Piece methods
        void pickupPiece(SDL_Point mousePosition, SDL_Point& clickOffset, Colour currentPlayColor);
        void dragPiece(SDL_Point mousePosition, SDL_Point clickOffset, const int initialPiecePositionX, const int initialPiecePositionY, std::vector< std::vector<Piece> > board);
        void drawFrame(std::vector< std::vector<Piece> > board);

        // Move hints
        void setLegalMoveHints(std::vector<std::pair<int, int>> hints, std::pair<int, int> originSquare);
        void clearLegalMoveHints();

        // Promotion
        void showPromotionMenu(Colour colour);
        void hidePromotionMenu();
        void drawPromotionMenu();
        PieceType handlePromotionClick(SDL_Point mousePosition);

    };
