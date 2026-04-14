#pragma once

// Header includes
#include "../state/Board.hpp"
#include "../state/Move.hpp"

class MoveLegality {

    public:

        static bool isLegal(const Move& move, Board& board);
        static bool isPseudoLegal(const Move& move, Board& board);

        static bool isSquareAttacked(int targetX, int targetY, Colour attackerColor, Board& board);
        static bool isInCheck(Colour colour, Board& board);
        static bool isInCheckmate(Colour colour, Board& board);

    private:

        static bool isPathClear(int fromX, int toX, int fromY, int toY, Board& board);

        static bool isLegalMovePawn(const Move& move, Board& board);
        static bool isLegalMoveKnight(const Move& move, Board& board);
        static bool isLegalMoveRook(const Move& move, Board& board);
        static bool isLegalMoveQueen(const Move& move, Board& board);
        static bool isLegalMoveKing(const Move& move, Board& board);
        static bool isLegalMoveBishop(const Move& move, Board& board);
  
};