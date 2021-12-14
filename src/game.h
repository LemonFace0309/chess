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
    std::unique_ptr<Board> board;
    bool isWhiteTurn;
    std::map<std::string, ColourEnum> stringToColourEnum;
  public:
    Game();
    Game(Player p1, Player p2);
    void setDefaults();
    void start();
    void setup();
    void changeTurn(std::string colour);
};
#endif
