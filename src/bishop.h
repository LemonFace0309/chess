#ifndef _BISHOP_H_
#define _BISHOP_H_
#include <iostream>
#include <vector>

#include "piece.h"
#include "pieceEnum.cc"

class Bishop : public Piece {
  public:
    PieceEnum getPieceType();
    std::vector<std::string> getValidMoves(std::string coord);
};
#endif