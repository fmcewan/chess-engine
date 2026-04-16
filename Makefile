# FLAGS
CXX = g++
CXX_FLAGS = -std=c++17 -g -Wall -Isrc/state -Isrc/engine -Isrc/utilities $(shell pkg-config --cflags sdl2 SDL2_image)
LDF_LAGS = $(shell pkg-config --libs sdl2 SDL2_image)
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# Source files (excluding main)
SRCS = src/state/Board.cpp src/engine/MoveGenerator.cpp src/engine/MoveLegality.cpp src/utilities/FEN.cpp src/state/pieces/Piece.cpp
OBJS = $(SRCS:.cpp=.o)

# Test files
TEST_SRCS = tests/test_board.cpp tests/test_move_generation.cpp tests/test_move_legality.cpp tests/test_perft.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Main target for the actual game
chess: src/main.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o chess $^ -lSDL2 -lSDL2_image

# Test target
test: $(TEST_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o run_tests $^ $(GTEST_FLAGS)
	./run_tests

# Generic rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/engine/*.o src/state/pieces/*.o src/state/*.o src/ui/*.o src/utilities/*.o tests/*.o run_tests

