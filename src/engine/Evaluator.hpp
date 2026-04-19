#pragma once

#include "../state/Board.hpp"

class Evaluator {

    public:
        static int evaluate(Board& board);

    private:

        static const int PAWN_VALUE = 100;
        static const int KNIGHT_VALUE = 100;
        static const int BISHOP_VALUE = 100;
        static const int ROOK_VALUE = 100;
        static const int QUEEN_VALUE = 100;

};