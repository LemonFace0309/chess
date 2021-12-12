#ifndef _PIECE_H_
#define _PIECE_H_
#include <iostream>
#include <vector>

#include "colourEnum.cc"
#include "pieceEnum.cc"

class Piece {
  private:
    ColourEnum colour;
  public:
    Piece(ColourEnum colour);
    virtual PieceEnum getPieceType() = 0;
    virtual std::vector<std::string> getValidMoves(std::string coord) = 0;
};
#endif
