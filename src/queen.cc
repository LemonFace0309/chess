#include <iostream>
#include <vector>

#include "queen.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"
#include "pieceEnum.cc"
using namespace std;

Queen::Queen(ColourEnum colour) : Piece{colour} {}

PieceEnum Queen::getPieceType() {
  if (colour == ColourEnum::Black) {
    return PieceEnum::q;
  } else {
    return PieceEnum::Q;
  }
}

vector<string> Queen::getValidMoves(string coord) {
  vector<string> validMoves;
  return validMoves;
}
