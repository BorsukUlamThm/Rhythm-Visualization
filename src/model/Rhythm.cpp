#include "Rhythm.h"


Beat::Beat(double timing,
		   bool accented) :
        timing(timing),
        accented(accented)
{}

Rhythm::Rhythm(double bpm) :
		bpm(bpm)
{}

void Rhythm::add_beat(const Beat& beat)
{
	beats.push_back(beat);
}

void Rhythm::add_beat(double timing,
                      bool accented)
{
	beats.emplace_back(timing, accented);
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
	std::cout << beat.timing
			  << (beat.accented ? " (accented)" : "");

    return os;
}

std::ostream& operator<<(std::ostream& os,
						 const Rhythm& rhythm)
{
	std::cout << "bpm : " << rhythm.bpm << std::endl;
	for(auto& beat : rhythm.beats)
	{
		std::cout << beat << std::endl;
	}

    return os;
}