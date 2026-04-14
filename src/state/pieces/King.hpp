#pragma once

#include <limits>
#include "Piece.hpp"

class King : public Piece {
    
    public:
        King(Colour colour_input) : Piece(KING, colour_input, std::numeric_limits<int>::max()) {}
};