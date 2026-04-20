#pragma once

#include "../state/Board.hpp"

class Evaluator {

    public:
        static int evaluate(Board& board);

    private:

        static const int PAWN_VALUE = 100;
        static const int KNIGHT_VALUE = 320;
        static const int BISHOP_VALUE = 330;
        static const int ROOK_VALUE = 500;
        static const int QUEEN_VALUE = 900;
        static const int KING_VALUE = 20000;

        static const int pawnPST[8][8];
        static const int knightPST[8][8];
        static const int bishopPST[8][8];
        static const int rookPST[8][8];

    };