#pragma once

// Standard library includes
#include <vector>
#include <string>
#include <utility>
    
// Header includes
#include "Square.hpp"
#include "Move.hpp"
#include "../pieces/Piece.hpp"


class Board {

    private:

        // Board attributes
        std::vector< std::vector<Square>> grid;
        std::pair<int, int> enPassantSquare;
        int totalPoints;
        int halfmoves;
        int fullmoves;
        int height;
        int width;

        // Player attributes
        Color currentPlayer;
        
        // Castling attributes
        bool hasKingMovedWhite;
        bool hasKingMovedBlack;

        bool hasKingsideRookMovedBlack;
        bool hasQueensideRookMovedBlack;
        bool hasKingsideRookMovedWhite;
        bool hasQueensideRookMovedWhite;
        
        bool canCastleKingsideBlack;
        bool canCastleQueensideBlack;
        bool canCastleKingsideWhite;
        bool canCastleQueensideWhite;

    public:
        
        // Constructors
        Board();
        Board(std::string FEN);

        // Getters
        int getHeight();
        int getWidth();
        int getHalfmoves();
        int getFullmoves();
        int getTotalPoints();
        
        Location getKingLocation(Color color);
        Color getCurrentPlayer();

        Square getSquare(int x, int y);
        std::pair<int, int> getEnPassantSquare();
        std::vector< std::vector<Square> > getGrid();

        // Setters
        void setHeight(int setHeight);
        void setWidth(int setWidth);
        void setHalfmoves(int setHalfmoves);
        void setFullmoves(int setFullmoves);
        void setSquare(Piece setPiece, int x, int y);
        void setCurrentPlayer(Color setCurrentPlayer);
        void setEnPassantSquare(std::pair<int, int> setEnPassantSquare);
        void setHasKingMoved(Color currentPlayer, bool setHasKingMoved);

        // Board methods   
        void initialiseBoard(std::string FEN);
        void printBoard();

        // Castling methods 
        bool hasKingMoved(Color color);
        bool canCastleKingside(Color color);
        bool canCastleQueenside(Color color);
        
        // Move methods
        
        // Lightweight methods for piece movement simulation
        bool executeMove(Move move); 
        bool undoMove(Move move);

        bool makeMove(Move move);

};
