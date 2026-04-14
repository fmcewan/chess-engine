#pragma once

#include <string>
#include <vector>
#include <utility>

#include "../state/pieces/Piece.hpp"

class Board;

struct FENData {
    std::string piecePlacement;
    Colour currentPlayer;
    std::string castlingRights;
    std::pair<int, int> enPassantSquare;
    int halfmoveClock;
    int fullmoveNumber;
};

class FEN {
    
    public:
        static FENData parse(const std::string& fenString);
        static std::string generate(Board& board);

};