#pragma once

#include "./pieces/Piece.hpp"
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
    PieceType capturedPieceType = EMPTY;
    Colour capturedColor = NONE;

    bool oldWhiteKingMoved = false;
    bool oldBlackKingMoved = false;
    bool oldWhiteKingsideRookMoved = false;
    bool oldWhiteQueensideRookMoved = false;
    bool oldBlackKingsideRookMoved = false;
    bool oldBlackQueensideRookMoved = false;

    std::pair<int, int> oldEnPassantSquare = {-1, -1};

    // Promotion piece
    PieceType promotionPiece = EMPTY;

    // Constructor
    Move(int fX, int fY, int tX, int tY): fromX(fX), fromY(fY), toX(tX), toY(tY) {}

    // Equality operator
    bool operator==(const Move& other) const {
        return fromX == other.fromX 
        && fromY == other.fromY 
        && toX == other.toX 
        && toY == other.toY;
    }

};
