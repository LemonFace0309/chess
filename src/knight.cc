#include <iostream>
#include <vector>

#include "knight.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"
#include "pieceEnum.cc"
using namespace std;

Knight::Knight(ColourEnum colour) : Piece{colour} {}

PieceEnum Knight::getPieceType() {
  if (colour == ColourEnum::Black) {
    return PieceEnum::n;
  } else {
    return PieceEnum::N;
  }
}

vector<string> Knight::getValidMoves(string coord) {
  vector<string> validMoves;
  return validMoves;
}
