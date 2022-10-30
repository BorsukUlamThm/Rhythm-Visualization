#include <iostream>
#include "model/Rhythm.h"
#include "view/Canvas.h"


int main()
{
	Beat beat1(1);

	Rhythm rhythm;
	rhythm.add_beat(beat1);
	rhythm.add_beat(2);
	rhythm.add_beat(4, true);

	Canvas canvas;
    canvas.display_rhythm(rhythm);

	return 0;
}
