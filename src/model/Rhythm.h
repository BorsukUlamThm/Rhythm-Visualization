#ifndef RYTHME_VISUALIZATION_RHYTHM_H
#define RYTHME_VISUALIZATION_RHYTHM_H

#include <vector>
#include <iostream>


struct Beat
{
	double length = 1;
	bool accented = false;

	Beat() = default;
	Beat(const Beat& other) = default;
	explicit Beat(double length,
				  bool accented = false);
};

struct Rhythm
{
	typedef std::vector<Beat> Beat_container;
	typedef Beat_container::iterator Beat_iterator;

	double bpm = 60;
	Beat_container beats;

	Rhythm() = default;
	Rhythm(const Rhythm& other) = default;
	explicit Rhythm(double bpm);

	void add_beat(const Beat& beat);
	void add_beat(double length,
				  bool accented = false);

	const Beat& operator[](unsigned i) const;
	Beat& operator[](unsigned i);
};

std::ostream& operator<<(std::ostream& os,
						 const Beat& beat);

std::ostream& operator<<(std::ostream& os,
						 const Rhythm& rythme);


#endif //RYTHME_VISUALIZATION_RHYTHM_H
