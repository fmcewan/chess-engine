#include <gtest/gtest.h>

#include "../src/state/Board.hpp"

#include "../src/engine/MoveGenerator.hpp"
#include "../src/engine/MoveLegality.hpp"

TEST(BoardTest, FENParsing) {
    
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    EXPECT_EQ(board.getCurrentPlayer(), WHITE);
    EXPECT_EQ(board.getHalfmoves(), 0);

}

TEST(BoardTest, MakeMove) {

    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move move(4, 6, 4, 4);

    board.makeMove(move);
    
    std::string FEN_after = board.getFEN();

    EXPECT_EQ(FEN_after, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

}

TEST(BoardTest, UndoMove) {

    Board board;
    Move move(4, 6, 4, 4);

    std::string FEN_before = board.getFEN();

    board.makeMove(move);

    std::string FEN_middle = board.getFEN();
    EXPECT_EQ(FEN_middle, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    board.undoMove(move);

    std::string FEN_after = board.getFEN();

    EXPECT_EQ(FEN_before, FEN_after);

}

TEST(MoveGenTest, InitialPosition) {

    Board board;

    auto moves = MoveGenerator::generateAllLegalMoves(board);

    EXPECT_EQ(moves.size(), 20);

}