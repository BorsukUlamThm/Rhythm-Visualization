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

            case sf::Event::KeyPressed:
                handle_key_pressed_event(event);
                break;

			default:
				break;
		}
	}
}

void Canvas::handle_key_pressed_event(const sf::Event& event)
{
    switch (event.key.code)
    {
        case sf::Keyboard::Space:
            timer.start_or_stop();
            break;

        case sf::Keyboard::Enter:
            timer.stop();
            timer.reset();
            break;

        case sf::Keyboard::Escape:
        case sf::Keyboard::Q:
            window.clear();
            window.close();
            break;

        default:
            break;
    }
}

void Canvas::draw_rhythm(const Rhythm& rhythm)
{
	window.clear(sf::Color::White);
    draw_center_circle();
    draw_time_line(rhythm);
    draw_highlighted_note(rhythm);

    for(unsigned i = 0; i < rhythm.notes.size(); ++i)
    {
        draw_ith_note(rhythm, i);
    }
}

float Canvas::make_time_line_angle(const Rhythm& rhythm)
{
    float pi = 2 * std::acos(0.0f);
    return 2 * pi
         * timer.read()
         * rhythm.bpm / 60
         / float(rhythm.nb_beats);
}

void Canvas::draw_time_line(const Rhythm& rhythm)
{
    float pi = 2 * std::acos(0.0f);
    float theta = make_time_line_angle(rhythm);
    theta = pi / 2 - theta;
    float y = std::sin(theta);
    float x = std::cos(theta);

    float ratio = view.getSize().x / size_x;
    float thickness = 3 * ratio;
    float x_thickness = thickness / 2 * std::cos(theta + pi / 2);
    float y_thickness = thickness / 2 * std::sin(theta + pi / 2);

    sf::VertexArray shape(sf::Quads, 4);
    shape[0].position = sf::Vector2f(x_thickness,-y_thickness);
    shape[1].position = sf::Vector2f(x + x_thickness,- y - y_thickness);
    shape[2].position = sf::Vector2f(x - x_thickness,- y + y_thickness);
    shape[3].position = sf::Vector2f(-x_thickness,y_thickness);

    shape[0].color = sf::Color::Black;
    shape[1].color = sf::Color::Black;
    shape[2].color = sf::Color::Black;
    shape[3].color = sf::Color::Black;

    window.draw(shape);
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

void Canvas::draw_highlighted_note(const Rhythm &rhythm)
{
    float theta0 = make_time_line_angle(rhythm);
    float pi = 2 * std::acos(0.0f);
    while (theta0 >= 2 * pi)
    { theta0 -= 2 * pi; }

    unsigned i = 0;
    while (make_ith_note_angle(rhythm, i) < theta0)
    {
        ++i;
    }
    if (i > 0)
    { --i; }

    float ratio = view.getSize().x / size_x;
    float radius = 40 * ratio;

    float theta = make_ith_note_angle(rhythm, i);
    theta = pi/2 - theta;
    float x = std::cos(theta);
    float y = -std::sin(theta);

    sf::CircleShape disk(radius);
    disk.move(x - radius, y - radius);
    disk.setFillColor(sf::Color::Green);

    window.draw(disk);
}

float Canvas::make_ith_note_angle(const Rhythm &rhythm,
                                 unsigned i)
{
    float pi = 2 * std::acos(0.0f);
    return 2 * pi
         * boost::rational_cast<float>(rhythm[i].timing)
         / float(rhythm.nb_beats);
}

void Canvas::draw_ith_note(const Rhythm& rhythm,
                           unsigned i)
{
    float ratio = view.getSize().x / size_x;
    float radius = 30 * ratio;

    float pi = 2 * std::acos(0.0f);
    float theta = make_ith_note_angle(rhythm, i);
    theta = pi/2 - theta;
    float x = std::cos(theta);
    float y = -std::sin(theta);

    sf::CircleShape disk(radius);
    disk.move(x - radius, y - radius);
    disk.setFillColor(rhythm[i].accented ? sf::Color::Red : sf::Color::Blue);

    window.draw(disk);
}
