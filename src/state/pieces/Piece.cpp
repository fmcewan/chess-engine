#include "Piece.hpp"
#include <utility>

// Constructor

Piece::Piece(PieceType piece_type_input, Colour color_input, int value_input): 
            type(piece_type_input), 
            colour(color_input), 
            value(value_input) {}

// Getters
PieceType Piece::getType() {
    return this->type;
}

Colour Piece::getColour() {
    return this->colour;
}

int Piece::getValue() {
    return this->value;
}

// Setters
void Piece::setType(PieceType piece_type_input) {
    this->type = piece_type_input;
}

void Piece::setColour(Colour color_input) {
    this->colour = color_input;
}

void Piece::setValue(int value_input) {
    this->value = value_input;
}


