#include <gtest/gtest.h>

#include "../src/engine/MoveLegality.hpp"
#include "../src/state/Board.hpp"
#include "../src/state/Move.hpp"

// PAWN MOVES
TEST(PawnLegalityTest, StandardPawnPushes) {
    // Initial position
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    // e2 to e4 (Double push from start) - White
    Move e2e4(4, 6, 4, 4);
    EXPECT_TRUE(MoveLegality::isLegal(e2e4, board));

    // e2 to e3 (Single push from start) - White
    Move e2e3(4, 6, 4, 5);
    EXPECT_TRUE(MoveLegality::isLegal(e2e3, board));

    // Invalid: e2 to e5 (Triple push)
    Move e2e5(4, 6, 4, 3);
    EXPECT_FALSE(MoveLegality::isLegal(e2e5, board));
}

TEST(PawnLegalityTest, PawnBlocked) {
    
    Board board("8/8/8/8/8/4p3/4P3/8 w - - 0 1");
    
    Move e2e3(4, 6, 4, 5);
    EXPECT_FALSE(MoveLegality::isLegal(e2e3, board));

    Move e2e4(4, 6, 4, 4);
    EXPECT_FALSE(MoveLegality::isLegal(e2e4, board));

}

TEST(PawnLegalityTest, PawnCaptures) {
    
    Board board("8/8/8/2p1p3/3P4/8/8/8 w - - 0 1");
    
    Move dxc5(3, 4, 2, 3);
    EXPECT_TRUE(MoveLegality::isLegal(dxc5, board));

    Move dxe5(3, 4, 4, 3);
    EXPECT_TRUE(MoveLegality::isLegal(dxe5, board));

    Board board_blocked("8/8/8/3p4/3P4/8/8/8 w - - 0 1");
    Move d4xd5(3, 4, 3, 3);
    EXPECT_FALSE(MoveLegality::isLegal(d4xd5, board_blocked));

}

// KNIGHT MOVES
TEST(KnightLegalityTest, ValidAndInvalidJumps) {
    
    Board board("8/8/8/8/3N4/8/8/8 w - - 0 1");

    Move d4e6(3, 4, 4, 2);
    EXPECT_TRUE(MoveLegality::isLegal(d4e6, board));

    Move d4c2(3, 4, 2, 6);
    EXPECT_TRUE(MoveLegality::isLegal(d4c2, board));

    Move d4d5(3, 4, 3, 3);
    EXPECT_FALSE(MoveLegality::isLegal(d4d5, board));

}

TEST(KnightLegalityTest, FriendlyFire) {
    
    Board board("8/8/4P3/8/3N4/8/8/8 w - - 0 1");

    Move d4e6(3, 4, 4, 2);
    EXPECT_FALSE(MoveLegality::isLegal(d4e6, board));

}

// SLIDING PIECES (BISHOP, ROOK, QUEEN)
TEST(SlidingPieceLegalityTest, PathBlocking) {
    
    Board board("k7/8/8/8/8/8/P7/R7 w - - 0 1");

    Move a1a3(0, 7, 0, 5);
    EXPECT_FALSE(MoveLegality::isLegal(a1a3, board));

    Move a1a8(0, 7, 0, 0);
    EXPECT_FALSE(MoveLegality::isLegal(a1a8, board));

}

TEST(SlidingPieceLegalityTest, QueenMovement) {
    
    Board board("8/8/8/8/3Q4/8/8/8 w - - 0 1");

    Move d4h8(3, 4, 7, 0);
    EXPECT_TRUE(MoveLegality::isLegal(d4h8, board));

    Move d4d8(3, 4, 3, 0);
    EXPECT_TRUE(MoveLegality::isLegal(d4d8, board));

    Move d4e6(3, 4, 4, 2);
    EXPECT_FALSE(MoveLegality::isLegal(d4e6, board));

}

// KING MOVES
TEST(KingLegalityTest, ValidSteps) {
    
    Board board("8/8/8/8/4K3/8/8/8 w - - 0 1");

    Move e4e5(4, 4, 4, 3);
    EXPECT_TRUE(MoveLegality::isLegal(e4e5, board));

    Move e4f5(4, 4, 5, 3);
    EXPECT_TRUE(MoveLegality::isLegal(e4f5, board));

    Move e4e6(4, 4, 4, 2);
    EXPECT_FALSE(MoveLegality::isLegal(e4e6, board));

}

// CHECK AND PINS (KING SAFETY)
TEST(CheckLegalityTest, CannotMoveIntoCheck) {
    
    Board board("3r4/8/8/8/8/8/8/4K3 w - - 0 1");

    Move e1d1(4, 7, 3, 7);
    EXPECT_FALSE(MoveLegality::isLegal(e1d1, board));

    Move e1d2(4, 7, 3, 6);
    EXPECT_FALSE(MoveLegality::isLegal(e1d2, board));

    Move e1f1(4, 7, 5, 7);
    EXPECT_TRUE(MoveLegality::isLegal(e1f1, board));

}

TEST(CheckLegalityTest, AbsolutePin) {
    
    Board board("4r3/8/8/8/8/8/4R3/4K3 w - - 0 1");

    Move e2d2(4, 6, 3, 6);
    EXPECT_FALSE(MoveLegality::isLegal(e2d2, board));

    Move e2e3(4, 6, 4, 5);
    EXPECT_TRUE(MoveLegality::isLegal(e2e3, board));

    Move e2e8(4, 6, 4, 0);
    EXPECT_TRUE(MoveLegality::isLegal(e2e8, board));

}

TEST(CheckLegalityTest, ResolvingCheck) {
    
    Board board("4r3/8/8/8/8/8/8/4K1N1 w - - 0 1");

    Move g1e2(6, 7, 4, 6);
    EXPECT_TRUE(MoveLegality::isLegal(g1e2, board));

    Move g1f3(6, 7, 5, 5);
    EXPECT_FALSE(MoveLegality::isLegal(g1f3, board));

}