#ifndef _KNIGHT_H_
#define _KNIGHT_H_
#include <iostream>
#include <vector>

#include "piece.h"
#include "pieceEnum.cc"

class Knight : public Piece {
  public:
    PieceEnum getPieceType();
    std::vector<std::string> getValidMoves(std::string coord);
};
#endif
