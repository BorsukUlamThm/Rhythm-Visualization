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
	std::vector<Note> notes;
	bool draw_polygon = false;

	Rhythm() = default;
	Rhythm(const Rhythm& other) = default;

	void add_note(const Note& note);
	void add_note(Time_t timing,
				  bool accented = false);

	const Note& operator[](unsigned i) const;
	Note& operator[](unsigned i);
};

struct Rhythm_set
{
	double bpm = 60;
	unsigned nb_beats = 0;
	bool hit_sounds;

	std::vector<Rhythm> rhythms;

	Rhythm_set() = default;
	explicit Rhythm_set(double bpm);

	void new_rhythm();

	Rhythm& back();
	const Rhythm& operator[](unsigned i) const;
	Rhythm& operator[](unsigned i);

	unsigned size() const;
};

std::ostream& operator<<(std::ostream& os,
						 const Note& note);

std::ostream& operator<<(std::ostream& os,
						 const Rhythm& rhythm);

Rhythm_set load_rhythms(const std::string& file_name);
