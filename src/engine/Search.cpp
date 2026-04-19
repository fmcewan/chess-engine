#include "Search.hpp"
#include <iostream>

int Search::minimax(Board& board, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    
    if (depth == 0) {
        return Evaluator::evaluate(board);
    }

    std::vector<Move> moves = MoveGenerator::generateAllLegalMoves(board);

    if (moves.empty()) {
        if (MoveLegality::isInCheck(board.getCurrentPlayer(), board)) {
            return isMaximizingPlayer ? -99999 : 99999;
        }
        return 0; 
    }

    if (isMaximizingPlayer) {
        
        int maxEval = -100000;
        
        for (Move& move : moves) {
            board.makeMove(move); 
            int eval = minimax(board, depth - 1, alpha, beta, false);
            board.undoMove(move);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break; 
        
        }
        
        return maxEval;
    } 
    
    else {
        
        int minEval = 100000;
        for (Move& move : moves) {
            board.makeMove(move); 
            int eval = minimax(board, depth - 1, alpha, beta, true);
            board.undoMove(move); 

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break; 
        }

        return minEval;
    
    }
}

Move Search::getBestMove(Board& board, int depth) {

    std::vector<Move> moves = MoveGenerator::generateAllLegalMoves(board);
    Move bestMove = moves[0]; 
    
    bool isMaximizing = (board.getCurrentPlayer() == WHITE);
    int bestValue = isMaximizing ? -100000 : 100000;

    for (Move& move : moves) {
        
        board.makeMove(move);
        int boardValue = minimax(board, depth - 1, -100000, 100000, !isMaximizing);
        board.undoMove(move);

        if (isMaximizing) {
            if (boardValue > bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        } 
        else {
            if (boardValue < bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        }
    
    }
    
    return bestMove;

}