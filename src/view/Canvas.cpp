#include "Canvas.h"
#include <cmath>


void Canvas::display_rhythm(const Rhythm& rhythm)
{
	open();
	setup_view();
	while (window.isOpen())
	{
		handle_events();
		draw_rhythm(rhythm);
		window.display();
	}
}

void Canvas::open()
{
	window.clear(sf::Color::White);
	if (window.isOpen())
	{ return; }

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create(sf::VideoMode::getDesktopMode(),
				  "Rhythm Visualization",
				  sf::Style::Default, settings);

	size_x = float(window.getSize().x);
	size_y = float(window.getSize().y);
}

void Canvas::setup_view()
{
	float box_size = 1.2f;
	float x_min = -box_size;
	float x_max = box_size;
	float y_min = -box_size;
	float y_max = box_size;

	float window_format = size_x / size_y;
	float figure_format = (x_max - x_min) / (y_max - y_min);
	float x_length = (x_max - x_min);
	float y_length = (y_max - y_min);

	if (window_format < figure_format)
	{
		view.setSize(x_length, x_length / window_format);
	}
	else
	{
		view.setSize(y_length * window_format, y_length);
	}

	float ratio = view.getSize().x / size_x;
	view.setCenter((x_min + x_max) / 2, (y_min + y_max) / 2);

	window.setView(view);
}

void Canvas::handle_events()
{
	sf::Event event {};
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::Resized:
				size_x = float(event.size.width);
				size_y = float(event.size.height);
				setup_view();
				break;

			default:
				break;
		}
	}
}

void Canvas::draw_rhythm(const Rhythm& rhythm)
{
	window.clear(sf::Color::White);
    draw_center_circle();
    for(unsigned i = 0; i < rhythm.notes.size(); ++i)
    {
        draw_ith_note(rhythm, i);
    }
}

void Canvas::draw_center_circle()
{
    float ratio = view.getSize().x / size_x;
    float thickness = 20 * ratio;
    float R = 1 + thickness / 2;
    float r = 1 - thickness / 2;
    unsigned nb_points = 64;

    sf::CircleShape ext_disk(R, nb_points);
    ext_disk.move(-R, -R);
    ext_disk.setFillColor(sf::Color::Black);

    sf::CircleShape int_disk(r, nb_points);
    int_disk.move(-r, -r);
    int_disk.setFillColor(sf::Color::White);

    window.draw(ext_disk);
    window.draw(int_disk);
}

void Canvas::draw_ith_note(const Rhythm& rhythm,
                           unsigned i)
{
    float ratio = view.getSize().x / size_x;
    float radius = 30 * ratio;

    float pi = 2 * std::acos(0.0f);
    float theta = 2 * pi
                * boost::rational_cast<float>(rhythm[i].timing)
                / float(rhythm.nb_beats());
    float x = std::cos(theta);
    float y = -std::sin(theta);

    sf::CircleShape disk(radius);
    disk.move(x - radius, y - radius);
    disk.setFillColor(rhythm[i].accented ? sf::Color::Red : sf::Color::Blue);

    window.draw(disk);
}
