#include <iostream>
#include <vector>

#include "bishop.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"
using namespace std;

Bishop::Bishop(ColourEnum colour) : Piece{colour} {}

PieceEnum Bishop::getPieceType() {
  if (colour == ColourEnum::Black) {
    return PieceEnum::b;
  } else {
    return PieceEnum::B;
  }
}

vector<string> Bishop::getValidMoves(string coord) {
  vector<string> validMoves;
  return validMoves;
}