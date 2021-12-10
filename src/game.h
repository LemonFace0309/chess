#include <iostream>
#include "playerEnum.h"

class Game {
  private:
    Player players[2];
  public:
    Game();
    Game(Player p1, Player p2);
    void start();
    void setup();
};