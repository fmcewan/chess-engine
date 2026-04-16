// Standard library includes
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <algorithm>
#include <set>

// Personal includes
#include "Board.hpp"
#include "Move.hpp"

#include "../engine/MoveGenerator.hpp"
#include "../engine/MoveLegality.hpp"

#include "./pieces/Piece.hpp"
#include "./pieces/Rook.hpp"
#include "./pieces/Pawn.hpp"
#include "./pieces/Bishop.hpp"
#include "./pieces/Knight.hpp"
#include "./pieces/King.hpp"
#include "./pieces/Queen.hpp"

// Constructors

// Default
Board::Board(): height(8), width(8) {
    this->grid = std::vector< std::vector<Piece> >(height, std::vector<Piece>(width, Piece(EMPTY, NONE, 0)));
    FENData data = FEN::parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    applyFENData(data);
}

// FEN
Board::Board(std::string FEN): height(8), width(8) {
    this->grid = std::vector< std::vector<Piece> >(height, std::vector<Piece>(width, Piece(EMPTY, NONE, 0)));
    FENData data = FEN::parse(FEN);
    
    applyFENData(data);
}

// Getters
int Board::getHeight() {
    return height;
}

int Board::getWidth() {
    return width;
}

int Board::getHalfmoves() {
    return halfmoves;
}

int Board::getFullmoves() {
    return fullmoves;
}

bool Board::getHasKingsideRookMoved(Colour colour) {
    return (colour == WHITE) ? hasKingsideRookMovedWhite : hasKingsideRookMovedBlack;
}

bool Board::getHasQueensideRookMoved(Colour colour) {
    return (colour == WHITE) ? hasQueensideRookMovedWhite : hasQueensideRookMovedBlack;
}

Piece Board::getPiece(int x, int y) {
    return grid[y][x];
}

Colour Board::getCurrentPlayer() {
    return currentPlayer;
}

std::pair<int, int> Board::getEnPassantSquare() {
    return enPassantSquare;
}

std::vector< std::vector<Piece> > Board::getGrid() {
    return grid;
}

std::string Board::getFEN() {
    return FEN::generate(*this);
}

// Setters
void Board::setHeight(int setHeight) {
    height = setHeight;
}

void Board::setWidth(int setWidth) {
    width = setWidth;
}

void Board::setHalfmoves(int setHalfmoves) {
    halfmoves = setHalfmoves; 
} 

void Board::setFullmoves(int setFullmoves) {
    fullmoves = setFullmoves;
}

void Board::setSquare(Piece setPiece, int setX, int setY) {
    grid[setY][setX] = setPiece;
}

void Board::setCurrentPlayer(Colour setCurrentPlayer) {
    currentPlayer = setCurrentPlayer;
}

void Board::setEnPassantSquare(std::pair<int, int> setEnPassantSquare) {
    enPassantSquare = setEnPassantSquare;
}

// Board methods

void Board::applyFENData(FENData data) {

   this->grid = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece(EMPTY, NONE, 0)));

    int x = 0;
    int y = 0;

    for (char c : data.piecePlacement) {
        if (c == '/') {
            y++;
            x = 0;
        } 
        else if (std::isdigit(c)) {
            x += (c - '0'); 
        } 
        else {
            Colour colour = std::isupper(c) ? WHITE : BLACK;
            char type = std::tolower(c);
            
            if (type == 'p') setSquare(Pawn(colour), x, y);
            else if (type == 'r') setSquare(Rook(colour), x, y);
            else if (type == 'n') setSquare(Knight(colour), x, y);
            else if (type == 'b') setSquare(Bishop(colour), x, y);
            else if (type == 'q') setSquare(Queen(colour), x, y);
            else if (type == 'k') setSquare(King(colour), x, y);
            
            x++;
        }
    }

    this->currentPlayer = data.currentPlayer;
    this->enPassantSquare = data.enPassantSquare;
    this->halfmoves = data.halfmoveClock;
    this->fullmoves = data.fullmoveNumber;

    hasKingMovedWhite = true;
    hasKingMovedBlack = true; // Default to moved, then clear if in FEN
    hasKingsideRookMovedWhite = true;
    hasQueensideRookMovedWhite = true;
    hasKingsideRookMovedBlack = true;
    hasQueensideRookMovedBlack = true;

    for (char c : data.castlingRights) {
        if (c == 'K') { 
            hasKingMovedWhite = false; 
            hasKingsideRookMovedWhite = false; 
        }
        if (c == 'Q') { 
            hasKingMovedWhite = false; 
            hasQueensideRookMovedWhite = false; 
        }
        if (c == 'k') { 
            hasKingMovedBlack = false; 
            hasKingsideRookMovedBlack = false; 
        }
        if (c == 'q') {
            hasKingMovedBlack = false;
            hasQueensideRookMovedBlack = false; 
        }
    }

}

