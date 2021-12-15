#ifndef _SQUARE_H_
#define _SQUARE_H_
#include <iostream>
#include <memory>

#include "piece.h"
#include "pieceEnum.cc"


class Square {
  private:
    int col;
    int row;
    std::unique_ptr<Piece> piece;
  public:
    Square(int col, int row);

    // Returns the row the square is in
    int getRow();

    // Returns the column the square is in
    int getCol();

    // Returns the type of piece on the square
    Piece *getPiece();

    // Sets a piece onto the square
    void setPiece(PieceEnum p, bool isWhiteTurn);

    // Determines if the move to coord is a valid move or not
    bool isValidMove(std::string coord, bool isWhiteTurn);
};
#endif
