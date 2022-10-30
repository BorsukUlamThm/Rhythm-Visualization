#include "Canvas.h"
#include <cmath>


void Canvas::display_rhythm(const Rhythm& rhythm)
{
	open();
	setup_view();
	while (window.isOpen())
	{
		handle_events();
		draw_rhythm();
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

void Canvas::draw_rhythm()
{
	window.clear(sf::Color::White);
    draw_center_circle();
}

void Canvas::draw_center_circle()
{
	unsigned nb_vertices = 128;
	float thickness = 10 * view.getSize().x / size_x;
	sf::VertexArray shape(sf::TriangleStrip, 2 * (nb_vertices + 1));
	auto pi = 2 * float(std::acos(0));
	float angle_step = 2 * pi / float(nb_vertices);

	float theta = 0;
	for (unsigned i = 1; i < nb_vertices; ++i)
	{
		float x = (1 - thickness / 2) * std::cos(theta);
		float y = (1 - thickness / 2) * std::sin(theta);
		shape[2 * i].position = sf::Vector2f(x, y);

		x = (1 + thickness / 2) * std::cos(theta);
		y = (1 + thickness / 2) * std::sin(theta);
		shape[2 * i + 1].position = sf::Vector2f(x, y);

		theta += angle_step;
	}
	float x = 1 - thickness / 2;
	float y = 0;
	shape[0].position = sf::Vector2f(x, y);
	shape[2 * nb_vertices].position = sf::Vector2f(x, y);
	x = 1 + thickness / 2;
	shape[1].position = sf::Vector2f(x, y);
	shape[2 * nb_vertices + 1].position = sf::Vector2f(x, y);

	for (unsigned i = 0; i < shape.getVertexCount(); ++i)
	{
		shape[i].color = sf::Color::Black;
	}
	window.draw(shape);
}

void Canvas::draw_beat(const Beat &beat)
{

}
