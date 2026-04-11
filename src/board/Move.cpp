#include "Move.hpp"
#include "../pieces/Piece.hpp"
#include "ThreatMap.hpp"

#include <stdexcept>
#include <vector>
#include <iostream>

// Constructor
Move::Move(std::vector<std::vector<Square>> setBoard, Color setCurrentPlayer, int setInitialX, int setInitialY, int setFinalX, int setFinalY) { 
    
    board = setBoard;
    currentPlayer = setCurrentPlayer;
    enPassantSquare.x = -1;
    enPassantSquare.y = -1;

    initialX = setInitialX;
    initialY = setInitialY;
    finalX = setFinalX;
    finalY = setFinalY;

}

// Getters
const int Move::getInitialX() {
	return initialX;
}

const int Move::getInitialY() {
	return initialY;
}

const int Move::getFinalX() {
	return finalX;
}

const int Move::getFinalY() {
	return finalY;
}

Location Move::getEnPassantSquare() {
    return enPassantSquare;
}

// Movement legality

bool Move::isEmptyPath() {
    
    int diagonal = (std::abs(finalX-initialX) == std::abs(finalY-initialY)) ? 0 : 1;

    switch (diagonal) {

        case 0: {

            int directionX = (finalX > initialX) ? 1 : -1;
            int directionY = (finalY > initialY) ? 1 : -1;

            int i=initialX+directionX;
            int j=initialY+directionY;

            while (i != finalX && j != finalY) {
                
                Piece piece = board[j][i].getPiece();

                if (piece.getType() != EMPTY) {
                    return false;
                }

                i += 1*directionX;
                j += 1*directionY;
           
            }

            break;
        }

        case 1: {
 
            if (finalY == initialY) {

                int direction = (finalY >= initialY) ? 1 : 0;

                switch (direction) {
                    case 1: {
                        for (int i=initialX+1; i<finalX; i++) {

                            Piece piece = board[finalY][i].getPiece();

                            if (piece.getType() != EMPTY) {
                                return false;
                            }
                        }   
                        break;
                    }
                    case 0: {
                        for (int i=initialX-1; i>finalX; i--) {

                            Piece piece = board[finalY][i].getPiece();

                            if (piece.getType() != EMPTY) {
                                return false;
                            }
                        }

                        break;
                    }
                } 
            }

            else if (finalX == initialX) {
                
                int direction = (finalY >= initialY) ? 1 : 0;

                switch (direction) {
                    case 1: {

                        for (int i=initialY+1; i<finalY; i++) {

                            Piece piece = board[i][finalX].getPiece();

                            if (piece.getType() != EMPTY) {
                                return false;    
                            }

                        }

                        break;
                    }
                    case 0: {

                        for (int i=initialY-1; i>finalY; i--) {
        
                            Piece piece = board[i][finalX].getPiece();

                            if (piece.getType() != EMPTY) {
                                return false;
                            }

                        }

                        break;
                    }
                }
            
            }

            break;
        }

        default:
            break;

    }

    return true;    
}

bool Move::isLegalMove() {

    Piece initialPiece = board[initialY][initialX].getPiece();
    Piece finalPiece = board[finalY][finalX].getPiece();
   
    // If the initial or final piece is not on the board, then it can't be a legal move

    if (initialX < 0 && initialX > 7 && finalX < 0 && finalX > 7 && initialY < 0 && initialY > 7 && finalY < 0 && finalY > 7) {
        return false;
    }

    // If the piece is the wrong color or the same color as the takee, it can't be a legal move

    if (initialPiece.getColor() == finalPiece.getColor()) {
        return false;
    }
    else if (initialPiece.getColor() != currentPlayer) {
        return false;
    }
    
    // Normal move logic

    switch (initialPiece.getType()) {
        case PAWN: {
            return isLegalMovePawn();
            break;
        }
        case ROOK: {
            return isLegalMoveRook();
            break;
        }                       
        case BISHOP: {
            return isLegalMoveBishop();    
            break;
        }
        case KNIGHT: {
            return isLegalMoveKnight();
            break;
        }
        case QUEEN: {
            return isLegalMoveQueen();
            break;
        }
        case KING: {
            return isLegalMoveKing(); 
            break;
        }
        default:
            break;

    }

    return false;

}

// Piece movement legality

bool Move::isLegalMovePawn() {

    Piece initialPiece = board[initialY][initialX].getPiece();
    Piece finalPiece = board[finalY][finalX].getPiece();

    int direction = (initialPiece.getColor() == BLACK) ? 1 : -1;
        
    if (finalX == initialX && finalY == initialY+direction && finalPiece.getType() == EMPTY) {
        return true;
    }  
    else if (finalX == initialX && finalY == initialY+2*direction && finalPiece.getType() == EMPTY && ((initialY == 1 && direction == 1) || (initialY == 6 && direction == -1))) {
        enPassantSquare.x = finalX;
        enPassantSquare.y = finalY-1*direction;
        return true;
    }
    else if (((finalX == initialX+1) || (finalX == initialX-1)) && finalY == initialY+1*direction && finalPiece.getType() != EMPTY)  {
        return true;
    }

    return false;

}

bool Move::isLegalMoveKnight() {

    // TODO: Convert to switch statement

    // Kings can move in an L shape from its current position, 
    // including over pieces so the path does not have to be empty
    // 
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

bool Move::isLegalMoveRook() {
    
    if (finalX == initialX && finalY != initialY && isEmptyPath() == true) {
        return true; 
    } 
    else if (finalY == initialY && finalX != initialX && isEmptyPath() == true) {
        return true;
    }

    return false;

}

bool Move::isLegalMoveBishop() {

    if (std::abs(finalX-initialX) == std::abs(finalY-initialY) && isEmptyPath() == true) {
        return true;
    }  
    
    return false;

}

bool Move::isLegalMoveQueen() {
    
    if (finalX == initialX && finalY != initialY && isEmptyPath() == true) {
        return true; 
    } 
    else if (finalY == initialY && finalX != initialX && isEmptyPath() == true) {
        return true;
    }
    else if (std::abs(finalX-initialX) == std::abs(finalY-initialY) && isEmptyPath() == true) {
        return true;
    }  

    return false;

}

bool Move::isLegalMoveKing() {
    
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

// King check logic

bool Move::isInCheck() {

    return false;

}

bool Move::isInCheckmate() {

    return false;

}
