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

TEST(PerftTest, Position3_Endgame) {
    
    Board board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    
    EXPECT_EQ(perft(board, 1), 14ULL);
    EXPECT_EQ(perft(board, 2), 191ULL);
    EXPECT_EQ(perft(board, 3), 2812ULL);
    EXPECT_EQ(perft(board, 4), 43238ULL);
    EXPECT_EQ(perft(board, 5), 674624ULL);
    // EXPECT_EQ(perft(board, 6), 11030083ULL);

}

TEST(PerftTest, Position4_Asymmetric) {
    
    Board board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    
    EXPECT_EQ(perft(board, 1), 6ULL);
    EXPECT_EQ(perft(board, 2), 264ULL);
    EXPECT_EQ(perft(board, 3), 9467ULL);
    EXPECT_EQ(perft(board, 4), 422333ULL);
    // EXPECT_EQ(perft(board, 5), 15833292ULL);

}

// Position 4b: Mirrored Position 4
TEST(PerftTest, Position4b_Mirrored) {
    
    Board board("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    
    EXPECT_EQ(perft(board, 1), 6ULL);
    EXPECT_EQ(perft(board, 2), 264ULL);
    EXPECT_EQ(perft(board, 3), 9467ULL);
    EXPECT_EQ(perft(board, 4), 422333ULL);
    // EXPECT_EQ(perft(board, 5), 15833292ULL);

}

TEST(PerftTest, Position5_ComplexMiddle) {
    
    Board board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    
    EXPECT_EQ(perft(board, 1), 44ULL);
    EXPECT_EQ(perft(board, 2), 1486ULL);
    EXPECT_EQ(perft(board, 3), 62379ULL);
    EXPECT_EQ(perft(board, 4), 2103487ULL);
    // EXPECT_EQ(perft(board, 5), 89941194ULL);

}

TEST(PerftTest, Position6_SymmetricDense) {
    
    Board board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    
    EXPECT_EQ(perft(board, 1), 46ULL);
    EXPECT_EQ(perft(board, 2), 2079ULL);
    EXPECT_EQ(perft(board, 3), 89890ULL);
    EXPECT_EQ(perft(board, 4), 3894594ULL);
    // EXPECT_EQ(perft(board, 5), 164075551ULL);

}