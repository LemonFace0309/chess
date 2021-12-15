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

vector<vector<string>> Bishop::getValidMoves(string coord, int cols, int rows, bool firstMove) {
  int x = coord[0] - 97 + 1; // a, b, ..., g => 1, 2, ... 8
  int y = coord[1] - 49 + 1; // 1, 2, ..., 8

  vector<vector<string>> validMoves;
  vector<string> upLeftMoves;
  vector<string> upRightMoves;
  vector<string> downLeftMoves;
  vector<string> downRightMoves;

  // gets up left moves
  for (int i = 1; x - i >= 1 && y + i <= rows; ++i) {
    char c = x - i + 96;
    upLeftMoves.push_back(string(1, c) + to_string(y + i));
  }

  // gets up right moves
  for (int i = 1; x + i <= cols && y + i <= rows; ++i) {
    char c = x + i + 96;
    upRightMoves.push_back(string(1, c) + to_string(y + i));
  }

  // gets down left moves
  for (int i = 1; x - i >= 1 && y - i >= 1; ++i) {
    char c = x - i + 96;
    downLeftMoves.push_back(string(1, c) + to_string(y - i));
  }

  // gets down right moves
  for (int i = 1; x + i <= cols && y - i >= 1; ++i) {
    char c = x + i + 96;
    downRightMoves.push_back(string(1, c) + to_string(y - i));
  }

  validMoves.push_back(upLeftMoves);
  validMoves.push_back(upRightMoves);
  validMoves.push_back(downLeftMoves);
  validMoves.push_back(downRightMoves);
  return validMoves;
}
