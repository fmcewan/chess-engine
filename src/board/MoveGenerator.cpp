// Standard library includes
#include <cmath>

// Header include
#include "MoveGenerator.hpp" 

std::vector<Move> MoveGenerator::generateAllLegalMoves(Board& board) {

    std::vector<Move> legalMoves;
    auto grid = board.getGrid();
    Color turn = board.getCurrentPlayer();

    for (int y=0; y<8; y++) {
        for (int x=0; x<8; x++) {

            Piece piece = grid[y][x].getPiece();

                if (piece.getColor() == turn) {
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

    return legalMoves;
}

bool MoveGenerator::isLegal(const Move& move, Board& board) {

    auto grid = board.getGrid();

    Piece movingPiece = grid[move.toY][move.toX].getPiece();
    Color movingColor = movingPiece.getColor();
    
    if (!isPseudoLegal(move, board)) {
        return false; 
    }

    board.executeMove(move); 

    bool stillInCheck = !isInCheck(movingColor, board);

    board.undoMove(move);

    return !stillInCheck;

}

bool MoveGenerator::isPseudoLegal(const Move& move, Board& board) {

    auto grid = board.getGrid();

    Piece movingPiece = grid[move.fromY][move.fromX].getPiece();
    Piece targetPiece = grid[move.toY][move.toX].getPiece();

    if (movingPiece.getColor() != board.getCurrentPlayer()) { 
        return false;
    }

    if (movingPiece.getColor() == targetPiece.getColor()) { 
        return false; 
    }

    bool pseudoLegal = false;

    switch (movingPiece.getType()) {
        case PAWN: {
            pseudoLegal = isLegalMovePawn(move, board); 
            break;
        }
        case ROOK: {
            pseudoLegal = isLegalMoveRook(move, board); 
            break;
        }
        case KNIGHT: {
            pseudoLegal = isLegalMoveKnight(move, board); 
            break;
        }
        case BISHOP: {
            pseudoLegal = isLegalMoveBishop(move, board); 
            break;
        }
        case QUEEN: {
            pseudoLegal = isLegalMoveQueen(move, board); 
            break;
        }
        case KING: {
            pseudoLegal = isLegalMoveKing(move, board); 
            break;
        }
        default: {
            return false;
        }
    }

    if (!pseudoLegal) {
        return false;
    }

    return true;
    
}

bool MoveGenerator::isSquareAttacked(int targetX, int targetY, Color attackerColor, Board& board) {
    
    auto grid = board.getGrid();

    std::vector<std::pair<int, int>> directions = {
        {0,1}, {0,-1}, {1,0}, {-1,0},   
        {1,1}, {1,-1}, {-1,1}, {-1,-1}  
    };

    for (auto& direction : directions) {
        for (int i = 1; i < 8; i++) {
            int tx = targetX + direction.first * i;
            int ty = targetY + direction.second * i;

            if (tx < 0 || tx > 7 || ty < 0 || ty > 7) {
                break;
            }

            Piece piece = grid[ty][tx].getPiece();
            if (piece.getType() != EMPTY) {
                if (piece.getColor() == attackerColor) {
                    // Check if this piece is the right type to attack from this direction
                    bool isOrthogonal = (direction.first == 0 || direction.second == 0);
                    
                    if (isOrthogonal && (piece.getType() == ROOK || piece.getType() == QUEEN)) {
                        return true;
                    }

                    if (!isOrthogonal && (piece.getType() == BISHOP || piece.getType() == QUEEN)) {
                        return true; 
                    } 
                
                }
                break; 
            }
        }
    }

    // Check for knights
    std::vector<int> kx = {1, 2, 2, 1, -1, -2, -2, -1};
    std::vector<int> ky = {2, 1, -1, -2, -2, -1, 1, 2};
    
    for (int i = 0; i < 8; i++) {
        int tx = targetX + kx[i];
        int ty = targetY + ky[i];
        
        if (tx >= 0 && tx < 8 && ty >= 0 && ty < 8) {
            
            Piece piece = grid[ty][tx].getPiece();
            if (piece.getType() == KNIGHT && piece.getColor() == attackerColor) {
                return true;
            }
        }

    }

    // Cheeck for pawns
    int pawnDirection = (attackerColor == BLACK) ? -1 : 1; // Looking for Black pawn? Check 'above' target square.
    for (int side : {-1, 1}) {
        
        int tx = targetX + side;
        int ty = targetY + pawnDirection;
        
        if (tx >= 0 && tx < 8 && ty >= 0 && ty < 8) {

            Piece piece = grid[ty][tx].getPiece();
            if (piece.getType() == PAWN && piece.getColor() == attackerColor) {
                return true;
            }

        }
    }

    return false;

}

bool MoveGenerator::isInCheck(Color color, Board& board) {
    
    Location kingPosition = board.getKingLocation(color);
    Color enemyColor = (color == WHITE) ? BLACK : WHITE;

    return isSquareAttacked(kingPosition.x, kingPosition.y, enemyColor, board);

}

// Possible moves for pieces

bool MoveGenerator::isPathClear(int initialX, int finalX, int initialY, int finalY, Board& board) {
    
    auto grid = board.getGrid();
    int directionX = (finalX > initialX) ? 1 : (finalX < initialX ? -1 : 0); 
    int directionY = (finalY > initialY) ? 1 : (finalY < initialY ? -1 : 0); 

    int currentX = initialX + directionX; 
    int currentY = initialY + directionY;

    while (currentX != finalX || currentY != finalY) {
        if (grid[currentY][currentX].getPiece().getType() != EMPTY) {
            return false;   
        }

        currentX += directionX;
        currentY += directionY;

    }

    return true;    

}


// Piece movement legality

bool MoveGenerator::isLegalMovePawn(const Move& move, Board& board) {

    int initialX = move.fromX;
    int initialY = move.fromY;
    int finalX = move.toX;
    int finalY = move.toY;

    auto grid = board.getGrid();

    Piece initialPiece = grid[initialY][initialX].getPiece();
    Piece finalPiece = grid[finalY][finalX].getPiece();

    int direction = (initialPiece.getColor() == BLACK) ? 1 : -1;
        
    if (finalX == initialX && finalY == initialY+direction && finalPiece.getType() == EMPTY) {
        return true;
    }  
    else if (finalX == initialX && finalY == initialY+2*direction && finalPiece.getType() == EMPTY && ((initialY == 1 && direction == 1) || (initialY == 6 && direction == -1))) {
        return true;
    }
    else if (((finalX == initialX+1) || (finalX == initialX-1)) && finalY == initialY+1*direction && finalPiece.getType() != EMPTY)  {
        return true;
    }

    return false;

}

bool MoveGenerator::isLegalMoveKnight(const Move& move, Board& board) {

    int initialX = move.fromX;
    int initialY = move.fromY;
    int finalX = move.toX;
    int finalY = move.toY;

    if (finalX == initialX+2 && finalY == initialY+1) {
        return true;
    }
    else if (finalX == initialX+1 && finalY == initialY+2) {
        return true;
    }
    else if (finalX == initialX+2 && finalY == initialY-1) {
        return true;
    }
    else if (finalX == initialX+1 && finalY == initialY-2) {
        return true;
    }
    else if (finalX == initialX-1 && finalY == initialY+2) {
        return true;
    }
    else if (finalX == initialX-2 && finalY == initialY+1) {
        return true;
    }
    else if (finalX == initialX-2 && finalY == initialY-1) {
        return true;
    }
    else if (finalX == initialX-1 && finalY == initialY-2) {
        return true;
    }
    
    return false;
}

bool MoveGenerator::isLegalMoveRook(const Move& move, Board& board) {

    int initialX = move.fromX;
    int initialY = move.fromY;
    int finalX = move.toX;
    int finalY = move.toY; 
    
    if (finalX == initialX && finalY != initialY && isPathClear(initialX, finalX, initialY, finalY, board) == true) {
        return true; 
    } 
    else if (finalY == initialY && finalX != initialX && isPathClear(initialX, finalX, initialY, finalY, board) == true) {
        return true;
    }

    return false;

}

bool MoveGenerator::isLegalMoveBishop(const Move& move, Board& board) {

    int initialX = move.fromX;
    int initialY = move.fromY;
    int finalX = move.toX;
    int finalY = move.toY; 
    
    if (std::abs(finalX-initialX) == std::abs(finalY-initialY) && isPathClear(initialX, finalX, initialY, finalY, board) == true) {
        return true;
    }  
    
    return false;

}

bool MoveGenerator::isLegalMoveQueen(const Move& move, Board& board) {

    int initialX = move.fromX;
    int initialY = move.fromY;
    int finalX = move.toX;
    int finalY = move.toY; 
    
    if (finalX == initialX && finalY != initialY && isPathClear(initialX, finalX, initialY, finalY, board) == true) {
        return true; 
    } 
    else if (finalY == initialY && finalX != initialX && isPathClear(initialX, finalX, initialY, finalY, board) == true) {
        return true;
    }
    else if (std::abs(finalX-initialX) == std::abs(finalY-initialY) && isPathClear(initialX, finalX, initialY, finalY, board) == true) {
        return true;
    }  

    return false;

}

bool MoveGenerator::isLegalMoveKing(const Move& move, Board& board) {

    int initialX = move.fromX;
    int initialY = move.fromY;
    int finalX = move.toX;
    int finalY = move.toY;
    
    if (finalX == initialX+1 && finalY == initialY+1) {
        return true;
    }
    else if (finalX == initialX+1 && finalY == initialY) {
        return true;
    }
    else if (finalX == initialX+1 && finalY == initialY-1) {
        return true;
    }
    else if (finalX == initialX && finalY == initialY-1) {
        return true;
    }
    else if (finalX == initialX && finalY == initialY+1) {
        return true;
    }
    else if (finalX == initialX-1 && finalY == initialY+1) {
        return true;
    }
    else if (finalX == initialX-1 && finalY == initialY) {
        return true;
    }
    else if (finalX == initialX-1 && finalY == initialY-1) {
        return true;
    }

    return false;

}

// Adder functions

void MoveGenerator::addPawnMoves(int x, int y, Board& board, std::vector<Move>& moves) {

    auto grid = board.getGrid();

    Piece piece = grid[y][x].getPiece();
    Color color = piece.getColor();

    int direction = (color == BLACK) ? 1 : -1;
    int startRank = (color == BLACK) ? 1 : 6;
    int promotionRank = (color == BLACK) ? 7 : 0;

    // 1. Forward Pushes
    int nextY = y + direction;
    if (nextY >= 0 && nextY <= 7 && grid[nextY][x].getPiece().getType() == EMPTY) {
        if (nextY == promotionRank) {
            // Add Queen, Rook, Bishop, Knight options
            addPromotionMoves(x, y, x, nextY, board, moves);
        } 
        else {
            Move m(x, y, x, nextY);
            if (isLegal(m, board)) moves.push_back(m);

            // Double Push
            int doubleY = y + 2 * direction;
            if (y == startRank && grid[doubleY][x].getPiece().getType() == EMPTY) {
                Move dm(x, y, x, doubleY);
                if (isLegal(dm, board)) moves.push_back(dm);
            }
        }
    }

    // 2. Captures (Regular & En Passant)
    for (int side : {-1, 1}) {
        int tx = x + side;
        int ty = y + direction;
        
        if (tx < 0 || tx > 7 || ty < 0 || ty > 7) {
            continue;
        }

        Piece target = grid[ty][tx].getPiece();
        
        // Regular Capture
        if (target.getType() != EMPTY && target.getColor() != color) {
            if (ty == promotionRank) {
                addPromotionMoves(x, y, tx, ty, board, moves);
            } else {
                Move m(x, y, tx, ty);
                if (isLegal(m, board)) moves.push_back(m);
            }
        }
        // En Passant
        else if (board.getEnPassantSquare().first == tx && board.getEnPassantSquare().second == ty) {
            Move m(x, y, tx, ty);
            m.isEnPassant = true;
            if (isLegal(m, board)) moves.push_back(m);
        }

    }
}

// Helper to expand a single promotion move into 4 options
void MoveGenerator::addPromotionMoves(int fx, int fy, int tx, int ty, Board& board, std::vector<Move>& moves) {
    
    Pieces promos[] = { QUEEN, ROOK, BISHOP, KNIGHT };
    for (Pieces type : promos) {
        Move m(fx, fy, tx, ty);
        m.isPromotion = true;
        m.promotionPiece = type;
        
        if (isLegal(m, board)) {
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
            if (isLegal(m, board)) {
                moves.push_back(m);
            }
        }
    }

}

void MoveGenerator::addSlidingMoves(int x, int y, Board& board, std::vector<Move>& moves) {

    auto grid = board.getGrid();
    Piece piece = grid[y][x].getPiece();
    
    // 1. Define the possible directions for this piece
    std::vector<std::pair<int, int>> directions;
    
    // Rooks and Queens move orthogonally
    if (piece.getType() == ROOK || piece.getType() == QUEEN) {
        directions.insert(directions.end(), {{0, 1}, {0, -1}, {1, 0}, {-1, 0}});
    }
    
    // Bishops and Queens move diagonally
    if (piece.getType() == BISHOP || piece.getType() == QUEEN) {
        directions.insert(directions.end(), {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}});
    }

    // 2. "Shoot" a ray in each direction
    for (auto& direction : directions) {
        for (int dist = 1; dist < 8; dist++) {
            int tx = x + (direction.first * dist);
            int ty = y + (direction.second * dist);

            // Stop if the ray goes off the board
            if (tx < 0 || tx > 7 || ty < 0 || ty > 7) break;

            Move m(x, y, tx, ty);
            
            // The Referee decides if the move is allowed (checks for friendly fire and King safety)
            if (isLegal(m, board)) {
                moves.push_back(m);
            }

            // HIT DETECTION: Stop the ray if we hit any piece (Friend or Foe)
            // Note: Unlike your old code, this correctly handles "jumps"
            if (grid[ty][tx].getPiece().getType() != EMPTY) {
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

                if (isLegal(m, board)) {
                    moves.push_back(m);
                }

            }
        }
    }

    if (!board.hasKingMoved(board.getCurrentPlayer())) {
        
        if (board.canCastleKingside(board.getCurrentPlayer())) {
            Move m(x, y, x + 2, y);
            m.isCastling = true;
            if (isLegal(m, board)) moves.push_back(m);
        }
        
        if (board.canCastleQueenside(board.getCurrentPlayer())) {
            Move m(x, y, x - 2, y);
            m.isCastling = true;
            if (isLegal(m, board)) moves.push_back(m);
        }

    }

}



