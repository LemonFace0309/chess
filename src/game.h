#ifndef _GAME_H_
#define _GAME_H_
#include <iostream>
#include <memory>
#include "map"

#include "board.h"
#include "playerEnum.cc"
#include "colourEnum.cc"

class Game {
  private:
    Player players[2];
    bool isFirstTurn;
    std::unique_ptr<Board> board;
    bool isWhiteTurn;
    std::map<std::string, ColourEnum> stringToColourEnum;
  public:
    Game();
    Game(Player p1, Player p2);
    void setDefaults();
    void start(); // starts with a new board
    void begin(); // starts with the current board
    void setup();
    void changeTurn(ColourEnum colour);
};
#endif
