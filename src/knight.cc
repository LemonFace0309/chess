#include <iostream>
#include <vector>
#include <string>

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

vector<vector<string>> Knight::getValidMoves(string coord, int cols, int rows, bool firstMove) {
  int x = coord[0] - 97 + 1; // a, b, ..., g => 1, 2, ... 8
  int y = coord[1] - 49 + 1; // 1, 2, ..., 8

  vector<vector<string>> validMoves;
  vector<string> upRight{ string(1, char(x + 96) + 1) + to_string(y + 2)};
  vector<string> rightUp{ string(1, char(x + 96) + 2) + to_string(y + 1)};
  vector<string> rightDown{ string(1, char(x + 96) + 2) + to_string(y - 1)};
  vector<string> downRight{ string(1, char(x + 96) + 1) + to_string(y - 2)};
  vector<string> downLeft{ string(1, char(x + 96) - 1) + to_string(y  - 2)};
  vector<string> leftDown{ string(1, char(x + 96) - 2) + to_string(y - 1)};
  vector<string> leftUp{ string(1, char(x + 96) - 2) + to_string(y + 1)};
  vector<string> upLeft{ string(1, char(x + 96) - 1) + to_string(y + 2)};

  // clearing moves that are outside of the board
  if (y + 2 > rows) {
    upLeft.clear();
    upRight.clear();
  } 
  if (x + 2 > cols) {
    rightUp.clear();
    rightDown.clear();
  }
   if (x - 2 < 1) {
    leftDown.clear();
    leftUp.clear();
  } 
  if (y - 2 < 1) {
    downRight.clear();
    downLeft.clear();
  }


  validMoves.push_back(upRight);
  validMoves.push_back(rightUp);
  validMoves.push_back(rightDown);
  validMoves.push_back(downRight);
  validMoves.push_back(downLeft);
  validMoves.push_back(leftDown);
  validMoves.push_back(leftUp);
  validMoves.push_back(upLeft);
  return validMoves;
}
