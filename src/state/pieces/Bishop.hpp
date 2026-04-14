#pragma once
#include "Piece.hpp"

class Bishop : public Piece {

    public:
        Bishop(Colour colour_input): Piece(BISHOP, colour_input, 3) {}

};