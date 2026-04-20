#include "Search.hpp"

void Search::orderMoves(std::vector<Move>& moves, Board& board) {
    auto grid = board.getGrid();
   
    // Sorts captures and promotions greater than other moves, random otherwise
    std::sort(moves.begin(), moves.end(), [&grid](const Move& a, const Move& b) {
        int scoreA = 0;
        int scoreB = 0;
        
        if (grid[a.toY][a.toX].getType() != EMPTY) scoreA += 10;
        if (grid[b.toY][b.toX].getType() != EMPTY) scoreB += 10;
        
        if (a.isPromotion) scoreA += 5;
        if (b.isPromotion) scoreB += 5;
        
        return scoreA > scoreB; 
    });
}

Move Search::getBestMove(Board& board, int depth) {

    std::vector<Move> legalMoves = MoveGenerator::generateAllLegalMoves(board);
    
    orderMoves(legalMoves, board); 

    Move bestMove = legalMoves.empty() ? Move(-1,-1,-1,-1) : legalMoves[0];
    int bestValue = (board.getCurrentPlayer() == WHITE) ? -999999 : 999999;

    for (Move& move : legalMoves) {
        board.makeMove(move);
        
        if (board.getCurrentPlayer() == WHITE) { 
            
            int boardValue = minimax(board, depth - 1, -999999, 999999, true);
            if (boardValue < bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        } 
        
        else {
            
            int boardValue = minimax(board, depth - 1, -999999, 999999, false);
            
            if (boardValue > bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        
        }
        
        board.undoMove(move); 
    
    }

    return bestMove;
}

int Search::minimax(Board& board, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    
    if (depth == 0) {
        return Evaluator::evaluate(board);
    }

    std::vector<Move> moves = MoveGenerator::generateAllLegalMoves(board);

    if (moves.empty()) {

        if (MoveLegality::isInCheck(board.getCurrentPlayer(), board)) {
            return isMaximizingPlayer ? -99999 : 99999;
        }
        
        return 0; // Statemate 
    
    }

    orderMoves(moves, board);

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