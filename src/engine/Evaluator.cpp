#include "Evaluator.hpp"

const int Evaluator::pawnPST[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    { 50, 50, 50, 50, 50, 50, 50, 50 },
    { 10, 10, 20, 30, 30, 20, 10, 10 },
    {  5,  5, 10, 25, 25, 10,  5,  5 },
    {  0,  0,  0, 20, 20,  0,  0,  0 },
    {  5, -5,-10,  0,  0,-10, -5,  5 },
    {  5, 10, 10,-20,-20, 10, 10,  5 },
    {  0,  0,  0,  0,  0,  0,  0,  0 }
};

const int Evaluator::knightPST[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

const int Evaluator::bishopPST[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};

const int Evaluator::rookPST[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    {  5, 10, 10, 10, 10, 10, 10,  5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    {  0,  0,  0,  5,  5,  0,  0,  0 }
};

int Evaluator::evaluate(Board& board) {

    int whiteScore = 0;
    int blackScore = 0;

    auto grid = board.getGrid();

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Piece p = grid[y][x];
            
            if (p.getType() == EMPTY) continue;

            int pieceValue = 0;
            int positionalBonus = 0;

            int pstRow = (p.getColour() == WHITE) ? y : (7 - y);

            switch (p.getType()) {
                case PAWN:
                    pieceValue = PAWN_VALUE;
                    positionalBonus = pawnPST[pstRow][x];
                    break;

                case KNIGHT:
                    pieceValue = KNIGHT_VALUE;
                    positionalBonus = knightPST[pstRow][x];
                    break;

                case BISHOP:
                    pieceValue = BISHOP_VALUE;
                    positionalBonus = bishopPST[pstRow][x];
                    break;

                case ROOK:
                    pieceValue = ROOK_VALUE;
                    positionalBonus = rookPST[pstRow][x];
                    break; 

                case QUEEN:
                    pieceValue = QUEEN_VALUE;
                    break;

                case KING:
                    pieceValue = KING_VALUE;
                    break;

                default:
                    break;
            
            if (p.getColour() == WHITE) {
                whiteScore += (pieceValue + positionalBonus);
            } else if (p.getColour()  == BLACK) {
                blackScore += (pieceValue + positionalBonus);
            }
        
            }
        }

    }
    
    return whiteScore - blackScore;
}