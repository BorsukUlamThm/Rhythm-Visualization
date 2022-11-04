#include <iostream>
#include "model/Rhythm.h"
#include "view/Canvas.h"


int main(int argc, char** argv)
{
	std::string file(argv[1]);

	Rhythm_set rhythm_set = load_rhythms(file);
	Canvas canvas;
	canvas.display_rhythm(rhythm_set);

	return 0;
}
