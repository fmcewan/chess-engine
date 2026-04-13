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
#include "MoveGenerator.hpp"
#include "Square.hpp"
#include "../pieces/Piece.hpp"
#include "../pieces/Rook.hpp"
#include "../pieces/Pawn.hpp"
#include "../pieces/Bishop.hpp"
#include "../pieces/Knight.hpp"
#include "../pieces/King.hpp"
#include "../pieces/Queen.hpp"

// Constructors
Board::Board(): height(8), width(8), grid(std::vector<std::vector<Square>>(height, std::vector<Square>(width, Square(Piece(EMPTY, NONE, 0), 0, 0)))) {
    initialiseBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

Board::Board(std::string FEN): height(8), width(8),  grid(std::vector<std::vector<Square>>(height, std::vector<Square>(width, Square(Piece(EMPTY, NONE, 0), 0, 0)))) {
    initialiseBoard(FEN);
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

Square Board::getSquare(int x, int y) {
    return grid[y][x];
}

Color Board::getCurrentPlayer() {
    return currentPlayer;
}

std::pair<int, int> Board::getEnPassantSquare() {
    return enPassantSquare;
}

std::vector< std::vector<Square> > Board::getGrid() {
    return grid;
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
    Square setSquare = Square(setPiece, setX, setY);
    grid[setY][setX] = setSquare;
}

void Board::setCurrentPlayer(Color setCurrentPlayer) {
    currentPlayer = setCurrentPlayer;
}

void Board::setEnPassantSquare(std::pair<int, int> setEnPassantSquare) {
    enPassantSquare = setEnPassantSquare;
}

// Board methods

void Board::initialiseBoard(std::string FEN) {

    // Helper initialisations
    std::map<char, int> charLocationToIntLocation {
        {'a', 0},
        {'b', 1},
        {'c', 2},
        {'d', 3},
        {'e', 4},
        {'f', 5},
        {'g', 6},
        {'h', 7},
    };

    std::vector<char> numbers{'1', '2', '3', '4', '5', '6', '7', '8'};

    /* FEN string processing */

    // Removes slashes for convenience
    FEN.erase(std::remove(FEN.begin(), FEN.end(), '/'), FEN.end());

    // Splits up FEN string into its separate parts
    std::stringstream ss(FEN); 
    std::vector<std::string> FENcomponents;
    std::string s; 
    while (std::getline(ss, s, ' ')) { 
        FENcomponents.push_back(s); 
    }

    /* Board allocation */

    // Allocating pieces to board from main FEN string

    int y=0;
    int x=0;

    for (auto character : FENcomponents[0]) {

        // If character is a number, we skip that many boxes
        if (std::find(numbers.begin(), numbers.end(), character) != numbers.end()) {
           switch (character) {
                case '1':
                    x+=1;
                    break; 
                case '2':
                    x+=2;
                    break;
                case '3':
                    x+=3;
                    break;
                case '4':
                    x+=4;
                    break;
                case '5':
                    x+=5;
                    break;
                case '6':
                    x+=6;
                    break;
                case '7':
                    x+=7;
                    break;
                case '8':
                    x+=8;
                    break;
                default:
                    break;
            } 

            // Increments line if j greater than 7 as a result of switch addition
            if (x>7) {
                y++;
                x-=8;
            } 

        }  
        // Otherwise we add the corresponding piece to that space
        else {

            switch (character) {
                case 'p': {
                    setSquare(Pawn(BLACK), x, y);
                    break;
                }
                case 'P': {
                    setSquare(Pawn(WHITE), x, y);
                    break;
                }
                case 'r': {
                    setSquare(Rook(BLACK, false), x, y);
                    break;
                }
                case 'R': {
                    setSquare(Rook(WHITE, false), x, y);
                    break;
                }
                case 'n': {
                    setSquare(Knight(BLACK), x, y);
                    break;
                }
                case 'N': {
                    setSquare(Knight(WHITE), x, y);
                    break;
                }
                case 'b': {
                    setSquare(Bishop(BLACK), x, y);
                    break;
                }
                case 'B': {
                    setSquare(Bishop(WHITE), x, y);
                    break;
                }
                case 'q': {
                    setSquare(Queen(BLACK), x, y);
                    break;
                }
                case 'Q': {
                    setSquare(Queen(WHITE), x, y);
                    break;
                }
                case 'k': {
                    setSquare(King(BLACK), x, y);
                    break;
                }
                case 'K': {
                    setSquare(King(WHITE), x, y);
                    break;
                }
                default:
                    break;
            }

            // Increments space (or line if new line required)
            if (x<7) {
                x++;
            }
        
            else {
                x=0;
                y++;
            }
        
        } 
    }

    // Starting player

    if (FENcomponents[1] == "w") {
        setCurrentPlayer(WHITE);
    }
    else {
        setCurrentPlayer(BLACK);
    }

    // Castling availability

    // TODO: Change to work with new Castling logic home (Board class instead of Piece class)

    if (FENcomponents[2] != "-") {

        Piece piece = grid[7][7].getPiece();
        
        if (FENcomponents[2].find("Q") && piece.getType() == ROOK && piece.getColor() == WHITE) {
		    auto rook = static_cast<Rook*>(&piece);
        	rook->setCanCastle(true);
    	}        

        piece = grid[7][0].getPiece();

        if (FENcomponents[2].find("Q") && piece.getType() == ROOK && piece.getColor() == WHITE) {
            auto rook = static_cast<Rook*>(&piece);
            rook->setCanCastle(true);
        }

        piece = grid[0][7].getPiece();

        if (FENcomponents[2].find("k") && piece.getType() == ROOK && piece.getColor() == BLACK) {
            auto rook = static_cast<Rook*>(&piece);
            rook->setCanCastle(true);
        }  

        piece = grid[0][0].getPiece();

         if (FENcomponents[2].find("q") && piece.getType() == ROOK && piece.getColor() == BLACK) {
            auto rook = static_cast<Rook*>(&piece);
            rook->setCanCastle(true);
        }

   }
    
    // En passant location

    if (FENcomponents[3] != "-") {
        std::string enPassantLocation = FENcomponents[3];
        
        int X = charLocationToIntLocation[enPassantLocation[0]];
        int Y = enPassantLocation[1] - '0';

        setEnPassantSquare(std::make_pair(X, Y));
    }
    else {
        setEnPassantSquare(std::make_pair(-1, -1));
    }

    // Halfmove clock

    const char* halfmoveClock = FENcomponents[4].c_str();

    int setHalfmoves = std::atoi(halfmoveClock);

    this->setHalfmoves(setHalfmoves);

    // Fullmove number

    const char* fullmoveNumber = FENcomponents[5].c_str();

    int setFullmoves = std::atoi(fullmoveNumber);

    this->setFullmoves(setFullmoves);

}

void Board::printBoard() {
 
    for (int i=0; i<this->getHeight(); i++) {
        for (int j=0; j<this->getWidth(); j++) {

            Piece piece = grid[i][j].getPiece();

            switch (piece.getType()) {
                case PAWN: {
                    if (piece.getColor() == BLACK) {
                        std::cout << "p";
                    }
                    else {
                        std::cout << "P";
                    }
                    break;
                }

                case ROOK: {
                    if (piece.getColor() == BLACK) {
                        std::cout << "r";
                    }
                    else {
                        std::cout << "R";
                    }
                    break;
                }
                case BISHOP: {
                    if (piece.getColor() == BLACK) {
                        std::cout << "b";
                    }
                    else {
                        std::cout << "B";
                    }
                    break;
                }
                case KNIGHT: {
                    if (piece.getColor() == BLACK) {
                        std::cout << "n";
                    }
                    else {
                        std::cout << "N";
                    }
                    break;
                }
                case KING: {
                    if (piece.getColor() == BLACK) {
                        std::cout << "k";
                    }
                    else {
                        std::cout << "K";
                    }
                    break;
                }
                case QUEEN: {
                    if (piece.getColor() == BLACK) {
                        std::cout << "q";
                    }
                    else {
                        std::cout << "Q";
                    }
                    break;
                }
                case EMPTY:
                    std::cout << "E";
                    break;
                default:
                    break;
            }
        }
        std::cout << std::endl;
    }
}

/* Castling methods */
bool Board::hasKingMoved(Color color) {
    return (color == WHITE) ? hasKingMovedWhite : hasKingMovedBlack; 
}

bool Board::canCastleKingside(Color color) {
    
    int y = (color == WHITE) ? 7 : 0;
    
    if (color == WHITE) {
        if (hasKingMovedWhite || hasKingsideRookMovedWhite) return false;
    } else {
        if (hasKingMovedBlack|| hasKingsideRookMovedBlack) return false;
    }

    if (grid[y][5].getPiece().getType() != EMPTY || 
        grid[y][6].getPiece().getType() != EMPTY) {
        return false;
    }
    
    Color enemyColor = (color == WHITE) ? BLACK : WHITE;
    if (MoveGenerator::isSquareAttacked(4, y, enemyColor, *this) ||
        MoveGenerator::isSquareAttacked(5, y, enemyColor, *this) ||
        MoveGenerator::isSquareAttacked(6, y, enemyColor, *this)) {
        return false;
    }

    return true;
}

bool Board::canCastleQueenside(Color color) {

    int y = (color == WHITE) ? 7 : 0;
    
    if (color == WHITE) {
        if (hasKingMovedWhite || hasQueensideRookMovedWhite) return false;
    } else {
        if (hasKingMovedBlack|| hasQueensideRookMovedBlack) return false;
    }

    if (grid[y][1].getPiece().getType() != EMPTY || 
        grid[y][2].getPiece().getType() != EMPTY || 
        grid[y][3].getPiece().getType() != EMPTY) {
        return false;
    }
    
    Color enemyColor = (color == WHITE) ? BLACK : WHITE;
    if (MoveGenerator::isSquareAttacked(4, y, enemyColor, *this) ||
        MoveGenerator::isSquareAttacked(3, y, enemyColor, *this) ||
        MoveGenerator::isSquareAttacked(2, y, enemyColor, *this)) {
        return false;
    }

    return true;
}

/* Move methods */
bool Board::executeMove(Move move) {

    move.capturedPieceType = grid[move.toY][move.toX].getPiece().getType();
    move.capturedColor = grid[move.toY][move.toX].getPiece().getColor();

    move.oldWhiteKingMoved = hasKingMovedWhite;
    move.oldBlackKingMoved = hasKingMovedBlack;
    move.oldWhiteKingsideRookMoved = hasKingsideRookMovedWhite;
    move.oldWhiteQueensideRookMoved = hasQueensideRookMovedWhite;
    move.oldBlackKingsideRookMoved = hasKingsideRookMovedBlack;
    move.oldBlackQueensideRookMoved = hasQueensideRookMovedBlack;

    Piece movingPiece = grid[move.fromY][move.fromX].getPiece();

    // Castling
    if (move.isCastling) {
        int rookFromX = (move.toX == 6) ? 7 : 0; // Kingside (7) or Queenside (0)
        int rookToX = (move.toX == 6) ? 5 : 3;   // Kingside (5) or Queenside (3)
        
        // Teleport the Rook
        grid[move.toY][rookToX].setPiece(grid[move.toY][rookFromX].getPiece());
        grid[move.toY][rookFromX].setPiece(Piece(EMPTY, NONE, 0));
    }

    // En Passant
    if (move.isEnPassant) {
        int victimY = move.fromY; // The pawn is on the same row as the start
        grid[victimY][move.toX].setPiece(Piece(EMPTY, NONE, 0));
    }

    if (move.isPromotion) {
        grid[move.toY][move.toX].setPiece(Piece(move.promotionPiece, movingPiece.getColor(), 0));
    } else {
        grid[move.toY][move.toX].setPiece(movingPiece);
    }
    grid[move.fromY][move.fromX].setPiece(Piece(EMPTY, NONE, 0));

    if (movingPiece.getType() == KING) {
        if (movingPiece.getColor() == WHITE) hasKingMovedWhite= true;
        else hasKingMovedBlack = true;
    }

    if (movingPiece.getType() == ROOK) {
        if (movingPiece.getColor() == WHITE) {
            if (move.fromX == 7 && move.fromY == 7) hasKingsideRookMovedWhite = true;
            if (move.fromX == 0 && move.fromY == 7) hasQueensideRookMovedWhite = true;
        } else {
            if (move.fromX == 7 && move.fromY == 0) hasKingsideRookMovedBlack= true;
            if (move.fromX == 0 && move.fromY == 0) hasQueensideRookMovedBlack = true;
        }
    }

    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;

    return true;

}

bool Board::undoMove(Move move) {

    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
    
    // 2. IDENTIFY THE PIECE THAT MOVED
    Piece movingPiece = grid[move.toY][move.toX].getPiece();

    // 3. REVERSE SPECIAL MOVES
    if (move.isCastling) {
        int rookFromX = (move.toX == 6) ? 7 : 0; 
        int rookToX = (move.toX == 6) ? 5 : 3;   
        
        grid[move.toY][rookFromX].setPiece(grid[move.toY][rookToX].getPiece());
        grid[move.toY][rookToX].setPiece(Piece(EMPTY, NONE, 0));
    }

    if (move.isEnPassant) {
        grid[move.fromY][move.toX].setPiece(Piece(PAWN, currentPlayer, 0)); 
    }

    // 4. MOVE THE PIECE BACK ON THE GRID
    if (move.isPromotion) {
        grid[move.fromY][move.fromX].setPiece(Piece(PAWN, movingPiece.getColor(), 0));
    } else {
        grid[move.fromY][move.fromX].setPiece(movingPiece);
    }

    // 5. RESTORE THE DESTINATION SQUARE
    if (move.isEnPassant) {
        grid[move.toY][move.toX].setPiece(Piece(EMPTY, NONE, 0));
    } else {
        grid[move.toY][move.toX].setPiece(Piece(move.capturedPieceType, move.capturedColor, 0));
    }

    // 6. RESTORE HISTORICAL FLAGS
    hasKingMovedWhite = move.oldWhiteKingMoved;
    hasKingMovedBlack = move.oldBlackKingMoved;
    hasKingsideRookMovedWhite = move.oldWhiteKingsideRookMoved;
    hasQueensideRookMovedWhite = move.oldWhiteQueensideRookMoved;
    hasKingsideRookMovedBlack = move.oldBlackKingsideRookMoved;
    hasQueensideRookMovedBlack = move.oldBlackQueensideRookMoved;

    return true;

}
 
bool Board::makeMove(Move move) {

    // Move information
    int initialX = move.fromX;
    int initialY = move.fromY;
    int finalX = move.toX;
    int finalY = move.toY;
    
    Piece initialPiece = grid[initialY][initialX].getPiece();
    Piece emptyPiece = Piece(EMPTY, NONE, 0);
    
    int direction = (initialPiece.getColor() == BLACK) ? 1 : -1;
    Square initialSquare = grid[initialY][initialX];

    // King History Logic
    if (initialPiece.getType() == KING) {
        if (initialPiece.getColor() == WHITE) {
            hasKingMovedWhite = true;
        } 
        else {
            hasKingMovedBlack = true;
        }
    }

    // Rook History Logic (Necessary for castling)
    if (initialPiece.getType() == ROOK) {
        if (initialPiece.getColor() == WHITE) {
            if (move.fromX == 7 && move.fromY == 7) hasKingsideRookMovedWhite = true;
            if (move.fromX == 0 && move.fromY == 7) hasQueensideRookMovedWhite = true;
        } 
        else {
            if (move.fromX == 7 && move.fromY == 0) hasKingsideRookMovedBlack = true;
            if (move.fromX == 0 && move.fromY == 0) hasQueensideRookMovedBlack = true;
        }
    }    
        
    // Regular taking
    if (MoveGenerator::isLegal(move, *this)) {
         
        // En Passant removal 
        if (move.isEnPassant) { 
            grid[move.toY-direction][move.toX].setPiece(Piece(EMPTY, NONE, 0));
        }

        // Castling
        if (move.isCastling) {
            if (finalX == 6) { // Kingside
                Piece rook = grid[finalY][7].getPiece();
                grid[finalY][5].setPiece(rook);
                grid[finalY][7].setPiece(emptyPiece);
            } 
            else if (finalX == 2) { // Queenside
                Piece rook = grid[finalY][0].getPiece();
                grid[finalY][3].setPiece(rook);
                grid[finalY][0].setPiece(emptyPiece);
            }
        }

        // Pawn promotion and piece placement
        if (move.isPromotion) {
            grid[finalY][finalX].setPiece(Piece(move.promotionPiece, initialPiece.getColor(), 9));
        } 
        else {
            grid[finalY][finalX].setPiece(initialPiece);
        }
         
        // Removing previous piece 
        grid[initialY][initialX].setPiece(emptyPiece);
        
        // En Passant target square update 
        if (initialPiece.getType() == PAWN && std::abs(move.toY - move.fromY) == 2) {
            this->setEnPassantSquare({move.fromX, move.fromY + direction});
        } 
        else {
            this->setEnPassantSquare({-1, -1});
        }    

        Color newPlayer = (getCurrentPlayer() == BLACK) ? WHITE : BLACK;
        setCurrentPlayer(newPlayer);

        return true;  
        
       
    }
     
    return false;

}
