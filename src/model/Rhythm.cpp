#include "Rhythm.h"
#include <fstream>


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

void read_line(const std::string& line,
               Rhythm& rhythm,
               Time_t& total_time)
{
    if (line.empty())
    { return; }

    std::vector<std::string> words = split_line(line);
    if (words[0][0] == '#')
    { return; }

    if (words[0] == "bpm")
    {
        rhythm.bpm = std::stoi(words[1]);
        return;
    }

    Time_t t = read_time(words[0]);
    Note note(total_time);
    note.accented = (words.size() > 1 && words[1] == "A");
    rhythm.add_note(note);

    total_time += t;
}

Rhythm load_rhythm(const std::string& file_name)
{
    std::string file_path = "../data/" + file_name;
    std::ifstream ifs(file_path);
    std::string line;
    Rhythm rhythm;

    Time_t total_time = 0;

    for (; !ifs.eof(); std::getline(ifs, line))
    {
        read_line(line, rhythm, total_time);
    }
    read_line(line, rhythm, total_time);

    integer n = total_time.numerator();
    integer d = total_time.denominator();
    rhythm.nb_beats = n / d;
    rhythm.nb_beats += (n % d == 0 ? 0 : 1);

    return rhythm;
}
