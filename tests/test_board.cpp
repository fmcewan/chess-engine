#include <gtest/gtest.h>

#include "../src/state/Board.hpp"
#include "../src/state/Move.hpp"

// INITIALIZATION, FEN PARSING
TEST(BoardTest, DefaultConstructor) {
    Board board;
    
    EXPECT_EQ(board.getCurrentPlayer(), WHITE);
    EXPECT_EQ(board.getHalfmoves(), 0);
    EXPECT_EQ(board.getFullmoves(), 1);

    EXPECT_EQ(board.getPiece(4, 7).getType(), KING); // White King on e1
    EXPECT_EQ(board.getPiece(4, 7).getColour(), WHITE);
    
    EXPECT_EQ(board.getPiece(3, 0).getType(), QUEEN); // Black Queen on d8
    EXPECT_EQ(board.getPiece(3, 0).getColour(), BLACK);

}

TEST(BoardTest, FENParsing_ComplexState) {
    
    Board board("rnbq1bnr/ppppkppp/8/4p3/4P3/8/PPPPKPPP/RNBQ1BNR b - e3 5 12");
    
    EXPECT_EQ(board.getCurrentPlayer(), BLACK);
    EXPECT_EQ(board.getHalfmoves(), 5);
    EXPECT_EQ(board.getFullmoves(), 12);
    
    EXPECT_FALSE(board.canCastleKingside(WHITE));
    EXPECT_FALSE(board.canCastleQueenside(WHITE));
    EXPECT_FALSE(board.canCastleKingside(BLACK));
    EXPECT_FALSE(board.canCastleQueenside(BLACK));

    std::pair<int, int> enPassantSquare = board.getEnPassantSquare();
    EXPECT_EQ(enPassantSquare.first, 4);
    EXPECT_EQ(enPassantSquare.second, 5);

}

TEST(BoardTest, FENGenerationMatchesInput) {
    std::string inputFEN = "r1bqk2r/pppp1ppp/2n2n2/2b1p3/2B1P3/2N2N2/PPPP1PPP/R1BQK2R w KQkq - 6 5";
    Board board(inputFEN);
    
    EXPECT_EQ(board.getFEN(), inputFEN);

}

// MAKE MOVE
TEST(BoardTest, MakeMove_StandardCapture) {
    Board board("8/8/8/3p4/4P3/8/8/8 w - - 0 1");
    Move capture(4, 4, 3, 3); 
    
    board.makeMove(capture);
    
    EXPECT_EQ(board.getPiece(4, 4).getType(), EMPTY);
    
    EXPECT_EQ(board.getPiece(3, 3).getType(), PAWN);
    EXPECT_EQ(board.getPiece(3, 3).getColour(), WHITE);
    
    EXPECT_EQ(board.getCurrentPlayer(), BLACK);

}

TEST(BoardTest, MakeMove_CastlingUpdatesRook) {
    Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    Move castleKingside(4, 7, 6, 7); 
    castleKingside.isCastling = true;
    
    board.makeMove(castleKingside);
    
    EXPECT_EQ(board.getPiece(6, 7).getType(), KING);
    
    EXPECT_EQ(board.getPiece(5, 7).getType(), ROOK);
    EXPECT_EQ(board.getPiece(7, 7).getType(), EMPTY); 

}

TEST(BoardTest, MakeMove_EnPassantRemovesCapturedPawn) {

    Board board("8/8/8/3pP3/8/8/8/8 w - d6 0 1");
    Move enPassant(4, 3, 3, 2);
    enPassant.isEnPassant = true;
    
    board.makeMove(enPassant);
    
    EXPECT_EQ(board.getPiece(3, 2).getType(), PAWN);
    EXPECT_EQ(board.getPiece(3, 2).getColour(), WHITE);
    
    EXPECT_EQ(board.getPiece(3, 3).getType(), EMPTY);
}

TEST(BoardTest, MakeMove_PromotionChangesPieceType) {
    Board board("8/P7/8/8/8/8/8/8 w - - 0 1");
    Move promote(0, 1, 0, 0);
    promote.isPromotion = true;
    promote.promotionPiece = QUEEN;
    
    board.makeMove(promote);
    
    EXPECT_EQ(board.getPiece(0, 0).getType(), QUEEN);
    EXPECT_EQ(board.getPiece(0, 0).getColour(), WHITE);

}

// UNDO MOVE
TEST(BoardTest, UndoMove_RestoresCapturedPiece) {

    Board board("8/8/8/3p4/4P3/8/8/8 w - - 0 1");
    Move capture(4, 4, 3, 3);
    std::string fenBefore = board.getFEN();
    
    board.makeMove(capture);
    board.undoMove(capture);
    
    EXPECT_EQ(board.getFEN(), fenBefore);
    
    EXPECT_EQ(board.getPiece(3, 3).getType(), PAWN);
    EXPECT_EQ(board.getPiece(3, 3).getColour(), BLACK);

}

TEST(BoardTest, UndoMove_CastlingRestoresRookAndRights) {

    std::string initialFEN = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";
    Board board(initialFEN);
    Move castleKingside(4, 7, 6, 7);
    castleKingside.isCastling = true;
    
    board.makeMove(castleKingside);
    board.undoMove(castleKingside);
    
    EXPECT_EQ(board.getFEN(), initialFEN);
    EXPECT_TRUE(board.canCastleKingside(WHITE));

}

TEST(BoardTest, UndoMove_EnPassantRestoresCapturedPawn) {

    std::string initialFEN = "8/8/8/3pP3/8/8/8/8 w - d6 0 1";
    Board board(initialFEN);
    Move enPassant(4, 3, 3, 2);
    enPassant.isEnPassant = true;
    
    board.makeMove(enPassant);
    board.undoMove(enPassant);
    
    EXPECT_EQ(board.getFEN(), initialFEN);
}

TEST(BoardTest, UndoMove_PromotionRestoresPawn) {

    std::string initialFEN = "8/P7/8/8/8/8/8/8 w - - 0 1";
    Board board(initialFEN);
    Move promote(0, 1, 0, 0);
    promote.isPromotion = true;
    promote.promotionPiece = QUEEN;
    
    board.makeMove(promote);
    board.undoMove(promote);
    
    EXPECT_EQ(board.getFEN(), initialFEN);
    EXPECT_EQ(board.getPiece(0, 1).getType(), PAWN);

}

