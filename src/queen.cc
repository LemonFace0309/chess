#include <iostream>
#include <vector>
#include <string>

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

vector<vector<string>> Queen::getValidMoves(string coord, int cols, int rows, bool firstMove) {
  int x = coord[0] - 97 + 1; // a, b, ..., g => 1, 2, ... 8
  int y = coord[1] - 49 + 1; // 1, 2, ..., 8

  vector<vector<string>> validMoves;
  vector<string> upMoves;
  vector<string> downMoves;
  vector<string> leftMoves;
  vector<string> rightMoves;
  vector<string> upLeftMoves;
  vector<string> upRightMoves;
  vector<string> downLeftMoves;
  vector<string> downRightMoves;

  // gets up moves
  for (int i = 1; y + 1 <= rows; ++i) {
    char c = x + 96;
    upMoves.push_back(string(1, char(c)) + to_string(y + i));
  }

  // gets down moves
  for (int i = 1; y - 1 >= 1; ++i) {
    char c = x + 96;
    downMoves.push_back(string(1, char(c)) + to_string(y - i));
  }

  // gets left moves
  for (int i = 1; x - 1 >= 1; ++i) {
    char c = x - i + 96;
    leftMoves.push_back(string(1, char(c)) + to_string(y));
  }

  // gets right moves
  for (int i = 1; x + 1 <= cols; ++i) {
    char c = x + i + 96;
    rightMoves.push_back(string(1, char(c)) + to_string(y));
  }

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

  validMoves.push_back(upMoves);
  validMoves.push_back(downMoves);
  validMoves.push_back(leftMoves);
  validMoves.push_back(rightMoves);
  validMoves.push_back(upLeftMoves);
  validMoves.push_back(upRightMoves);
  validMoves.push_back(downLeftMoves);
  validMoves.push_back(downRightMoves);
  return validMoves;
}
