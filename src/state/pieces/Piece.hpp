#pragma once

enum PieceType {PAWN, ROOK, BISHOP, KNIGHT, QUEEN, KING, EMPTY};
enum Colour {WHITE, BLACK, NONE};

class Piece {

    private:
        PieceType type;
        Colour colour;
        int value;

    public:
        // Constructor
        Piece(PieceType piece_type, Colour color, int value);

        // Getters
        PieceType getType();
        Colour getColour();
        int getValue();

        // Setters
        void setType(PieceType piece_input);
        void setColour(Colour color_input);
        void setValue(int value_input);


};
