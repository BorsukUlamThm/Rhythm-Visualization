#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../model/Rhythm.h"
#include "../utils/Timer.h"


class Canvas
{
protected:
	sf::RenderWindow window;
	sf::View view;
	float size_x;
	float size_y;

    Timer timer;

public:
	void display_rhythm(const Rhythm& rhythm);

protected:
	void open();
	void setup_view();
	void handle_events();
    void handle_key_pressed_event(const sf::Event& event);

	void draw_rhythm(const Rhythm& rhythm);
    void draw_center_circle();
    void draw_time_line(const Rhythm& rhythm);
    void draw_beat_lines(const Rhythm& rhythm);
    void draw_highlighted_note(const Rhythm& rhythm);
    void draw_ith_note(const Rhythm& rhythm,
                       unsigned i);

    float make_time_line_angle(const Rhythm& rhythm);
    float make_ith_note_angle(const Rhythm& rhythm,
                              unsigned i);
};

