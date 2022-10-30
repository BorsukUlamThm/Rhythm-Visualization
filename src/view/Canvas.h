#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../model/Rhythm.h"


class Canvas
{
protected:
	sf::RenderWindow window;
	sf::View view;
	float size_x;
	float size_y;

public:
	void display_rhythm(const Rhythm& rhythm);

protected:
	void open();
	void setup_view();
	void handle_events();

	void draw_rhythm(const Rhythm& rhythm);
	void draw_center_circle();
    void draw_ith_note(const Rhythm& rhythm,
                       unsigned i);
};

