#include <iostream>
#include <vector>

#include "piece.h"
#include "colourEnum.cc"

Piece::Piece(ColourEnum colour) : colour{colour} {};

ColourEnum Piece::getColour{
    return colour;
};