void Board::printGrid() {
 
    for (int i=0; i<this->getHeight(); i++) {
        for (int j=0; j<this->getWidth(); j++) {

            Piece piece = grid[i][j];

            switch (piece.getType()) {
                case PAWN: {
                    if (piece.getColour() == BLACK) {
                        std::cout << "p";
                    }
                    else {
                        std::cout << "P";
                    }
                    break;
                }

                case ROOK: {
                    if (piece.getColour() == BLACK) {
                        std::cout << "r";
                    }
                    else {
                        std::cout << "R";
                    }
                    break;
                }
                case BISHOP: {
                    if (piece.getColour() == BLACK) {
                        std::cout << "b";
                    }
                    else {
                        std::cout << "B";
                    }
                    break;
                }
                case KNIGHT: {
                    if (piece.getColour() == BLACK) {
                        std::cout << "n";
                    }
                    else {
                        std::cout << "N";
                    }
                    break;
                }
                case KING: {
                    if (piece.getColour() == BLACK) {
                        std::cout << "k";
                    }
                    else {
                        std::cout << "K";
                    }
                    break;
                }
                case QUEEN: {
                    if (piece.getColour() == BLACK) {
                        std::cout << "q";
                    }
                    else {
                        std::cout << "Q";
                    }
                    break;
                }
                case EMPTY:
                    std::cout << " ";
                    break;
                default:
                    break;
            }
        }
        std::cout << std::endl;
    }
}

/* Castling methods */
std::pair<int,int> Board::getKingLocation(Colour colour) {

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Piece piece = grid[y][x];
            if (piece.getType() == KING && piece.getColour() == colour) {
                return {x, y};
            }
        }
    }

    return {-1, -1}; 

}

bool Board::hasKingMoved(Colour colour) {
    return (colour == WHITE) ? hasKingMovedWhite : hasKingMovedBlack; 
}

bool Board::canCastleKingside(Colour colour) {
    
    int y = (colour == WHITE) ? 7 : 0;
    
    if (colour == WHITE) {
        if (hasKingMovedWhite || hasKingsideRookMovedWhite) return false;
    } else {
        if (hasKingMovedBlack|| hasKingsideRookMovedBlack) return false;
    }

    if (grid[y][5].getType() != EMPTY || 
        grid[y][6].getType() != EMPTY) {
        return false;
    }
    
    Colour enemyColor = (colour == WHITE) ? BLACK : WHITE;
    if (MoveLegality::isSquareAttacked(4, y, enemyColor, *this) ||
        MoveLegality::isSquareAttacked(5, y, enemyColor, *this) ||
        MoveLegality::isSquareAttacked(6, y, enemyColor, *this)) {
        return false;
    }

    return true;
}

bool Board::canCastleQueenside(Colour colour) {

    int y = (colour == WHITE) ? 7 : 0;
    
    if (colour == WHITE) {
        if (hasKingMovedWhite || hasQueensideRookMovedWhite) return false;
    } else {
        if (hasKingMovedBlack|| hasQueensideRookMovedBlack) return false;
    }

    if (grid[y][1].getType() != EMPTY || 
        grid[y][2].getType() != EMPTY || 
        grid[y][3].getType() != EMPTY) {
        return false;
    }
    
    Colour enemyColor = (colour == WHITE) ? BLACK : WHITE;
    if (MoveLegality::isSquareAttacked(4, y, enemyColor, *this) ||
        MoveLegality::isSquareAttacked(3, y, enemyColor, *this) ||
        MoveLegality::isSquareAttacked(2, y, enemyColor, *this)) {
        return false;
    }

    return true;

}

