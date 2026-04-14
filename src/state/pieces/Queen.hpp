#pragma once
#include "Piece.hpp"

class Queen : public Piece {
    
    public: 
        Queen(Colour colour_input): Piece(QUEEN, colour_input, 9) {}
    
};