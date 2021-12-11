#include <iostream>
#include <map>
#include <memory>

#include "playerEnum.cc"
#include "pieceEnum.cc"

class Square;

class Board {
  private:
    const int rows;
    const int cols;
    Player players[2];
    std::map<std::string, std::unique_ptr<Square>> squares;
    bool isWhiteTurn;
    int difficulty;
  public:
    Board(const int rows, const int cols);
    bool setSquare(PieceEnum p, bool isWhiteTurn, std::string coord); // true for success, false otherwise
};