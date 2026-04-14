#pragma once
#include "Piece.hpp"

class Knight : public Piece {
    
    public: 
        Knight(Colour colour_input) : Piece(KNIGHT, colour_input, 3) {}

};