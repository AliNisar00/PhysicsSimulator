#include "Borders.h"


void Borders::addBorders()
{
    addRect(WIDTH / 2, HEIGHT - 30, 1300, 10, false); //floor
    addRect(WIDTH / 30, 250, 10, 800, false); //leftBorder
    addRect(632, 250, 10, 800, false); //middle
    addRect(1236, 250, 10, 800, false); //rightBorder
    addRect(WIDTH / 2, 25, 1300, 10, false); //top
}
