#ifndef _BOARD_H_
#define _BOARD_H_
#include <iostream>
#include <map>
#include <memory>

#include "window.h"
#include "subject.h"
#include "square.h"
#include "playerEnum.cc"
#include "pieceEnum.cc"
#include "colourEnum.cc"

class Board : public Subject {
  private:
    const int rows;
    const int cols;
    int difficulty;
    ColourEnum winner;
    ColourEnum loser;
    std::shared_ptr<Xwindow> window;
    std::map<std::string, std::unique_ptr<Square>> squares;
    std::map<std::string, std::vector<std::string>> allValidMoves; // keys are "black" and "white"
    std::string lastAction; // coordinate 
    bool isWhiteTurn;
    std::string whiteKingCoord;
    std::string blackKingCoord;
    void setDefaults();
    void flattenMoves(std::string coord, PieceEnum pieceEnum, std::vector<std::vector<std::string>> allMoves, ColourEnum other,
                      std::vector<std::string> &validWhiteMoves, std::vector<std::string> &validBlackMoves);
    // like flattenMoves except the only allowed moves are moves that stop the check
    void flattenCheckedMoves(std::string coord, PieceEnum pieceEnum, std::vector<std::vector<std::string>> allMoves, 
                             std::vector<std::string> validCheckMoves, ColourEnum other, std::vector<std::string> &validWhiteMoves, 
                             std::vector<std::string> &validBlackMoves);
    void findAllValidMoves(bool firstTurn = false);
  public:
    Board(const int rows, const int cols);
    void move(std::string coord1, std::string coord2, bool firstTurn);
    Square *getRecentSquareWithAction();
    bool isValidMove(std::string coord1, std::string coord2, bool firstTurn);
    std::vector<std::string> possibleUncheckMoves(std::string checkCoord, bool isWhiteTurn);
    bool setSquare(PieceEnum p, bool isWhiteTurn, std::string coord, bool firstTurn = false); // true for success, false otherwise
    void reset();
    void setTurn(bool isWhiteTurn);
    ColourEnum getLoser();
    void setLoser(ColourEnum c);
    ColourEnum getWinner();
    void setWinner(ColourEnum c);
    std::string isPlayerChecked(bool isWhiteTurn);
    void render(); // renders the board
    void finishTurn(bool firstTurn = false); // ends the turn and renders the board
    void displayGraphicText(std::string msg);
    ~Board();
};
#endif
