#ifndef _PIECE_H_
#define _PIECE_H_
#include <iostream>
#include <vector>

#include "colourEnum.cc"
#include "pieceEnum.cc"

class Piece {
  protected:
    ColourEnum colour;
  public:
    // Constructor
    Piece(ColourEnum colour);

    // Returns the piece's type. Uppercase letters are white pieces, while lowercase is black
    virtual PieceEnum getPieceType() = 0;

    // Returns a vector of strings representing the piece's valid moves (ex: "a1")
    virtual std::vector<std::vector<std::string>> getValidMoves(std::string coord, int cols, int rows, bool firstMove = false) = 0; 
};
#endif
