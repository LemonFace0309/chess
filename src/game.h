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
    std::map<ColourEnum, int> score;
    bool isWhiteTurn;
    void begin(); // starts with the current board - called by setup() and start()
    void setDefaults();
    void addPoints(ColourEnum winner, ColourEnum loser, bool stalemate = false);
    void printScore();
    void setWinner(ColourEnum winner, ColourEnum loser, bool stalemate = false);
  public:
    Game();
    Game(Player p1, Player p2);
    void start(); // starts with a new board
    void setup();
    void changeTurn(ColourEnum colour);
    ~Game();
};
#endif
