#include "Canvas.h"
#include <cmath>


void Canvas::display_rhythm(const Rhythm_set& rhythm_set)
{
	for (unsigned i = 0; i < rhythm_set.size(); ++i)
	{ next_note_indexes.push_back(0); }

	hit_buffers = std::vector<sf::SoundBuffer>(rhythm_set.size());
	hit_players = std::vector<sf::Sound>(rhythm_set.size());

	for (unsigned i = 0; i < rhythm_set.size(); ++i)
	{
		if (!rhythm_set[i].hit_sound_file.empty())
		{
			std::string sounds = "../data/sounds/";
			hit_buffers[i].loadFromFile(sounds + rhythm_set[i].hit_sound_file);
			hit_players[i].setBuffer(hit_buffers[i]);
		}
	}


	open();
	setup_view();
	while (window.isOpen())
	{
		handle_events();
		if (timer.read() * rhythm_set.bpm / 60 > rhythm_set.nb_beats)
		{ timer.reset(); }
		play_hit_sounds(rhythm_set);
		draw_rhythms(rhythm_set);
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
			for (unsigned& next_note_index : next_note_indexes)
			{ next_note_index = 0; }
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

void Canvas::draw_rhythms(const Rhythm_set& rhythms)
{
	window.clear(sf::Color::White);
	draw_center_circles(rhythms);
	draw_time_line(rhythms);
	//draw_beat_lines(rhythm);

	draw_polygons(rhythms);
	draw_notes(rhythms);
}

float Canvas::make_time_line_angle(const Rhythm_set& rhythm_set)
{
	float pi = 2 * std::acos(0.0f);
	return 2 * pi
		   * timer.read()
		   * rhythm_set.bpm / 60
		   / float(rhythm_set.nb_beats);
}

void Canvas::draw_center_circles(const Rhythm_set& rhythm_set)
{
	for (unsigned i = 0; i < rhythm_set.size(); ++i)
	{
		float thickness = 0.03;
		float R = (1 - float(i) / float(rhythm_set.size()))
				  + thickness / 2;
		float r = (1 - float(i) / float(rhythm_set.size()))
				  - thickness / 2;
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

void Canvas::draw_time_line(const Rhythm_set& rhythm_set)
{
	float pi = 2 * std::acos(0.0f);
	float theta = make_time_line_angle(rhythm_set);
	theta = pi / 2 - theta;
	float y = std::sin(theta);
	float x = std::cos(theta);

	float ratio = view.getSize().x / size_x;
	float thickness = 3 * ratio;
	float x_thickness = thickness / 2 * std::cos(theta + pi / 2);
	float y_thickness = thickness / 2 * std::sin(theta + pi / 2);

	sf::VertexArray shape(sf::Quads, 4);
	shape[0].position = sf::Vector2f(x_thickness, -y_thickness);
	shape[1].position = sf::Vector2f(x + x_thickness, -y - y_thickness);
	shape[2].position = sf::Vector2f(x - x_thickness, -y + y_thickness);
	shape[3].position = sf::Vector2f(-x_thickness, y_thickness);

	shape[0].color = sf::Color::Black;
	shape[1].color = sf::Color::Black;
	shape[2].color = sf::Color::Black;
	shape[3].color = sf::Color::Black;

	window.draw(shape);
}

void Canvas::draw_beat_lines(const Rhythm_set& rhythm_set)
{
	float pi = 2 * std::acos(0.0f);

	for (unsigned i = 0; i < rhythm_set.nb_beats; ++i)
	{
		float theta = float(i) * 2 * pi / float(rhythm_set.nb_beats);
		theta = pi / 2 - theta;
		float y = std::sin(theta);
		float x = std::cos(theta);

		sf::VertexArray shape(sf::LineStrip, 2);
		shape[0].position = sf::Vector2f(0, 0);
		shape[1].position = sf::Vector2f(x, -y);

		shape[0].color = sf::Color::Black;
		shape[1].color = sf::Color::Black;

		window.draw(shape);
	}
}

void Canvas::draw_polygons(const Rhythm_set& rhythm_set)
{
	for (unsigned i = 0; i < rhythm_set.size(); ++i)
	{
		if (!rhythm_set[i].draw_polygon)
		{ continue; }

		float thickness = 0.02;
		sf::VertexArray polygon(sf::TriangleStrip,
								2 * (rhythm_set[i].notes.size() + 1));
		float r = float(i + 1) / float(rhythm_set.size())
				  - thickness / 2;
		float R = float(i + 1) / float(rhythm_set.size())
				  + thickness / 2;

		for (unsigned j = 0; j < rhythm_set[i].notes.size(); ++j)
		{
			float pi = 2 * std::acos(0.0f);
			float theta = make_ijth_note_angle(rhythm_set, i, j);
			theta = pi / 2 - theta;

			float x_int = r * std::cos(theta);
			float x_ext = R * std::cos(theta);
			float y_int = -r * std::sin(theta);
			float y_ext = -R * std::sin(theta);

			polygon[2 * j].position = sf::Vector2f(x_ext, y_ext);
			polygon[2 * j + 1].position = sf::Vector2f(x_int, y_int);
			polygon[2 * j].color = sf::Color::Magenta;
			polygon[2 * j + 1].color = sf::Color::Magenta;
		}
		float pi = 2 * std::acos(0.0f);
		float theta = make_ijth_note_angle(rhythm_set, i, 0);
		theta = pi / 2 - theta;

		float x_int = r * std::cos(theta);
		float x_ext = R * std::cos(theta);
		float y_int = -r * std::sin(theta);
		float y_ext = -R * std::sin(theta);

		polygon[2 * rhythm_set[i].notes.size()].position =
				sf::Vector2f(x_ext, y_ext);
		polygon[2 * rhythm_set[i].notes.size() + 1].position =
				sf::Vector2f(x_int, y_int);
		polygon[2 * rhythm_set[i].notes.size()].color = sf::Color::Magenta;
		polygon[2 * rhythm_set[i].notes.size() + 1].color = sf::Color::Magenta;

		window.draw(polygon);
	}
}

void Canvas::draw_notes(const Rhythm_set& rhythm_set)
{
	draw_highlighted_notes(rhythm_set);
	for (unsigned i = 0; i < rhythm_set.size(); ++i)
	{
		for (unsigned j = 0; j < rhythm_set[i].notes.size(); ++j)
		{
			draw_ijth_note(rhythm_set, i, j);
		}
	}
}

void Canvas::draw_highlighted_notes(const Rhythm_set& rhythm_set)
{
	for (unsigned i = 0; i < rhythm_set.size(); ++i)
	{
		float theta0 = make_time_line_angle(rhythm_set);

		unsigned j = 0;
		while (make_ijth_note_angle(rhythm_set, i, j) < theta0)
		{ ++j; }
		if (j > 0)
		{ --j; }

		float scale = 1.2;
		float radius = (rhythm_set[i][j].accented ?
						big_circle_radius * scale :
						small_circle_radius + (scale - 1) * big_circle_radius);

		float pi = 2 * std::acos(0);
		float theta = make_ijth_note_angle(rhythm_set, i, j);
		theta = pi / 2 - theta;
		float x = float(i + 1) / float(rhythm_set.size())
				  * std::cos(theta);
		float y = -float(i + 1) / float(rhythm_set.size())
				  * std::sin(theta);

		sf::CircleShape disk(radius);
		disk.move(x - radius, y - radius);
		disk.setFillColor(sf::Color::Green);

		window.draw(disk);
	}
}

float Canvas::make_ijth_note_angle(const Rhythm_set& rhythm_set,
								   unsigned i,
								   unsigned j)
{
	float pi = 2 * std::acos(0.0f);
	return 2 * pi
		   * boost::rational_cast<float>(rhythm_set[i][j].timing)
		   / float(rhythm_set.nb_beats);
}

void Canvas::draw_ijth_note(const Rhythm_set& rhythm_set,
							unsigned i,
							unsigned j)
{
	float radius = (rhythm_set[i][j].accented ?
					big_circle_radius : small_circle_radius);

	float pi = 2 * std::acos(0.0f);
	float theta = make_ijth_note_angle(rhythm_set, i, j);
	theta = pi / 2 - theta;
	float x = float(i + 1) / float(rhythm_set.size()) * std::cos(theta);
	float y = -float(i + 1) / float(rhythm_set.size()) * std::sin(theta);

	sf::CircleShape disk(radius);
	disk.move(x - radius, y - radius);
	disk.setFillColor(rhythm_set[i][j].accented ?
					  sf::Color::Red : sf::Color::Black);

	window.draw(disk);
}

void Canvas::play_hit_sounds(const Rhythm_set& rhythm_set)
{
	if (state == STOPPED)
	{ return; }

	double t = timer.read() * rhythm_set.bpm / 60.0f;
	for (unsigned i = 0; i < rhythm_set.size(); ++i)
	{
		if (rhythm_set[i].hit_sound_file.empty())
		{ continue; }

		bool need_play_sound;
		if (next_note_indexes[i] == 0)
		{ need_play_sound = t < 0.01; }
		else
		{
			need_play_sound = t > boost::rational_cast<double>(
					rhythm_set[i][next_note_indexes[i]].timing);
		}

		if (need_play_sound)
		{
			hit_players[i].play();
			next_note_indexes[i]++;
			if (next_note_indexes[i] == rhythm_set[i].notes.size())
			{
				next_note_indexes[i] = 0;
			}
		}
	}
}