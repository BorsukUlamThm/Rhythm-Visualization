#include "Canvas.h"
#include <cmath>


Canvas::Canvas()
{
    if (!hit_buffer.loadFromFile("../data/sounds/hit.wav"))
        std::cerr << "Couldn't load hit sound file";
}

void Canvas::display_rhythm(const std::vector<Rhythm>& rhythms)
{
	open();
	setup_view();
	while (window.isOpen())
	{
		handle_events();
        if (timer.read() * rhythms[0].bpm / 60 > rhythms[0].nb_beats)
        {
           timer.reset();
        }
        play_sounds(rhythms[0]);
        draw_rhythms(rhythms);
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
            state = (state == RUNNING ? STOPPED : RUNNING);
            break;

        case sf::Keyboard::Enter:
            timer.stop();
            timer.reset();
            state = STOPPED;
            next_note_index = 0;
            break;

        case sf::Keyboard::A:
            sound.setBuffer(hit_buffer);
            sound.play();
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

void Canvas::draw_rhythms(const std::vector<Rhythm>& rhythms)
{
	window.clear(sf::Color::White);
    draw_center_circles(rhythms);
    draw_time_line(rhythms[0]);
    //draw_beat_lines(rhythm);
    draw_highlighted_notes(rhythms);

    for(unsigned i = 0; i < rhythms.size(); ++i)
    {
        for(unsigned j = 0; j < rhythms[i].notes.size(); ++j)
        {
            draw_ijth_note(rhythms, i, j);
        }
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

void Canvas::draw_center_circles(const std::vector<Rhythm>& rhythms)
{
    for(unsigned i = 0; i < rhythms.size(); ++i)
    {
        float thickness = 0.03;
        float R = (1 - float(i) / float(rhythms.size())) + thickness / 2;
        float r = (1 - float(i) / float(rhythms.size())) - thickness / 2;
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

void Canvas::draw_beat_lines(const Rhythm &rhythm)
{
    float pi = 2 * std::acos(0.0f);

    for(unsigned i = 0; i < rhythm.nb_beats; ++i)
    {
        float theta = float(i) * 2 * pi / float(rhythm.nb_beats);
        theta = pi / 2 - theta;
        float y = std::sin(theta);
        float x = std::cos(theta);

        sf::VertexArray shape(sf::LineStrip, 2);
        shape[0].position = sf::Vector2f(0,0);
        shape[1].position = sf::Vector2f(x,-y);

        shape[0].color = sf::Color::Black;
        shape[1].color = sf::Color::Black;

        window.draw(shape);
    }
}

void Canvas::draw_highlighted_notes(const std::vector<Rhythm>& rhythms)
{
    for (unsigned id = 0; id < rhythms.size(); ++id)
    {
        float theta0 = make_time_line_angle(rhythms[id]);

        unsigned i = 0;
        while (make_ith_note_angle(rhythms[id], i) < theta0)
        { ++i; }
        if (i > 0)
        { --i; }

        float scale = 1.2;
        float radius = (rhythms[id][i].accented ?
                        big_circle_radius * scale :
                        small_circle_radius + (scale - 1) * big_circle_radius);

        float pi = 2 * std::acos(0);
        float theta = make_ith_note_angle(rhythms[id], i);
        theta = pi/2 - theta;
        float x = float(id + 1) / float(rhythms.size()) * std::cos(theta);
        float y = -float(id + 1) / float(rhythms.size()) * std::sin(theta);

        sf::CircleShape disk(radius);
        disk.move(x - radius, y - radius);
        disk.setFillColor(sf::Color::Green);

        window.draw(disk);
    }
}

float Canvas::make_ith_note_angle(const Rhythm &rhythm,
                                 unsigned i)
{
    float pi = 2 * std::acos(0.0f);
    return 2 * pi
         * boost::rational_cast<float>(rhythm[i].timing)
         / float(rhythm.nb_beats);
}

void Canvas::draw_ijth_note(const std::vector<Rhythm>& rhythms,
                            unsigned i,
                            unsigned j)
{
    float radius = (rhythms[i][j].accented ?
            big_circle_radius : small_circle_radius);

    float pi = 2 * std::acos(0.0f);
    float theta = make_ith_note_angle(rhythms[i], j);
    theta = pi/2 - theta;
    float x = float(i + 1) / float(rhythms.size()) * std::cos(theta);
    float y = -float(i + 1) / float(rhythms.size()) * std::sin(theta);

    sf::CircleShape disk(radius);
    disk.move(x - radius, y - radius);
    disk.setFillColor(rhythms[i][j].accented ? sf::Color::Red : sf::Color::Black);

    window.draw(disk);
}

void Canvas::play_sounds(const Rhythm& rhythm)
{
    if (state == STOPPED)
    { return; }

    double t = timer.read() * rhythm.bpm / 60.0f;

    bool need_play_sound;
    if (next_note_index == 0)
    { need_play_sound = t < 0.01; }
    else
    {
        need_play_sound = t >
                boost::rational_cast<double>(rhythm[next_note_index].timing);
    }

    if (need_play_sound)
    {
        sound.setBuffer(hit_buffer);
        sound.play();
        next_note_index++;
        if (next_note_index == rhythm.notes.size())
        {
            next_note_index = 0;
        }
    }
}