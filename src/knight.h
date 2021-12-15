#ifndef _KNIGHT_H_
#define _KNIGHT_H_
#include <iostream>
#include <vector>

#include "piece.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"

class Knight : public Piece {
  public:
    Knight(ColourEnum colour);
    PieceEnum getPieceType();
    std::vector<std::vector<std::string>> getValidMoves(std::string coord, int cols, int rows, bool firstMove = false) override;
};
#endif
