# FLAGS
CXX = g++
CXX_FLAGS = -std=c++17 -g -Wall -Isrc/state -Isrc/engine -Isrc/utilities $(shell pkg-config --cflags sdl2 SDL2_image)
LD_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf 
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# Source files (excluding main)
SRCS = src/state/Board.cpp src/engine/MoveGenerator.cpp src/engine/MoveLegality.cpp src/engine/Evaluator.cpp src/engine/Search.cpp src/utilities/FEN.cpp src/state/pieces/Piece.cpp src/app/View.cpp src/app/Controller.cpp
OBJS = $(SRCS:.cpp=.o)

# Test files
TEST_SRCS = src/state/Board.cpp src/engine/MoveGenerator.cpp src/engine/MoveLegality.cpp src/engine/Evaluator.cpp src/engine/Search.cpp src/utilities/FEN.cpp src/state/pieces/Piece.cpp tests/test_board.cpp tests/test_move_generation.cpp tests/test_move_legality.cpp tests/test_perft.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Main target for the actual game
chess: src/main.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o chess $^ $(LD_FLAGS)

# Test target
test: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o run_tests $^ $(GTEST_FLAGS)
	./run_tests

# Generic rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/engine/*.o src/state/pieces/*.o src/state/*.o src/app/*.o src/utilities/*.o tests/*.o src/*.o run_tests chess

