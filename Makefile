# FLAGS
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall $(shell pkg-config --cflags sdl2 SDL2_image)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image)

# PATH
VPATH = src/game:src/board:src/pieces:tests

# OBJECT VARIABLES
GUI_OBJS = main.o Game.o Board.o Move.o Square.o Piece.o Pawn.o Rook.o Bishop.o Knight.o King.o Queen.o
TEST_OBJS = Board.o Move.o Square.o Piece.o Pawn.o Rook.o Bishop.o Knight.o King.o Queen.o MoveGenerator.o

chess-engine: $(GUIOBJS)
	$(CXX) $(OBJS) -o chess $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_OBJS) test_board.o
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) test_board.o -o run_tests $(LDFLAGS) $(GTEST_FLAGS)
	./run_tests

test_board.o: tests/test_board.cpp
	$(CXX) $(CXXFLAGS) -Isrc -c tests/test_board.cpp -o test_board.o

clean:
	rm -f *.o chess

