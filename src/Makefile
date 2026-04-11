CXX = g++
CXXFLAGS = -std=c++17 -g -Wall $(shell pkg-config --cflags sdl2 SDL2_image)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image)

VPATH = game:board:pieces
OBJS = main.o Game.o Board.o Move.o Square.o Piece.o Pawn.o Rook.o Bishop.o Knight.o King.o Queen.o

all: Chess
Chess: $(OBJS)
	$(CXX) $(OBJS) -o Chess $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

DEBUG_OBJS = Board.o Move.o Square.o Piece.o Pawn.o Rook.o Bishop.o Knight.o King.o Queen.o

debug: $(DEBUG_OBJS) debug_driver.o
	$(CXX) $(DEBUG_OBJS) debug_driver.o -o Debugger $(LDFLAGS)

debug_driver.o: debug_driver.cpp
	$(CXX) $(CXXFLAGS) -c debug_driver.cpp

clean:
	rm -f *.o Chess Debugger 

