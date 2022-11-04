#include <iostream>
#include "model/Rhythm.h"
#include "view/Canvas.h"


void test1()
{
	Rhythm_set rhythm_set = load_rhythms("test");

	Canvas canvas;
	canvas.display_rhythm(rhythm_set);
}

int main()
{
	test1();

	return 0;
}
