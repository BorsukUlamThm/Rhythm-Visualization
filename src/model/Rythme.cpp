#include "Rythme.h"


Beat::Beat(double length,
		   bool accented) :
		length(length),
		accented(accented)
{}

Rythme::Rythme(double bpm) :
		bpm(bpm)
{}

void Rythme::add_beat(const Beat& beat)
{
	beats.push_back(beat);
}

void Rythme::add_beat(double length,
					  bool accented)
{
	beats.emplace_back(length, accented);
}

const Beat& Rythme::operator[](unsigned i) const
{
	return beats[i];
}

Beat& Rythme::operator[](unsigned i)
{
	return beats[i];
}

std::ostream& operator<<(std::ostream& os,
						 const Beat& beat)
{
	std::cout << beat.length
			  << (beat.accented ? " (accented)" : "");
}

std::ostream& operator<<(std::ostream& os,
						 const Rythme& rythme)
{
	std::cout << "bpm : " << rythme.bpm << std::endl;
	for(auto& beat : rythme.beats)
	{
		std::cout << beat << std::endl;
	}
}