# FLAGS
CXX = g++
CXX_FLAGS = -std=c++17 -g -Wall $(shell pkg-config --cflags sdl2 SDL2_image)
LDF_LAGS = $(shell pkg-config --libs sdl2 SDL2_image)
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# PATH
VPATH = src/ui:src/state:src/state/pieces:src/engine:src/utilities:tests

# OBJECT VARIABLES
GUI_OBJS = main.o Game.o Board.o Piece.o MoveGenerator.o 
TEST_OBJS = Board.o FEN.o Piece.o MoveGenerator.o MoveLegality.o 

chess-engine: $(GUI_OBJS)
	$(CXX) $(GUI_OBJS) -o chess $(LDFLAGS)

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

