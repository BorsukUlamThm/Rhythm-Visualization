#include <iostream>
#include "model/Rhythm.h"
#include "view/Canvas.h"


int main()
{
    Rhythm rhythm = load_rhythm("test");

	Canvas canvas;
    canvas.display_rhythm(rhythm);

	return 0;
}
