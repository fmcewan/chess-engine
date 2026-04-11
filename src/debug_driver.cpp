#include <iostream>
#include "board/Board.hpp"
#include "board/Move.hpp"

int main() {
    
    Board board("r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 1");

    std::cout << "--- Current Board State ---" << std::endl;
    board.printBoard();

    Move testMove(board.getBoard(), board.getCurrentPlayer(), 5, 5, 5, 1);

    std::cout << "\nTesting Move: Qf3 to f7" << std::endl;
    std::cout << "Is Legal: " << (testMove.isLegalMove() ? "YES" : "NO") << std::endl;

    if (board.makeMove(testMove)) {
        std::cout << "\n--- Board After Move ---" << std::endl;
        board.printBoard();
    } else {
        std::cout << "\nMove failed to execute!" << std::endl;
    }

    return 0;
}