#ifndef _SQUARE_H_
#define _SQUARE_H_
#include <iostream>
#include <memory>

#include "piece.h"
#include "pieceEnum.cc"


class Square {
  private:
    int row;
    int col;
    std::unique_ptr<Piece> piece;
  public:
    Square(int row, int col);
    int getRow();
    int getCol();
    Piece *getPiece();
    void setPiece(PieceEnum p, bool isWhiteTurn);
    bool isValidMove(std::string coord, bool isWhiteTurn);
};
#endif
