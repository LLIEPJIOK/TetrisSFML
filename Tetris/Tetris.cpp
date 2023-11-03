#include "Tetris.h"

Tetris::Tetris() : cell_width_(32), width_(10), height_(20), field_(height_ + 2, std::vector<int>(width_ + 2, -1))
{
	texture_cells_.loadFromFile("../Resourses/cells.png");
	cell_sprite_ = sf::Sprite(texture_cells_);

	stream_.open("../Resourses/max_score.txt", std::ios_base::in);
	stream_ >> record_;
	stream_.close();

	cur_figure_.resize(4);
	figure_move_.resize(4);
	next_figure_.resize(4);

	cur_type_ = next_type_ = 0;
	cur_color_ = next_color_ = 0;
	score_ = 0;
	dx_ = 0;
	is_game_over_ = true;

	create_new_game();

	font_.loadFromFile("../Resourses/arial.ttf");
	next_figure_text_.setFont(font_);
	next_figure_text_.setFillColor(sf::Color::Black);
	next_figure_text_.setCharacterSize(24);
	next_figure_text_.setString("Next figure:");
	next_figure_text_.setPosition((width_ + 5) * cell_width_, cell_width_);

	score_text_.setFont(font_);
	score_text_.setFillColor(sf::Color::Black);
	score_text_.setCharacterSize(24);
	score_text_.setPosition((width_ + 4) * cell_width_, 13 * cell_width_);
	score_text_.setString("Score: " + std::to_string(score_));

	game_over_text_.setFont(font_);
	game_over_text_.setFillColor(sf::Color::Black);
	game_over_text_.setCharacterSize(24);
	game_over_text_.setPosition((width_ + 4) * cell_width_, 16 * cell_width_);
	game_over_text_.setString("Game over! Press enter to start new game");

	record_text_.setFont(font_);
	record_text_.setFillColor(sf::Color::Black);
	record_text_.setCharacterSize(24);
	record_text_.setPosition((width_ + 4) * cell_width_, 14 * cell_width_);
	record_text_.setString("Record: " + std::to_string(record_));

	delta_time_ = 0.5f;

	for (int i = 0; i < height_ + 2; ++i)
	{
		field_[i][0] = 7;
		field_[i][width_ + 1] = 7;
	}

	for (int i = 1; i < width_ + 1; ++i)
	{
		field_[0][i] = 7;
		field_[height_ + 1][i] = 7;
	}
}

void Tetris::create_new_game()
{
	if (is_game_over_)
	{
		clear_field();

		next_type_ = 0;
		cur_type_ = 0;

		initialize_figure();
		cur_color_ = rand() % 7;
	
		initialize_figure();
		next_color_ = rand() % 7;

		dx_ = 0;
		is_game_over_ = false;

		if (record_ < score_)
		{
			record_ = score_;
			record_text_.setString("Record: " + std::to_string(record_));
		}

		score_ = 0;
	}
}

void Tetris::set_horizontal_offset(const int& dx)
{
	dx_ = dx;
}

void Tetris::move_sideways()
{
	if (is_game_over_)
	{
		return;
	}

	if (dx_ != 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			figure_move_[i].x = cur_figure_[i].x + dx_;
			figure_move_[i].y = cur_figure_[i].y;
		}

		if (is_possible_move(figure_move_))
		{
			for (int i = 0; i < 4; ++i)
			{
				cur_figure_[i].x = figure_move_[i].x;
			}
		}
		else
		{
			for (int i = 0; i < 4; ++i)
			{
				figure_move_[i].x = cur_figure_[i].x;
			}
		}

		dx_ = 0;
	}
}

bool Tetris::move_down()
{
	if (is_game_over_)
	{
		return false;
	}

	for (int i = 0; i < 4; ++i)
	{
		figure_move_[i].x = cur_figure_[i].x;
		figure_move_[i].y = cur_figure_[i].y + 1;
	}

	if (is_possible_move(figure_move_))
	{
		for (int i = 0; i < 4; ++i)
		{
			cur_figure_[i].y = figure_move_[i].y;
		}

		return true;
	}

	for (const auto& i : cur_figure_)
	{
		field_[i.y][i.x] = cur_color_;
	}

	check_filled_rows();


	cur_color_ = next_color_;
	initialize_figure();

	next_color_ = rand() % 7;

	if (!is_possible_move(cur_figure_))
	{
		next_figure_ = cur_figure_;
		next_color_ = cur_color_;
		is_game_over_ = true;

		if (score_ > record_)
		{
			stream_.open("../Resourses/max_score.txt", std::ios_base::out);
			stream_ << score_;
			stream_.close();
		}
	}

	return false;
}

void Tetris::fall_down(float& time)
{
	if (time >= delta_time_)
	{
		time -= delta_time_;
		move_down();
	}
}

void Tetris::drop()
{
	while (move_down());
}

void Tetris::rotate_left()
{
	if (is_game_over_)
	{
		return;
	}

	// don't need to rotate O
	if (cur_type_ == 2)
	{
		return;
	}

	for (int i = 0; i < 4; ++i)
	{
		const int x = cur_figure_[i].x - cur_figure_[1].x;
		const int y = cur_figure_[i].y - cur_figure_[1].y;
		figure_move_[i].x = cur_figure_[1].x + y;
		figure_move_[i].y = cur_figure_[1].y - x;
	}

	if (is_possible_move(figure_move_))
	{
		for (int i = 0; i < 4; ++i)
		{
			cur_figure_[i].x = figure_move_[i].x;
			cur_figure_[i].y = figure_move_[i].y;
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			figure_move_[i].x = cur_figure_[i].x;
			figure_move_[i].y = cur_figure_[i].y;
		}
	}
}

