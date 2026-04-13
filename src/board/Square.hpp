#pragma once
#include "../pieces/Piece.hpp"

struct Location {
    int x;
    int y;
};

class Square {

    private:
        Piece piece;
        Location location;

    public:       
        // Constructor
        Square(Piece setPiece, int setLocationX, int setLocationY);

        // Getters
    
        int getLocationX();
        int getLocationY();
        Location getLocation();        
        
        Piece getPiece();

        // Setters
        void setLocationX(int setLocationX);
        void setLocationY(int setLocationY);
        void setLocation(int setLocationX, int setLocationY);

        void setPiece(Piece setPiece);

};      
