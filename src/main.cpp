#include <iostream>
#include "model/Rhythm.h"
#include "view/Canvas.h"


void test1()
{
    Rhythm rhythm = load_rhythm("test");

    Canvas canvas;
    canvas.display_rhythm(rhythm);
}

int main()
{
    test1();

    return 0;
}
