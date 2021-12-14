#include <iostream>
#include <vector>

#include "pawn.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"
#include "pieceEnum.cc"
using namespace std;

Pawn::Pawn(ColourEnum colour) : Piece{colour} {}

PieceEnum Pawn::getPieceType() {
  if (colour == ColourEnum::Black) {
    return PieceEnum::p;
  } else {
    return PieceEnum::P;
  }
}

vector<string> Pawn::getValidMoves(string coord) {
  vector<string> validMoves;
  return validMoves;
}
