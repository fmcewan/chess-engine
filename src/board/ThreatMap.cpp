// Standard library includes
#include <iostream>
#include <set>


// Header include
#include "Board.hpp"
#include "ThreatMap.hpp" 
#include "Move.hpp"

// Constructors

ThreatMap::ThreatMap(std::vector< std::vector<Square> > setBoard, Color setColor) {
    board = setBoard;
    color = setColor;
    threatMap = buildThreatMap(getPossibleMoves());
}

ThreatMap::ThreatMap(std::vector< std::vector<Square> > setBoard, std::vector<Location> setThreatMap, Color setColor) {
    board = setBoard;
    color = setColor;
    threatMap = setThreatMap;
}

// Getters

std::vector<Location> ThreatMap::getThreatMap() {
    return threatMap;
}

// Setters

void ThreatMap::setThreatMap(std::vector<Location> setThreatMap) {
    threatMap = setThreatMap;
}

// Possible moves for pieces

std::vector<Move> ThreatMap::possibleMovesPawn(int x, int y) {

    std::vector<Move> possibleMoves;

    Piece piece = board[y][x].getPiece();
    int direction = (piece.getColor() == BLACK) ? 1 : -1;

    if (x+1 < 8 && x+1 >= 0 && y+1*direction < 8 && y+1*direction >= 0) {
        possibleMoves.push_back(Move(board, color, x, y, x+1, y+1*direction));
    }
    else if (x-1 < 8 && x-1 >= 0 && y+1*direction < 8 && y+1*direction >= 0) {
        possibleMoves.push_back(Move(board, color, x, y, x-1, y+1*direction));
    }

    return possibleMoves;

}

std::vector<Move> ThreatMap::possibleMovesRook(int x, int y) {
    
    std::vector<Move> possibleMoves;

    // Possible horizontal moves
    
    for (int i=0; i<8; i++) {
        if (i!=x) {
            possibleMoves.push_back(Move(board, color, x, y, i, y));
        }
    }

    // Possible vertical moves
    
    for (int i=0; i<8; i++) {
        if (i!=y) {
            possibleMoves.push_back(Move(board, color, x, y, x, i));
        }
    }

    return possibleMoves;
        
}

std::vector<Move> ThreatMap::possibleMovesBishop(int x, int y) {

    std::vector<Move> possibleMoves; 

    int j = 0;

    // Positive diagonal
    
    j = y-1;

    for (int i=x+1; i<8; i++) {
        if (j >= 8) break;
        possibleMoves.push_back(Move(board, color, x, y, i, j));
        j--;
    }
    
    j = y+1;

    for (int i=x-1; i>=0; i--) {
        if (j <= 0) break;
        possibleMoves.push_back(Move(board, color, x, y, i, j));
        j++;
    }
    
    // Negative diagonal
   
    j = y+1;

    for (int i=x+1; i<8; i++) { 
        if (j >= 8) break;
        possibleMoves.push_back(Move(board, color, x, y, i, j));
        j++;
    }
    
    j = y-1;

    for (int i=x-1; i>=0; i--) {
        if (j <= 0) break;
        possibleMoves.push_back(Move(board, color, x, y, i, j));
        j--;
    }
        
    return possibleMoves; 

}

std::vector<Move> ThreatMap::possibleMovesKnight(int x, int y) {

    std::vector<Move> possibleMoves;
    
    std::vector<int> xOffsets = {-2, -2, -1, -1, 1, 1, 2, 2};
    std::vector<int> yOffsets = {1, -1, 2, -2, 2, -2, 1, -1}; 

    for (int i=0; i<xOffsets.size(); i++) {
        possibleMoves.push_back(Move(board, color, x, y, x+xOffsets[i], y+yOffsets[i]));
    }

    return possibleMoves;
}

std::vector<Move> ThreatMap::possibleMovesQueen(int x, int y) {
    
    std::vector<Move> possibleMoves;

    for (auto move : possibleMovesBishop(x, y)) {
        possibleMoves.push_back(move);   
    }

    for (auto move : possibleMovesRook(x, y)) {
        possibleMoves.push_back(move);
    }


    return possibleMoves; 
}

std::vector<Move> ThreatMap::possibleMovesKing(int x, int y) {

    std::vector<Move> possibleMoves;

    
    for (int i=y-1; i<=y+1; i++) {
        for (int j=x-1; j<=x+1; j++) {
            possibleMoves.push_back(Move(board, color, x, y, i, j));
        }
    }

    return possibleMoves; 
}

// ThreatMap Methods

std::vector<Move> ThreatMap::getPossibleMoves() {

    // Gets all of the unique possible moves to squares

    std::vector<Move> possibleMoves;

    int height = board.size();
    int width = board[0].size();

    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            
            Square square = board[y][x];
            Piece piece = square.getPiece();

            switch (piece.getType()) {
                case PAWN: {
                    for (auto move : possibleMovesPawn(x, y)) {
                        if (std::find(possibleMoves.begin(), possibleMoves.end(), move) != possibleMoves.end()) {
                            possibleMoves.push_back(move);
                        }
                    }
                    break;
                }
                case ROOK:
                    for (auto move : possibleMovesRook(x, y)) possibleMoves.push_back(move);
                    break;
                case BISHOP:
                    for (auto move : possibleMovesBishop(x, y)) possibleMoves.push_back(move);
                    break;
                case KNIGHT:
                    for (auto move : possibleMovesKnight(x, y)) possibleMoves.push_back(move);
                    break;
                case QUEEN:
                    for (auto move : possibleMovesQueen(x, y)) possibleMoves.push_back(move);
                    break;
                case KING:
                    for (auto move : possibleMovesKing(x, y)) possibleMoves.push_back(move);
                    break;
                default:
                    break;
            }

        }
    }

    return possibleMoves;

}

std::vector<Location> ThreatMap::buildThreatMap(std::vector<Move> possibleMoves) {

    // Gets all the squares that it is legal for pieces to move to (different than attacking)
    
    std::vector<Location> threatMap; 

    for (auto move : possibleMoves) {

        if (move.isLegalMove() == true) {
            
            Location location;

            location.x = move.getFinalX();
            location.y = move.getFinalY();

            threatMap.push_back(location);
        }

    }

    return threatMap;
}

