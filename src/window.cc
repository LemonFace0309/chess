#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "window.h"

using namespace std;

// Constructor
Xwindow::Xwindow() {
  int width = squareSize * 12; 
  int height = squareSize * 14;
  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  char color_vals[10][10]={"white", "black", "red", "green", "blue", "cyan", "yellow", "magenta", "orange", "brown"};

  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 5; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

  usleep(1000);
}

// Destructor
Xwindow::~Xwindow() {
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

// Draws a chessboard square
void Xwindow::drawSquare(int x, int y, int colour) {
  // Changes the squares's color and draws the square
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, squareSize, squareSize);
  XSetForeground(d, gc, colours[Black]);
}

// Draws a string
void Xwindow::drawString(int x, int y, string msg, int colour) {
  // Calculates the x and y coordinates, so that the string will be centered on the chess square
  x += (squareSize - 6) / 2;
  y += (squareSize - 10) / 2 + 10;

  // Changes the string's color and draws the string
  XSetForeground(d, gc, colours[colour]);
  XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
  XSetForeground(d, gc, colours[Black]);
}

