#include <iostream>
#include "model/Rythme.h"


int main()
{
	Beat beat1(1);

	Rythme rythme;
	rythme.add_beat(beat1);
	rythme.add_beat(2);
	rythme.add_beat(4, true);

	std::cout << rythme << std::endl;
	return 0;
}
