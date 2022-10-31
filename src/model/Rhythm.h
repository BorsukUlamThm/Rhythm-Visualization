#pragma once

#include <vector>
#include <iostream>
#include <boost/rational.hpp>


typedef int integer;
typedef boost::rational<integer> Time_t;

struct Note
{
	Time_t timing = 0; // timing/bpm will be the time when the note slaps
	bool accented = false;

	Note() = default;
	Note(const Note& other) = default;
	explicit Note(Time_t timing,
                  bool accented = false);
};

struct Rhythm
{
	double bpm = 60;
	std::vector<Note> notes;
    unsigned nb_beats = 0;

	Rhythm() = default;
	Rhythm(const Rhythm& other) = default;
	explicit Rhythm(double bpm);

	void add_note(const Note& note);
	void add_note(Time_t timing,
                  bool accented = false);

	const Note& operator[](unsigned i) const;
	Note& operator[](unsigned i);
};

std::ostream& operator<<(std::ostream& os,
						 const Note& note);

std::ostream& operator<<(std::ostream& os,
						 const Rhythm& rhythm);

std::vector<Rhythm> load_rhythms(const std::string& file_name);
