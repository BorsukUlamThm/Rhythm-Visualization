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

	std::vector<sf::SoundBuffer> hit_buffers;
	std::vector<sf::Sound> hit_players;
	std::vector<unsigned> next_note_indexes;

	bool background_music = false;
	sf::Music music;

public:
	Canvas() = default;
	void display_rhythm(const Rhythm_set& rhythm_set);

protected:
	void open();
	void setup_view();
	void handle_events();
	void handle_key_pressed_event(const sf::Event& event);

	void draw_rhythms(const Rhythm_set& rhythm_set);
	void draw_center_circles(const Rhythm_set& rhythm_set);
	void draw_time_line(const Rhythm_set& rhythm_set);
	void draw_beat_lines(const Rhythm_set& rhythm_set);
	void draw_polygons(const Rhythm_set& rhythm_set);
	void draw_polygon_edge(const sf::Vector2f& v1,
						   const sf::Vector2f& v2);
	void draw_notes(const Rhythm_set& rhythm_set);
	void draw_highlighted_notes(const Rhythm_set& rhythm_set);
	void draw_ijth_note(const Rhythm_set& rhythm_set,
						unsigned i,
						unsigned j);

	float make_time_line_angle(const Rhythm_set& rhythm_set);
	float make_ijth_note_angle(const Rhythm_set& rhythm_set,
							   unsigned i,
							   unsigned j);

	void play_hit_sounds(const Rhythm_set& rhythm_set);
};

