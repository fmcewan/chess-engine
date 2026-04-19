#include "Evaluator.hpp"

int Evaluator::evaluate(Board& board) {

    int score = 0;
    auto grid = board.getGrid();

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Piece p = grid[y][x];
            
            if (p.getType() == EMPTY) continue;

            int pieceValue = 0;
            switch (p.getType()) {
                case PAWN: pieceValue = PAWN_VALUE; break;
                case KNIGHT: pieceValue = KNIGHT_VALUE; break;
                case BISHOP: pieceValue = BISHOP_VALUE; break;
                case ROOK: pieceValue = ROOK_VALUE; break;
                case QUEEN: pieceValue = QUEEN_VALUE; break;
                case KING: pieceValue = 20000; break; 
                default: break;
            }

            if (p.getColour() == WHITE) {
                score += pieceValue;
            } else if (p.getColour() == BLACK) {
                score -= pieceValue;
            }
        
        }
    }

    return score;
}