void Tetris::rotate_right()
{
	if (is_game_over_)
	{
		return;
	}

	// don't need to rotate O
	if (cur_type_ == 2)
	{
		return;
	}

	for (int i = 0; i < 4; ++i)
	{
		const int x = cur_figure_[i].x - cur_figure_[1].x;
		const int y = cur_figure_[i].y - cur_figure_[1].y;
		figure_move_[i].x = cur_figure_[1].x - y;
		figure_move_[i].y = cur_figure_[1].y + x;
	}

	if (is_possible_move(figure_move_))
	{
		for (int i = 0; i < 4; ++i)
		{
			cur_figure_[i].x = figure_move_[i].x;
			cur_figure_[i].y = figure_move_[i].y;
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			figure_move_[i].x = cur_figure_[i].x;
			figure_move_[i].y = cur_figure_[i].y;
		}
	}
}

void Tetris::draw(sf::RenderWindow& window)
{
	// drawing field
	for (int i = 0; i < height_ + 2; ++i)
	{
		for (int j = 0; j < width_ + 2; ++j)
		{
			if (field_[i][j] != -1)
			{
				cell_sprite_.setTextureRect(sf::IntRect(field_[i][j] % 4 * cell_width_, field_[i][j] / 4 * cell_width_,
				                                        cell_width_, cell_width_));
				cell_sprite_.setPosition(j * cell_width_, i * cell_width_);
				window.draw(cell_sprite_);
			}
		}
	}

	// drawing next figure
	window.draw(next_figure_text_);
	cell_sprite_.setTextureRect(sf::IntRect(3 * cell_width_, 1 * cell_width_, cell_width_, cell_width_));
	for (int i = 1; i <= 6; ++i)
	{
		cell_sprite_.setPosition((width_ + i + 3) * cell_width_, 2 * cell_width_);
		window.draw(cell_sprite_);

		cell_sprite_.setPosition((width_ + i + 3) * cell_width_, 9 * cell_width_);
		window.draw(cell_sprite_);
	}

	for (int j = 2; j <= 7; ++j)
	{
		cell_sprite_.setPosition((width_ + 4) * cell_width_, (1 + j) * cell_width_);
		window.draw(cell_sprite_);

		cell_sprite_.setPosition((width_ + 9) * cell_width_, (1 + j) * cell_width_);
		window.draw(cell_sprite_);
	}

	cell_sprite_.setTextureRect(sf::IntRect(next_color_ % 4 * cell_width_, next_color_ / 4 * cell_width_,
	                                        cell_width_, cell_width_));
	for (const auto& i : next_figure_)
	{
		cell_sprite_.setPosition((i.x + width_ + 1) * cell_width_, (i.y + 3) * cell_width_);
		window.draw(cell_sprite_);
	}

	// drawing score
	window.draw(score_text_);
	window.draw(record_text_);

	if (is_game_over_)
	{
		// drawing label "game over"
		window.draw(game_over_text_);
	}
	else
	{
		// drawing current figure
		cell_sprite_.setTextureRect(sf::IntRect(cur_color_ % 4 * cell_width_, cur_color_ / 4 * cell_width_,
		                                                 cell_width_, cell_width_));
		for (const auto& i : cur_figure_)
		{
			cell_sprite_.setPosition(i.x * cell_width_, i.y * cell_width_);
			window.draw(cell_sprite_);
		}
	}
}

bool Tetris::is_possible_move(std::vector<sf::Vector2i>& cur_figure) const
{
	return std::all_of(cur_figure.begin(), cur_figure.end(), [&](auto& i)
	{
		return field_[i.y][i.x] == -1;
	});
}

void Tetris::initialize_figure()
{
	cur_figure_ = next_figure_;
	cur_type_ = next_type_;
	next_type_ = rand() % 7;
	for (int i = 0; i < 4; ++i)
	{
		next_figure_[i].x = (figures_[next_type_][i] & 1) + width_ / 2;
		next_figure_[i].y = figures_[next_type_][i] / 2 + 1;
	}
}

void Tetris::check_filled_rows()
{
	int number_of_lines = 0;
	int first_line = -1;
	for (int i = height_; i > 0;)
	{
		if (i + 4 < first_line)
		{
			break;
		}

		if (std::count(field_[i].begin(), field_[i].end(), -1) == 0)
		{
			clear_row(i);
			if (first_line == -1)
			{
				first_line = i;
			}

			++number_of_lines;
		}
		else
		{
			--i;
		}
	}

	if (number_of_lines != 0)
	{
		add_score(number_of_lines);
	}
}

void Tetris::clear_row(const int& row)
{
	for (int i = row; i > 1; --i)
	{
		field_[i] = field_[i - 1];
	}

	for (int i = 1; i < width_ + 1; ++i)
	{
		field_[1][i] = -1;
	}
}

void Tetris::add_score(const int& number_of_lines)
{
	switch (number_of_lines)
	{
	case 1:
		score_ += 100;
		break;
	case 2:
		score_ += 300;
		break;
	case 3:
		score_ += 700;
		break;
	case 4:
		score_ += 1500;
		break;
	default:
		throw std::out_of_range("number of lines is out of range");
	}

	score_text_.setString("Score:" + std::to_string(score_));
}

void Tetris::clear_field()
{
	for (int i = 1; i < height_ + 1; ++i)
	{
		for (int j = 1; j < width_ + 1; ++j)
		{
			field_[i][j] = -1;
		}
	}
}
