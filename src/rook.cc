#include <iostream>
#include <vector>

#include "rook.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"
#include "pieceEnum.cc"
using namespace std;

Rook::Rook(ColourEnum colour) : Piece{colour} {}

PieceEnum Rook::getPieceType() {
  if (colour == ColourEnum::Black) {
    return PieceEnum::r;
  } else {
    return PieceEnum::R;
  }
}

vector<string> Rook::getValidMoves(string coord) {
  vector<string> validMoves;
  return validMoves;
}
