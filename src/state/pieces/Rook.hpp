#pragma once
#include "Piece.hpp"

class Rook : public Piece {
    
    public:
        Rook(Colour colour_input) : Piece(ROOK, colour_input, 5) {}
        
};