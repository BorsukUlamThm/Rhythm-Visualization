#include "Rhythm.h"


Beat::Beat(double length,
		   bool accented) :
		length(length),
		accented(accented)
{}

Rhythm::Rhythm(double bpm) :
		bpm(bpm)
{}

void Rhythm::add_beat(const Beat& beat)
{
	beats.push_back(beat);
}

void Rhythm::add_beat(double length,
                      bool accented)
{
	beats.emplace_back(length, accented);
}

const Beat& Rhythm::operator[](unsigned i) const
{
	return beats[i];
}

Beat& Rhythm::operator[](unsigned i)
{
	return beats[i];
}

std::ostream& operator<<(std::ostream& os,
						 const Beat& beat)
{
	std::cout << beat.length
			  << (beat.accented ? " (accented)" : "");

    return os;
}

std::ostream& operator<<(std::ostream& os,
						 const Rhythm& rythme)
{
	std::cout << "bpm : " << rythme.bpm << std::endl;
	for(auto& beat : rythme.beats)
	{
		std::cout << beat << std::endl;
	}

    return os;
}