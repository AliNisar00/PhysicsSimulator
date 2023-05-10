#include "Borders.h"

void Borders::addBorders()
{
    bords->addRect(WIDTH / 2, HEIGHT - 30, 1300, 10, false); //floor
    bords->addRect(WIDTH / 30, 250, 10, 800, false); //leftBorder
    bords->addRect(632, 250, 10, 800, false); //middle
    bords->addRect(1236, 250, 10, 800, false); //rightBorder
    bords->addRect(WIDTH / 2, 25, 1300, 10, false); //top
}

Borders::~Borders()
{
    delete bords;
}
