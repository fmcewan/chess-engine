#include <gtest/gtest.h>

#include <cstdint>

#include "../src/state/Board.hpp"
#include "../src/engine/MoveGenerator.hpp"

// Helper function
int perft(Board& board, int depth) {
    
    if (depth == 0) {
        return 1;
    }

    int nodes = 0;
    auto moves = MoveGenerator::generateAllLegalMoves(board);

    for (Move& move : moves) {
        board.makeMove(move);
        nodes += perft(board, depth - 1);
        board.undoMove(move);
    }

    return nodes;

}

// PERFT TESTS
TEST(PerftTest, InitialPosition) {

    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    EXPECT_EQ(perft(board, 1), 20);
    EXPECT_EQ(perft(board, 2), 400);
    EXPECT_EQ(perft(board, 3), 8902); 
    EXPECT_EQ(perft(board, 4), 197281);

}

TEST(PerftTest, Kiwipete) {

    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    
    EXPECT_EQ(perft(board, 1), 48ULL);
    EXPECT_EQ(perft(board, 2), 2039ULL);
    EXPECT_EQ(perft(board, 3), 97862ULL);
    
}