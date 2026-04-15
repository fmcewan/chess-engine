#include "MoveLegality.hpp"


// Public methods 

bool MoveLegality::isLegal(const Move& move, Board& board) {

    auto grid = board.getGrid();

    Piece movingPiece = grid[move.fromY][move.fromX];
    Colour movingColor = movingPiece.getColour();

    if (!isPseudoLegal(move, board)) {
        return false; 
    }
    
    Move moveCopy = move;

    board.makeMove(moveCopy);

    bool isSafe = !isInCheck(movingColor, board);

    board.undoMove(moveCopy);

    return isSafe;

}

bool MoveLegality::isPseudoLegal(const Move& move, Board& board) {

    auto grid = board.getGrid();

    Piece movingPiece = grid[move.fromY][move.fromX];
    Piece targetPiece = grid[move.toY][move.toX];

    if (movingPiece.getColour() != board.getCurrentPlayer()) { 
        return false;
    }

    if (movingPiece.getColour() == targetPiece.getColour()) { 
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

bool MoveLegality::isSquareAttacked(int targetX, int targetY, Colour attackerColor, Board& board) {
    
    auto grid = board.getGrid();

    std::vector<std::pair<int, int>> directions = {
        {0,1}, {0,-1}, {1,0}, {-1,0},   
        {1,1}, {1,-1}, {-1,1}, {-1,-1}  
    };

    // Check for sliding pieces
    for (auto& direction : directions) {
        for (int i = 1; i < 8; i++) {
            int tx = targetX + direction.first * i;
            int ty = targetY + direction.second * i;

            if (tx < 0 || tx > 7 || ty < 0 || ty > 7) {
                break;
            }

            Piece piece = grid[ty][tx];
            if (piece.getType() != EMPTY) {
                if (piece.getColour() == attackerColor) {
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
            
            Piece piece = grid[ty][tx];
            if (piece.getType() == KNIGHT && piece.getColour() == attackerColor) {
                return true;
            }
        }

    }

    // Check for pawns
    int pawnDirection = (attackerColor == BLACK) ? -1 : 1;
    for (int side : {-1, 1}) {
        
        int tx = targetX + side;
        int ty = targetY + pawnDirection;
        
        if (tx >= 0 && tx < 8 && ty >= 0 && ty < 8) {

            Piece piece = grid[ty][tx];
            if (piece.getType() == PAWN && piece.getColour() == attackerColor) {
                return true;
            }

        }
    }

    return false;

}

bool MoveLegality::isInCheck(Colour colour, Board& board) {
    
    std::pair<int, int> kingPosition = board.getKingLocation(colour);
    Colour enemyColor = (colour == WHITE) ? BLACK : WHITE;

    if (kingPosition.first == -1) {
        return false;
    }

    return isSquareAttacked(kingPosition.first, kingPosition.second, enemyColor, board);

}

// Private methods

bool MoveLegality::isPathClear(int fromX, int toX, int fromY, int toY, Board& board) {

    auto grid = board.getGrid();
    int directionX = (toX > fromX) ? 1 : (toX < fromX ? -1 : 0); 
    int directionY = (toY > fromY) ? 1 : (toY < fromY ? -1 : 0); 

    int currentX = fromX + directionX; 
    int currentY = fromY + directionY;

    while (currentX != toX || currentY != toY) {

        if (grid[currentY][currentX].getType() != EMPTY) {
            return false;   
        }

        currentX += directionX;
        currentY += directionY;

    }

    return true;    
}


bool MoveLegality::isLegalMovePawn(const Move& move, Board& board) {

    int initialX = move.fromX;
    int initialY = move.fromY;
    int finalX = move.toX;
    int finalY = move.toY;

    auto grid = board.getGrid();

    Piece initialPiece = grid[initialY][initialX];
    Piece finalPiece = grid[finalY][finalX];

    int direction = (initialPiece.getColour() == BLACK) ? 1 : -1;
        
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

bool MoveLegality::isLegalMoveKnight(const Move& move, Board& board) {

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

bool MoveLegality::isLegalMoveRook(const Move& move, Board& board) {

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

bool MoveLegality::isLegalMoveBishop(const Move& move, Board& board) {

    int initialX = move.fromX;
    int initialY = move.fromY;
    int finalX = move.toX;
    int finalY = move.toY; 
    
    if (std::abs(finalX-initialX) == std::abs(finalY-initialY) && isPathClear(initialX, finalX, initialY, finalY, board) == true) {
        return true;
    }  
    
    return false;

}

bool MoveLegality::isLegalMoveQueen(const Move& move, Board& board) {

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

bool MoveLegality::isLegalMoveKing(const Move& move, Board& board) {

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

