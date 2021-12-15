#include <iostream>
#include <vector>
#include <string>


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

vector<vector<string>> Rook::getValidMoves(string coord, int cols, int rows, bool firstMove) {
  int x = coord[0] - 97 + 1; // a, b, ..., g => 1, 2, ... 8
  int y = coord[1] - 49 + 1; // 1, 2, ..., 8

  vector<vector<string>> validMoves;
  vector<string> upMoves;
  vector<string> downMoves;
  vector<string> leftMoves;
  vector<string> rightMoves;

  // gets up moves
  for (int i = 1; y + i <= rows; ++i) {
    char c = x + 96;
    upMoves.push_back(string(1, char(c)) + to_string(y + i));
  }

  // gets down moves
  for (int i = 1; y - i >= 1; ++i) {
    char c = x + 96;
    downMoves.push_back(string(1, char(c)) + to_string(y - i));
  }

  // gets left moves
  for (int i = 1; x - i >= 1; ++i) {
    char c = x - i + 96;
    leftMoves.push_back(string(1, char(c)) + to_string(y));
  }

  // gets right moves
  for (int i = 1; x + i <= cols; ++i) {
    char c = x + i + 96;
    rightMoves.push_back(string(1, char(c)) + to_string(y));
  }

  validMoves.push_back(upMoves);
  validMoves.push_back(downMoves);
  validMoves.push_back(leftMoves);
  validMoves.push_back(rightMoves);
  return validMoves;
}
