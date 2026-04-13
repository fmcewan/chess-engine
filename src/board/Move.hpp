#pragma once
#include "Square.hpp"
#include "../pieces/Piece.hpp"
#include <vector>


struct Move {

    // Location
    int fromX, fromY;
    int toX, toY;

    // Move type flags
    bool isCastling = false;
    bool isEnPassant = false;
    bool isPromotion = false;

    // "Memory" for undoing moves
    Pieces capturedPieceType = EMPTY;
    Color capturedColor = NONE;
    bool oldWhiteKingMoved;
    bool oldBlackKingMoved;
    bool oldWhiteKingsideRookMoved;
    bool oldWhiteQueensideRookMoved;
    bool oldBlackKingsideRookMoved;
    bool oldBlackQueensideRookMoved;

    // Promotion piece
    Pieces promotionPiece = EMPTY;

    // Constructor
    Move(int fX, int fY, int tX, int tY): fromX(fX), fromY(fY), toX(tX), toY(tY) {}

    // Equality operator
    bool operator==(const Move& other) const {
        return fromX == other.fromX && fromY == other.fromY && toX == other.toX && toY == other.toY;
    }
    
};
