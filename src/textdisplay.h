#ifndef _TEXTDISPLAY_H_
#define _TEXTDISPLAY_H_
#include <iostream>
#include <vector>

#include "observer.h"
#include "pieceEnum.cc"

class Board;

class TextDisplay : public Observer {
  private:
    Board *subject;
    int cols;
    int rows;
    std::vector<std::vector<PieceEnum>> display; // first col then row
  public:
    TextDisplay(Board *subject, int cols, int rows);
    void notify() override;
    void render() override;
    ~TextDisplay();
};

#endif