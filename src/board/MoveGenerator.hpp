#pragma once 

// Header includes
#include "Board.hpp"
#include "Move.hpp"

class MoveGenerator {
    
    public:

        static std::vector<Move> generateAllLegalMoves(Board& board);

        static bool isLegal(const Move& move, Board& board);
        static bool isPseudoLegal(const Move& move, Board& board);
        
        static bool isSquareAttacked(int targetX, int targetY, Color attackerColor, Board& board);
        static bool isInCheck(Color color, Board& board);
        static bool isInCheckmate(Color color, Board& board);

    private:

        static bool isPathClear(int x1, int x2, int y1, int y2, Board& board);

        static bool isLegalMovePawn(const Move& move, Board& board);
        static bool isLegalMoveKnight(const Move& move, Board& board);
        static bool isLegalMoveRook(const Move& move, Board& board);
        static bool isLegalMoveQueen(const Move& move, Board& board);
        static bool isLegalMoveKing(const Move& move, Board& board);
        static bool isLegalMoveBishop(const Move& move, Board& board);

        static void addPawnMoves(int x, int y, Board& board, std::vector<Move>& moves);
        static void addPromotionMoves(int fx, int fy, int tx, int ty, Board& board, std::vector<Move>& moves);
        static void addKnightMoves(int x, int y, Board& board, std::vector<Move>& moves);
        static void addSlidingMoves(int x, int y, Board& board, std::vector<Move>& moves);
        static void addKingMoves(int x, int y, Board& board, std::vector<Move>& moves);

};


