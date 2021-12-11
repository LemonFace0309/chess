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
    void setPiece(PieceEnum p, bool isWhiteTurn);
    bool isValidMove(std::string coord, bool isWhiteTurn);
};
