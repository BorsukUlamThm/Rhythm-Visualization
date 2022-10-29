#include <iostream>
#include "model/Rhythm.h"
#include "view/Canvas.h"


int main()
{
	Beat beat1(1);

	Rhythm rythme;
	rythme.add_beat(beat1);
	rythme.add_beat(2);
	rythme.add_beat(4, true);

	Canvas canvas;
	canvas.display_rythme(rythme);

	return 0;
}
