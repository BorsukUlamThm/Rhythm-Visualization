#include "Rhythm.h"
#include <fstream>


Note::Note(Time_t timing,
		   bool accented) :
		timing(timing),
		accented(accented)
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

Rhythm_set::Rhythm_set(double bpm) :
		bpm(bpm)
{}

void Rhythm_set::new_rhythm()
{ rhythms.emplace_back(); }

Rhythm& Rhythm_set::back()
{ return rhythms.back(); }

const Rhythm& Rhythm_set::operator[](unsigned i) const
{ return rhythms[i]; }

Rhythm& Rhythm_set::operator[](unsigned i)
{ return rhythms[i]; }

unsigned Rhythm_set::size() const
{ return rhythms.size(); }

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
	for (auto& note : rhythm.notes)
	{
		std::cout << note << std::endl;
	}

	return os;
}

std::vector<std::string> split_line(const std::string& line)
{
	std::istringstream iss(line);
	std::vector<std::string> res;
	std::string word;

	while (!iss.eof())
	{
		iss >> word;
		res.push_back(word);
	}

	return res;
}

Time_t read_time(const std::string& word)
{
	unsigned i = 0;
	while (i < word.size() && word[i] != '/')
	{ ++i; }

	integer n;
	integer d = 1;

	if (i == word.size())
	{
		n = std::stoi(word);
	}
	else
	{
		n = std::stoi(word.substr(0, i));
		d = std::stoi(word.substr(i + 1, word.size() - i - 1));
	}

	return {n, d};
}

void update_nb_beats(Rhythm_set& rhythm_set,
					 Time_t& total_time)
{
	integer n = total_time.numerator();
	integer d = total_time.denominator();
	unsigned nb_nbeats = n / d;
	nb_nbeats += (n % d == 0 ? 0 : 1);

	if (nb_nbeats > rhythm_set.nb_beats)
	{ rhythm_set.nb_beats = nb_nbeats; }
}

void read_line(const std::string& line,
			   Rhythm_set& rhythm_set,
			   Time_t& total_time)
{
	if (line.empty())
	{ return; }

	std::vector<std::string> words = split_line(line);
	if (words[0][0] == '#')
	{ return; }

	if (words[0] == "bpm")
	{
		rhythm_set.bpm = std::stoi(words[1]);
		return;
	}

	if (words[0] == "music")
	{
		rhythm_set.music_file = words[1];
		return;
	}

	if (words[0] == "new")
	{
		update_nb_beats(rhythm_set, total_time);
		rhythm_set.new_rhythm();
		total_time = 0;
		return;
	}

	if (words[0] == "polygon")
	{
		rhythm_set.back().draw_polygon = true;
		return;
	}

	if (words[0] == "hit_sound")
	{
		rhythm_set.back().hit_sound_file = words[1];
		return;
	}

	Time_t t = read_time(words[0]);
	Note note(total_time);
	note.accented = (words.size() > 1 && words[1] == "A");
	rhythm_set.back().add_note(note);

	total_time += t;
}

Rhythm_set load_rhythms(const std::string& file_name)
{
	std::string file_path = "../data/" + file_name;
	std::ifstream ifs(file_path);
	std::string line;
	Rhythm_set rhythm_set;

	rhythm_set.new_rhythm();
	Time_t total_time = 0;

	for (; !ifs.eof(); std::getline(ifs, line))
	{
		read_line(line, rhythm_set, total_time);
	}
	read_line(line, rhythm_set, total_time);
	update_nb_beats(rhythm_set, total_time);

	return rhythm_set;
}
