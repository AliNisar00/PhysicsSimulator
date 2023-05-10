#pragma once
#include "Square.h"

class Borders : public Square
{
    Square* bords;

public:

    void addBorders();

    ~Borders();
};