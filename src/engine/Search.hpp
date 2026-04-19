#pragma once

#include <vector>
#include <algorithm>

#include "../state/Board.hpp"
#include "../state/Move.hpp"
#include "Evaluator.hpp"
#include "MoveGenerator.hpp"
#include "MoveLegality.hpp"

class Search {
    private:
        static int minimax(Board& board, int depth, int alpha, int beta, bool isMaximizingPlayer);

    public:
        static Move getBestMove(Board& board, int depth);
};