#include <iostream>
#include <vector>

#include "king.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"
#include "pieceEnum.cc"
using namespace std;

King::King(ColourEnum colour) : Piece{colour} {}

PieceEnum King::getPieceType() {
  if (colour == ColourEnum::Black) {
    return PieceEnum::k;
  } else {
    return PieceEnum::K;
  }
}

vector<string> King::getValidMoves(string coord) {
  vector<string> validMoves;
  return validMoves;
}
