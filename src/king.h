#ifndef _KING_H_
#define _KING_H_
#include <iostream>
#include <vector>

#include "piece.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"

class King : public Piece {
  public:
    King(ColourEnum colour);
    PieceEnum getPieceType();
    std::vector<std::vector<std::string>> getValidMoves(std::string coord, int cols, int rows, bool firstMove = false) override;
};
#endif
