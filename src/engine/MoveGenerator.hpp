#pragma once 

// Header includes
#include "../state/Board.hpp"
#include "../state/Move.hpp"

class MoveGenerator {
    
    public:

        static std::vector<Move> generateAllLegalMoves(Board& board);

    private:

        static void addPawnMoves(int x, int y, Board& board, std::vector<Move>& moves);
        static void addPromotionMoves(int fx, int fy, int tx, int ty, Board& board, std::vector<Move>& moves);
        static void addKnightMoves(int x, int y, Board& board, std::vector<Move>& moves);
        static void addSlidingMoves(int x, int y, Board& board, std::vector<Move>& moves);
        static void addKingMoves(int x, int y, Board& board, std::vector<Move>& moves);

};