/* Move methods */
void Board::makeMove(Move& move) {
    
    Piece initialPiece = grid[move.fromY][move.fromX];
    Piece emptyPiece = Piece(EMPTY, NONE, 0);
    int direction = (initialPiece.getColour() == BLACK) ? 1 : -1;

    move.capturedPieceType = grid[move.toY][move.toX].getType();
    move.capturedColor = grid[move.toY][move.toX].getColour();

    move.oldWhiteKingMoved = hasKingMovedWhite;
    move.oldBlackKingMoved = hasKingMovedBlack;
    move.oldWhiteKingsideRookMoved = hasKingsideRookMovedWhite;
    move.oldWhiteQueensideRookMoved = hasQueensideRookMovedWhite;
    move.oldBlackKingsideRookMoved = hasKingsideRookMovedBlack;
    move.oldBlackQueensideRookMoved = hasQueensideRookMovedBlack;

    move.oldEnPassantSquare = this->getEnPassantSquare();

    if (initialPiece.getType() == KING) {
        if (initialPiece.getColour() == WHITE) hasKingMovedWhite = true;
        else hasKingMovedBlack = true;
    }

    if (move.fromX == 7 && move.fromY == 7 || move.toX == 7 && move.toY == 7) hasKingsideRookMovedWhite = true;
    if (move.fromX == 0 && move.fromY == 7 || move.toX == 0 && move.toY == 7) hasQueensideRookMovedWhite = true;
    if (move.fromX == 7 && move.fromY == 0 || move.toX == 7 && move.toY == 0) hasKingsideRookMovedBlack = true;
    if (move.fromX == 0 && move.fromY == 0 || move.toX == 0 && move.toY == 0) hasQueensideRookMovedBlack = true;

    if (move.isEnPassant) { 
        grid[move.toY - direction][move.toX] = emptyPiece;
    }

    if (move.isCastling) {
        if (move.toX == 6) { 
            grid[move.toY][5] = grid[move.toY][7];
            grid[move.toY][7] = emptyPiece;
        } 
        else if (move.toX == 2) { 
            grid[move.toY][3] = grid[move.toY][0];
            grid[move.toY][0] = emptyPiece;
        }
    }

    if (move.isPromotion) {
        grid[move.toY][move.toX] = Piece(move.promotionPiece, initialPiece.getColour(), 9);
    } 
    else {
        grid[move.toY][move.toX] = initialPiece;
    }
    
    grid[move.fromY][move.fromX] = emptyPiece; 
    
    if (initialPiece.getType() == PAWN && std::abs(move.toY - move.fromY) == 2) {
        this->setEnPassantSquare({move.fromX, move.fromY + direction});
    } 
    else {
        this->setEnPassantSquare({-1, -1});
    }    

    setCurrentPlayer((this->currentPlayer == BLACK) ? WHITE : BLACK);
   
}


void Board::undoMove(const Move& move) {

    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;

    int direction = (currentPlayer == BLACK) ? 1 : -1;
    Piece movedPiece = grid[move.toY][move.toX];

    if (move.isPromotion) {
        grid[move.fromY][move.fromX] = Piece(PAWN, currentPlayer, 1);
    } 
    else {
        grid[move.fromY][move.fromX] = movedPiece;
    }

    grid[move.toY][move.toX] = Piece(move.capturedPieceType, move.capturedColor, 0);

    if (move.isEnPassant) {
        Colour enemyColor = (currentPlayer == WHITE) ? BLACK : WHITE;
        grid[move.toY - direction][move.toX] = Piece(PAWN, enemyColor, 1);
        grid[move.toY][move.toX] = Piece(EMPTY, NONE, 0);
    }

    if (move.isCastling) {
        if (move.toX == 6) { 
            Piece rook = grid[move.toY][5];
            grid[move.toY][7] = rook;
            grid[move.toY][5] = Piece(EMPTY, NONE, 0);
        } 
        else if (move.toX == 2) { 
            Piece rook = grid[move.toY][3];
            grid[move.toY][0] = rook;
            grid[move.toY][3] = Piece(EMPTY, NONE, 0);
        }
    }

    hasKingMovedWhite = move.oldWhiteKingMoved;
    hasKingMovedBlack = move.oldBlackKingMoved;
    hasKingsideRookMovedWhite = move.oldWhiteKingsideRookMoved;
    hasQueensideRookMovedWhite = move.oldWhiteQueensideRookMoved;
    hasKingsideRookMovedBlack = move.oldBlackKingsideRookMoved;
    hasQueensideRookMovedBlack = move.oldBlackQueensideRookMoved;
    this->setEnPassantSquare(move.oldEnPassantSquare);

}