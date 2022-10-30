#pragma once

#include <vector>
#include <iostream>


struct Beat
{
	double timing = 0; // timing/bpm will be the time when the beat slaps
	bool accented = false;

	Beat() = default;
	Beat(const Beat& other) = default;
	explicit Beat(double timing,
				  bool accented = false);
};

struct Rhythm
{
	typedef std::vector<Beat> Beat_container;

	double bpm = 60;
	Beat_container beats;

	Rhythm() = default;
	Rhythm(const Rhythm& other) = default;
	explicit Rhythm(double bpm);

	void add_beat(const Beat& beat);
	void add_beat(double timing,
				  bool accented = false);

	const Beat& operator[](unsigned i) const;
	Beat& operator[](unsigned i);
};

std::ostream& operator<<(std::ostream& os,
						 const Beat& beat);

std::ostream& operator<<(std::ostream& os,
						 const Rhythm& rhythm);

