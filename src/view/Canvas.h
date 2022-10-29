#ifndef RYTHME_VISUALIZATION_CANVAS_H
#define RYTHME_VISUALIZATION_CANVAS_H

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
	void display_rythme(const Rhythm& rythme);

protected:
	void open();
	void setup_view();
	void handle_events();

	void draw_rythme();
	void display_center_circle();
};


#endif //RYTHME_VISUALIZATION_CANVAS_H
