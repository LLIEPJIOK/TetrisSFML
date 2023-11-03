#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

class Tetris
{
public:
	Tetris();
	~Tetris() = default;

	void create_new_game();

	void set_horizontal_offset(const int& dx);
	void move_sideways();
	bool move_down();
	void fall_down(float& time);
	void drop();

	void rotate_left();
	void rotate_right();

	void draw(sf::RenderWindow& window);

private:

	bool is_possible_move(std::vector<sf::Vector2i>& cur_figure) const;

	void initialize_figure();

	void check_filled_rows();
	void clear_row(const int& row);

	void add_score(const int& number_of_lines);

	void clear_field();

	/*
	 * Template for figures:
	 *
	 *     |0 1|
	 *     |2 3|
	 *     |4 5|
	 *     |6 7|
	 */

	int figures_[7][4]
	{
		{1, 3, 5, 4}, // J
		{0, 2, 4, 6}, // I
		{0, 1, 2, 3}, // O
		{0, 2, 4, 5}, // L
		{1, 3, 2, 4}, // Z
		{0, 2, 4, 3}, // T
		{0, 2, 3, 5}, // S
	};

	const int cell_width_;
	const int width_, height_;

	sf::Texture texture_cells_;
	sf::Sprite cell_sprite_;
	sf::Font font_;
	sf::Text next_figure_text_, score_text_, game_over_text_, record_text_;

	std::vector<sf::Vector2i> cur_figure_, figure_move_, next_figure_;

	int cur_type_, next_type_;
	int cur_color_, next_color_;

	int dx_;

	int score_, record_;

	float delta_time_;

	bool is_game_over_;

	std::vector<std::vector<int>> field_;

	std::fstream stream_;
};
