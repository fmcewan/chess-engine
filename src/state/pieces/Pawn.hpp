#pragma once
#include "Piece.hpp"

class Pawn : public Piece {
    
    public:
        // Constructor
        Pawn(Colour colour_input): Piece(PAWN, colour_input, 1) {}

};