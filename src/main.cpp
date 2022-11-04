#include <iostream>
#include "model/Rhythm.h"
#include "view/Canvas.h"


void test1()
{
	std::vector<Rhythm> rhythms = load_rhythms("test");

	Canvas canvas;
	canvas.display_rhythm(rhythms);
}

int main()
{
	test1();

	return 0;
}
