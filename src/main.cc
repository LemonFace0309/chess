#include <iostream>
#include <memory>
#include "game.h"
using namespace std;

int main() {
  string cmd;
  cin >> cmd;
  if (cmd == "game") {
    string p1;
    string p2;
    cin >> p1 >> p2;
    auto game = std::make_unique<Game>(p1, p2);
    game->start();
  } else if (cmd == "setup") {
    auto game = std::make_unique<Game>();
    game->start();
  }
}