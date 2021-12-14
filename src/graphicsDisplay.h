#ifndef _GRAPHICSDISPLAY_H_
#define _GRAPHICSDISPLAY_H_
#include "observer.h"
#include "pieceEnum.cc"
#include "window.h"

class Board;

class GraphicsDisplay : public Observer {
  private:
    Board *subject;
    int cols;
    int rows;
    std::vector<std::vector<PieceEnum>> display; // first col then row
    Xwindow *window;
  public:
    GraphicsDisplay(Board *subject, int cols, int rows);
    void notify() override;
    void render() override;
    ~GraphicsDisplay();
};

#endif
