#include <iostream>
#include <vector>
#include <string>

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

vector<vector<string>> King::getValidMoves(string coord, int cols, int rows, bool firstMove) {
  int x = coord[0] - 97 + 1; // a, b, ..., g => 1, 2, ... 8
  int y = coord[1] - 49 + 1; // 1, 2, ..., 8

  vector<vector<string>> validMoves;
  vector<string> upMove{ string(1, char(x + 96)) + to_string(y + 1)};
  vector<string> downMove{ string(1, char(x + 96)) + to_string(y - 1)};
  vector<string> leftMove{ string(1, char(x + 96) - 1) + to_string(y)};
  vector<string> rightMove{ string(1, char(x + 96) + 1) + to_string(y)};
  vector<string> upLeftMove{ string(1, char(x + 96) - 1) + to_string(y + 1)};
  vector<string> upRightMove{ string(1, char(x + 96) + 1) + to_string(y + 1)};
  vector<string> downLeftMove{ string(1, char(x + 96) - 1) + to_string(y - 1)};
  vector<string> downRightMove{ string(1, char(x + 96) + 1) + to_string(y - 1)};

  // clearing moves that are outside of the board
  if (y + 1 > rows && x - 1 < 1) {
    upLeftMove.clear();
    upMove.clear();
    leftMove.clear();
  } else if (y + 1 > rows && x + 1 > cols) {
    upRightMove.clear();
    upMove.clear();
    rightMove.clear();
  } else if (y - 1 < 1 && x - 1 < 1) {
    downLeftMove.clear();
    downMove.clear();
    leftMove.clear();
  } else if (y - 1 < 1 && x + 1 > cols) {
    downRightMove.clear();
    downMove.clear();
    rightMove.clear();
  } else if (y + 1 > rows) {
    upMove.clear();
  } else if (y - 1 < 1) {
    downMove.clear();
  } else if (y + 1 > cols) {
    leftMove.clear();
  } else if (y - 1 < 1) {
    rightMove.clear();
  }


  validMoves.push_back(upMove);
  validMoves.push_back(downMove);
  validMoves.push_back(leftMove);
  validMoves.push_back(rightMove);
  validMoves.push_back(upLeftMove);
  validMoves.push_back(upRightMove);
  validMoves.push_back(downLeftMove);
  validMoves.push_back(downRightMove);
  return validMoves;
}
