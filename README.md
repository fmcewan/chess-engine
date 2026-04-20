# C++ Chess Engine

A fully functional Chess Engine and graphical user interface written in C++. This project features a custom chess AI opponent, complete rule implementation, an interactive UI powered by SDL2, and a robust suite of engine testing tools.

## Features

### 🎮 Graphical User Interface (SDL2)
* **Main Menu:** Select between "Player vs Player" and "Player vs Engine" modes.
* **Interactive Board:** Drag-and-drop piece movement with visual highlighting for selected pieces.
* **Legal Move Hints:** Clicking a piece displays all valid squares it can move to.
* **Pawn Promotion Menu:** A graphical overlay allowing players to choose their promotion piece (Queen, Rook, Bishop, or Knight).
* **Game Over Screen:** Detects Checkmate, Stalemate, and Timeouts (Flagging), offering an option to restart or quit.
* **Chess Clocks:** Live countdown timers for both Black and White players.

### 🧠 Custom Engine
* **Minimax Search:** Explores future game states to determine the optimal move.
* **Alpha-Beta Pruning:** Optimises the search tree to look deeper into positions efficiently.
* **Move Ordering:** Evaluates captures and promotions first to dramatically speed up pruning and overall performance.
* **Positional Evaluation:** Utilises standard centipawn values (e.g., Queen = 900, Pawn = 100) and Piece-Square Tables (PSTs) to give the engine an understanding of positional play (e.g., controlling the center, keeping the King safe).

### ⚙️ Utilities
* **FEN Parsing:** Includes a Forsyth-Edwards Notation (FEN) utility (`src/utilities/FEN.cpp`) allowing the board to be initialized from any standard chess position.
* **Complete Move Legality:** Accurate move generation handling complex rules like absolute pins, checks, and pawn promotions.

## Project Structure

* `src/app/`: Contains the visual interface (`View`) and the game loop/state machine (`Controller`).
* `src/engine/`: The brains of the engine (`Search`, `Evaluator`) and rule processors (`MoveGenerator`, `MoveLegality`).
* `src/state/`: Core game representations (`Board`, `Move`, and individual `Piece` classes).
* `src/utilities/`: Helper tools, specifically FEN string loading and parsing.
* `tests/`: A robust testing suite ensuring engine accuracy:
  * `test_board.cpp`: Validates board state representation.
  * `test_move_generation.cpp` & `test_move_legality.cpp`: Ensures pieces move correctly and obey complex rules.
  * `test_perft.cpp`: Performance tests that count all possible nodes at a given depth to guarantee the engine has zero move-generation bugs.
* `assets/`: Images, piece sprites and fonts used by the SDL2 interface.

## Prerequisites

To compile and run this project, you will need a C++ compiler (e.g., `g++` or `clang++`) and the SDL2 development libraries.

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

**MacOS:**
```bash
brew install sdl2 sdl2_image sdl2_ttf
```

## Building and Running

A 'Makefile' is included for easy compilation.

1. Clone the repository and navigate to the project directory.
2. Build the engine and GUI the following command:

```bash
make
```

3. Run the game 

```bash
./chess
```

### Future Improvements
* **Bitboards:** Refactoring the underlying board state to use 64-bit integers for much faster move generation. 
* **Quiescence Search:** Preventing the "horizon effect" by continuing the search until a quiet position (no captures available) is reached.
* **Transposition Tables:** Implementing Zobrist hashing to cache and reuse previously evaluated board states.
* **MCTS Agent:** Implementation of an MCTS agent, with the option of picking between it and the minimax agent.

### License 
This project is open-source. Please see the LICENSE file for more details. 
