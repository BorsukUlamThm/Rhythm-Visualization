#include "Rhythm.h"


Note::Note(Time_t timing,
           bool accented) :
        timing(timing),
        accented(accented)
{}

Rhythm::Rhythm(double bpm) :
		bpm(bpm)
{}

void Rhythm::add_note(const Note& note)
{
	notes.push_back(note);
}

void Rhythm::add_note(Time_t timing,
                      bool accented)
{
	notes.emplace_back(timing, accented);
}

const Note& Rhythm::operator[](unsigned i) const
{
	return notes[i];
}

Note& Rhythm::operator[](unsigned i)
{
	return notes[i];
}

unsigned Rhythm::nb_beats() const
{
    integer n = notes.back().timing.numerator();
    integer d = notes.back().timing.denominator();
    return 1 + n / d;
}

std::ostream& operator<<(std::ostream& os,
						 const Note& note)
{
	std::cout << note.timing
              << (note.accented ? " (accented)" : "");

    return os;
}

std::ostream& operator<<(std::ostream& os,
						 const Rhythm& rhythm)
{
	std::cout << "bpm : " << rhythm.bpm << std::endl;
	for(auto& note : rhythm.notes)
	{
		std::cout << note << std::endl;
	}

    return os;
}