// Standard library includes
#include <cmath>
#include <iostream>

// Header include
#include "MoveGenerator.hpp"
#include "MoveLegality.hpp"

void printMoves(std::vector<Move> moves) {
    for (Move move : moves) {
        std::cout << "("<< move.fromX << "->" << move.toX << ", " << move.fromY << "->" << move.toX << ")";
    }
    std::cout << std::endl;
}

std::vector<Move> MoveGenerator::generateAllLegalMoves(Board& board) {

    std::vector<Move> legalMoves;
    auto grid = board.getGrid();

    Colour turn = board.getCurrentPlayer();

    for (int y=0; y<8; y++) {
        for (int x=0; x<8; x++) {

            Piece piece = grid[y][x];

            if (piece.getColour() == turn) {

                switch (piece.getType()) {
                    case PAWN: {
                        addPawnMoves(x, y, board, legalMoves);
                        break;
                    }
                    case KNIGHT: {
                        addKnightMoves(x, y, board, legalMoves); 
                        break;
                    }
                    case KING: {
                        addKingMoves(x, y, board, legalMoves); 
                        break;
                    }
                    case ROOK:
                    case BISHOP:
                    case QUEEN: {
                        addSlidingMoves(x, y, board, legalMoves); 
                        break;
                    }
                    default: 
                        break;
                }

            }

        }
    }

    printMoves(legalMoves);

    return legalMoves;

}

// Adder functions

void MoveGenerator::addPawnMoves(int x, int y, Board& board, std::vector<Move>& moves) {

    auto grid = board.getGrid();

    Piece piece = grid[y][x];
    Colour colour = piece.getColour();

    int direction = (colour == BLACK) ? 1 : -1;
    int startRank = (colour == BLACK) ? 1 : 6;
    int promotionRank = (colour == BLACK) ? 7 : 0;

    int nextY = y + direction;

    if (nextY >= 0 && nextY <= 7 && grid[nextY][x].getType() == EMPTY) {

        if (nextY == promotionRank) {
            addPromotionMoves(x, y, x, nextY, board, moves);
        } 
        else {
            Move m(x, y, x, nextY);
            if (MoveLegality::isLegal(m, board)) moves.push_back(m);

            int doubleY = y + 2 * direction;
            if (y == startRank && grid[doubleY][x].getType() == EMPTY) {
                Move dm(x, y, x, doubleY);
                if (MoveLegality::isLegal(dm, board)) moves.push_back(dm);
            }
        }

    }

    for (int side : {-1, 1}) {

        int tx = x + side;
        int ty = y + direction;
        
        if (tx < 0 || tx > 7 || ty < 0 || ty > 7) {
            continue;
        }

        Piece target = grid[ty][tx];
        
        if (target.getType() != EMPTY && target.getColour() != colour) {
            if (ty == promotionRank) {
                addPromotionMoves(x, y, tx, ty, board, moves);
            } else {
                Move m(x, y, tx, ty);
                if (MoveLegality::isLegal(m, board)) moves.push_back(m);
            }
        }

        // En Passant
        else if (board.getEnPassantSquare().first == tx && board.getEnPassantSquare().second == ty) {
            Move m(x, y, tx, ty);
            m.isEnPassant = true;
            if (MoveLegality::isLegal(m, board)) moves.push_back(m);
        }

    }
}

void MoveGenerator::addPromotionMoves(int fx, int fy, int tx, int ty, Board& board, std::vector<Move>& moves) {
    
    PieceType promotions[] = { QUEEN, ROOK, BISHOP, KNIGHT};

    for (PieceType type : promotions) {
        Move m(fx, fy, tx, ty);
        m.isPromotion = true;
        m.promotionPiece = type;
        
        if (MoveLegality::isLegal(m, board)) {
            moves.push_back(m);
        }
    
    }

}
        
 void MoveGenerator::addKnightMoves(int x, int y, Board& board, std::vector<Move>& moves) {

    int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
    int dy[] = {2, 1, -1, -2, -2, -1, 1, 2};

    for (int i = 0; i < 8; i++) {
        int tx = x + dx[i];
        int ty = y + dy[i];

        if (tx >= 0 && tx < 8 && ty >= 0 && ty < 8) {
            Move m(x, y, tx, ty);
            if (MoveLegality::isLegal(m, board)) {
                moves.push_back(m);
            }
        }
    }

}

void MoveGenerator::addSlidingMoves(int x, int y, Board& board, std::vector<Move>& moves) {

    auto grid = board.getGrid();
    Piece piece = grid[y][x];
    
    std::vector<std::pair<int, int>> directions;

    if (piece.getType() == ROOK || piece.getType() == QUEEN) {
        directions.insert(directions.end(), {{0, 1}, {0, -1}, {1, 0}, {-1, 0}});
    }
    
    if (piece.getType() == BISHOP || piece.getType() == QUEEN) {
        directions.insert(directions.end(), {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}});
    }

    for (auto& direction : directions) {
        for (int dist = 1; dist < 8; dist++) {
            int tx = x + (direction.first * dist);
            int ty = y + (direction.second * dist);

            if (tx < 0 || tx > 7 || ty < 0 || ty > 7) break;

            Move m(x, y, tx, ty);
            
            if (MoveLegality::isLegal(m, board)) {
                moves.push_back(m);
            }

            if (grid[ty][tx].getType() != EMPTY) {
                break; 
            }
        }
    } 

}

void MoveGenerator::addKingMoves(int x, int y, Board& board, std::vector<Move>& moves) {

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            
            if (dx == 0 && dy == 0) {
                continue;
            }
            
            int tx = x + dx;
            int ty = y + dy;

            if (tx >= 0 && tx < 8 && ty >= 0 && ty < 8) {

                Move m(x, y, tx, ty);

                if (MoveLegality::isLegal(m, board)) {
                    moves.push_back(m);
                }

            }
        }
    }

    if (!board.hasKingMoved(board.getCurrentPlayer())) {
        
        if (board.canCastleKingside(board.getCurrentPlayer())) {
            Move m(x, y, x + 2, y);
            m.isCastling = true;
            if (MoveLegality::isLegal(m, board)) moves.push_back(m);
        }
        
        if (board.canCastleQueenside(board.getCurrentPlayer())) {
            Move m(x, y, x - 2, y);
            m.isCastling = true;
            if (MoveLegality::isLegal(m, board)) moves.push_back(m);
        }

    }

}



