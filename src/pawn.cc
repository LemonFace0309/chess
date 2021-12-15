#include <iostream>
#include <vector>
#include <string>

#include "pawn.h"
#include "colourEnum.cc"
#include "pieceEnum.cc"
#include "pieceEnum.cc"
using namespace std;
#include <string>

Pawn::Pawn(ColourEnum colour) : Piece{colour} {}

PieceEnum Pawn::getPieceType() {
  // Checks if the piece is black or write
  if (colour == ColourEnum::Black) {
    return PieceEnum::p;
  } else {
    return PieceEnum::P;
  }
}

vector<vector<string>> Pawn::getValidMoves(string coord, int cols, int rows, bool firstMove) {
  int x = coord[0] - 97 + 1; // a, b, ..., g => 1, 2, ... 8
  int y = coord[1] - 49 + 1; // 1, 2, ..., 8

  vector<vector<string>> validMoves;
  vector<string> forwards;

  // filtering moves based on colour and if it's the first move
  if (colour == ColourEnum::White) {
    if (!(y + 1 > cols)) {
      forwards.push_back(string(1, char(x + 96)) + to_string(y + 1));
    }
    if (!(y + 2 > cols) && firstMove) {
      forwards.push_back(string(1, char(x + 96)) + to_string(y + 2));
    }
    
  } else {
    if (!(y - 1 < 1)) {
      forwards.push_back(string(1, char(x + 96)) + to_string(y - 1));
    }
    if (!(y - 2 < 1) && firstMove) {
      forwards.push_back(string(1, char(x + 96)) + to_string(y - 2));
    }
  }


  validMoves.push_back(forwards);
  return validMoves;
}
