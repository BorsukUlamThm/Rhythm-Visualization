#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../model/Rhythm.h"
#include "../utils/Timer.h"


class Canvas
{
protected:
	sf::RenderWindow window;
	sf::View view;
	float size_x = 0;
	float size_y = 0;

    float big_circle_radius = 0.08;
    float small_circle_radius = 0.05;

    enum State
    {
        RUNNING,
        STOPPED
    };
    State state = STOPPED;

    Timer timer;

    sf::SoundBuffer hit_buffer;
    sf::Sound sound;
    std::vector<unsigned> next_note_indexes;

public:
    Canvas();
	void display_rhythm(const std::vector<Rhythm>& rhythms);

protected:
	void open();
	void setup_view();
	void handle_events();
    void handle_key_pressed_event(const sf::Event& event);

	void draw_rhythms(const std::vector<Rhythm>& rhythms);
    void draw_center_circles(const std::vector<Rhythm>& rhythms);
    void draw_time_line(const Rhythm& rhythm);
    void draw_beat_lines(const Rhythm& rhythm);
    void draw_highlighted_notes(const std::vector<Rhythm>& rhythms);
    void draw_ijth_note(const std::vector<Rhythm>& rhythms,
                        unsigned i,
                        unsigned j);

    float make_time_line_angle(const Rhythm& rhythm);
    float make_ith_note_angle(const Rhythm& rhythm,
                              unsigned i);

    void play_sounds(const std::vector<Rhythm>& rhythms);
};

