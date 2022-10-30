#include <iostream>
#include "model/Rhythm.h"
#include "view/Canvas.h"


int main()
{
    Time_t half (1, 2);
    Time_t quarter (1, 4);

	Rhythm rhythm;
    rhythm.add_note(0, true);
    rhythm.add_note(1, true);
    rhythm.add_note(1 + half);
    rhythm.add_note(2, true);
    rhythm.add_note(3, true);
    rhythm.add_note(3 + quarter);
    rhythm.add_note(3 + 2 * quarter);
    rhythm.add_note(3 + 3 * quarter);

	Canvas canvas;
    canvas.display_rhythm(rhythm);

	return 0;
}
