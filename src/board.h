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
    std::map<std::string, std::vector<std::string>> allValidMoves;
    std::string lastAction; // coordinate
    bool isWhiteTurn;
    int difficulty;
    std::string whiteKingCoord;
    std::string blackKingCoord;
    //string playerUnderCheck; // in the form of "[(b or w)] [from coordinate (ex: e1)]"
  public:
    Board(const int rows, const int cols);
    Square *getRecentSquareWithAction();
    bool isValidMove(std::string coord1, std::string coord2, bool firstTurn);
    bool isChecked(bool isWhiteChecked);
    void flattenMoves(std::string coord, PieceEnum pieceEnum, std::vector<std::vector<std::string>> allMoves, ColourEnum other,
                      std::vector<std::string> &validWhiteMoves, std::vector<std::string> &validBlackMoves);
    bool setSquare(PieceEnum p, bool isWhiteTurn, std::string coord, bool firstTurn = false); // true for success, false otherwise
    void finishTurn(); // renders the board
    void findAllValidMoves(bool firstTurn = false);
};
#endif
