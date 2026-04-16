#include <gtest/gtest.h>
#include <algorithm>

#include "../src/engine/MoveGenerator.hpp"
#include "../src/state/Board.hpp"
#include "../src/state/Move.hpp"

bool containsMove(const std::vector<Move>& moves, int fromX, int fromY, int toX, int toY) {
    Move target(fromX, fromY, toX, toY);
    return std::find(moves.begin(), moves.end(), target) != moves.end();
}

int countMoves(const std::vector<Move>& moves, int fromX, int fromY, int toX, int toY) {
    Move target(fromX, fromY, toX, toY);
    return std::count(moves.begin(), moves.end(), target);
}

// MOVE COUNTS & BASIC GENERATION
TEST(MoveGenTest, InitialPosition) {
    
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    auto moves = MoveGenerator::generateAllLegalMoves(board);

    EXPECT_EQ(moves.size(), 20);

}

// SPECIAL PAWN MOVES
TEST(MoveGenTest, EnPassantGeneration) {
    
    Board board("k7/8/8/3pP3/8/8/8/K7 w - d6 0 2");
    
    auto moves = MoveGenerator::generateAllLegalMoves(board);
    
    EXPECT_TRUE(containsMove(moves, 4, 3, 3, 2));

}

TEST(MoveGenTest, PromotionGeneration) {
    
    Board board("7k/P7/8/8/8/8/8/K7 w - - 0 1");
    
    auto moves = MoveGenerator::generateAllLegalMoves(board);
    
    EXPECT_EQ(countMoves(moves, 0, 1, 0, 0), 4);

}

// CASTLING
TEST(MoveGenTest, CastlingGeneration) {
    
    Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    
    auto moves = MoveGenerator::generateAllLegalMoves(board);
    
    EXPECT_TRUE(containsMove(moves, 4, 7, 6, 7));
    
    EXPECT_TRUE(containsMove(moves, 4, 7, 2, 7));

}

TEST(MoveGenTest, CannotCastleThroughCheck) {
    
    Board board("3r4/8/8/8/8/8/8/R3K3 w Q - 0 1");
    
    auto moves = MoveGenerator::generateAllLegalMoves(board);
    
    EXPECT_FALSE(containsMove(moves, 4, 7, 2, 7));

}

// PINS & KING SAFETY
TEST(MoveGenTest, PinnedPieceRestrictions) {
    
    Board board("4r3/8/8/8/8/4N3/8/4K3 w - - 0 1");
    
    auto moves = MoveGenerator::generateAllLegalMoves(board);
    
    EXPECT_FALSE(containsMove(moves, 4, 5, 3, 3)); 
    EXPECT_FALSE(containsMove(moves, 4, 5, 5, 3)); 
    EXPECT_FALSE(containsMove(moves, 4, 5, 2, 4)); 

}

TEST(MoveGenTest, MustResolveCheck) {
    
    Board board("8/8/8/8/7q/8/6P1/4K3 w - - 0 1");
    
    auto moves = MoveGenerator::generateAllLegalMoves(board);
    
    EXPECT_TRUE(containsMove(moves, 6, 6, 6, 5));
    EXPECT_TRUE(containsMove(moves, 4, 7, 3, 7));
    EXPECT_FALSE(containsMove(moves, 6, 6, 6, 4));

}

// ENDGAME STATES
TEST(MoveGenTest, CheckmateYieldsZeroMoves) {
    
    Board board("rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3");
    
    auto moves = MoveGenerator::generateAllLegalMoves(board);
    
    EXPECT_EQ(moves.size(), 0);

}

TEST(MoveGenTest, StalemateYieldsZeroMoves) {
    
    Board board("7k/5Q2/8/8/8/8/8/7K b - - 0 1");
    
    auto moves = MoveGenerator::generateAllLegalMoves(board);
    
    EXPECT_EQ(moves.size(), 0);

}