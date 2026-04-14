#pragma once

// Standard library includes
#include <vector>
#include <string>
#include <utility>
    
// Header includes
#include "Move.hpp"
#include "../utilities/FEN.hpp"
#include "./pieces/Piece.hpp"

struct FENData;

class Board {

    private:

        int height;
        int width;
        std::vector< std::vector<Piece>> grid;

        std::pair<int, int> enPassantSquare;
        
        int totalPoints;
        int halfmoves;
        int fullmoves;

        Colour currentPlayer;
        
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
        
        bool getHasKingsideRookMoved(Colour colour);
        bool getHasQueensideRookMoved(Colour colour);

        std::string getFEN();
        
        std::pair<int,int> getKingLocation(Colour color);
        Colour getCurrentPlayer();

        Piece getPiece(int x, int y);
        std::pair<int, int> getEnPassantSquare();
        std::vector< std::vector<Piece> > getGrid();

        // Setters
        void setHeight(int setHeight);
        void setWidth(int setWidth);
        void setHalfmoves(int setHalfmoves);
        void setFullmoves(int setFullmoves);
        void setSquare(Piece setPiece, int x, int y);
        void setCurrentPlayer(Colour setCurrentPlayer);
        void setEnPassantSquare(std::pair<int, int> setEnPassantSquare);
        void setHasKingMoved(Colour currentPlayer, bool setHasKingMoved);

        // Board methods   
        void applyFENData(FENData data);
        void printGrid();

        // Castling methods 
        bool hasKingMoved(Colour color);
        bool canCastleKingside(Colour color);
        bool canCastleQueenside(Colour color);
        
        // Move methods 
        void makeMove(Move& move);
        void undoMove(const Move& move);

};
