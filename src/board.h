#ifndef _BOARD_H_
#define _BOARD_H_
#include <iostream>
#include <map>
#include <memory>

#include "subject.h"
#include "square.h"
#include "playerEnum.cc"
#include "pieceEnum.cc"

class Board : public Subject {
  private:
    const int rows;
    const int cols;
    Player players[2];
    std::map<std::string, std::unique_ptr<Square>> squares;
    std::string lastAction; // coordinate
    bool isWhiteTurn;
    int difficulty;
  public:
    Board(const int rows, const int cols);
    Square *getRecentSquareWithAction();
    bool setSquare(PieceEnum p, bool isWhiteTurn, std::string coord, bool firstTurn = false); // true for success, false otherwise
    void finishTurn(); // renders the board
    std::vector<std::string> getValidMoves(bool firstTurn = false);
};
#endif
