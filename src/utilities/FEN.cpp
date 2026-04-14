#include <string>
#include <sstream>

#include "FEN.hpp"
#include "../state/Board.hpp"

FENData FEN::parse(const std::string& FEN) {

    FENData data;
    std::stringstream ss(FEN);
    std::vector<std::string> components;
    std::string s;

    while (std::getline(ss, s, ' ')) {
        components.push_back(s);
    }

    data.piecePlacement = components[0];
    data.currentPlayer = (components[1] == "w") ? WHITE : BLACK;
    data.castlingRights = components[2];

    if (components[3] != "-") {
        int x = components[3][0] - 'a';
        int y = '8' - components[3][1]; 
        data.enPassantSquare = {x, y};
    } 
    else {
        data.enPassantSquare = {-1, -1};
    }

    data.halfmoveClock = std::stoi(components[4]);
    data.fullmoveNumber = std::stoi(components[5]);

    return data;

}

std::string FEN::generate(Board& board) {
    
    std::stringstream fen;

    for (int y = 0; y < board.getHeight(); y++) {

        int emptySquares = 0;
        for (int x = 0; x < board.getWidth(); x++) {

            Piece piece = board.getPiece(x, y);
            if (piece.getType() == EMPTY) {
                emptySquares++;
            } 
            else {

                if (emptySquares > 0) {
                    fen << emptySquares;
                    emptySquares = 0;
                }
                
                char pieceChar;
                switch (piece.getType()) {
                    case PAWN:   pieceChar = 'p'; break;
                    case ROOK:   pieceChar = 'r'; break;
                    case KNIGHT: pieceChar = 'n'; break;
                    case BISHOP: pieceChar = 'b'; break;
                    case QUEEN:  pieceChar = 'q'; break;
                    case KING:   pieceChar = 'k'; break;
                    default:     pieceChar = '?'; break;
                }
                
                if (piece.getColour() == WHITE) fen << (char)std::toupper(pieceChar);
                else fen << pieceChar;
            
            }
        }

        if (emptySquares > 0) fen << emptySquares;
        if (y < board.getHeight() - 1) fen << "/";

    }

    fen << " " << (board.getCurrentPlayer() == WHITE ? "w" : "b");

    std::string castling = "";

    if (!board.hasKingMoved(WHITE)) {
        if (!board.getHasKingsideRookMoved(WHITE)) castling += "K";
        if (!board.getHasQueensideRookMoved(WHITE)) castling += "Q";
    }
    if (!board.hasKingMoved(BLACK)) {
        if (!board.getHasKingsideRookMoved(BLACK)) castling += "k";
        if (!board.getHasQueensideRookMoved(BLACK)) castling += "q";
    }

    fen << " " << (castling.empty() ? "-" : castling);

    auto enPassantSquare = board.getEnPassantSquare();

    if (enPassantSquare.first != -1) {
        char file = 'a' + enPassantSquare.first;
        char rank = '8' - enPassantSquare.second;
        fen << " " << file << rank;
    } 
    else {
        fen << " -";
    }

    fen << " " << board.getHalfmoves() << " " << board.getFullmoves();

    return fen.str();

}



