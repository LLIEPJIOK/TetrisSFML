#include <SFML/Graphics.hpp>
#include "Tetris.h"

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	Tetris tetris;
	sf::RenderWindow window(sf::VideoMode(750, 700), "Tetris");

	sf::Clock clock;
	float time = 0.0f;

	while (window.isOpen())
	{
		time += clock.restart().asSeconds();
		sf::Event e{};
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (e.key.code)
				{
				case sf::Keyboard::A:
				case sf::Keyboard::Left:
					tetris.set_horizontal_offset(-1);
					break;
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
					tetris.set_horizontal_offset(1);
					break;
				case sf::Keyboard::Q:
					tetris.rotate_left();
					break;
				case sf::Keyboard::W:
				case sf::Keyboard::Up:
					tetris.rotate_right();
					break;
				case sf::Keyboard::S:
				case sf::Keyboard::Down:
					tetris.move_down();
					break;
				case sf::Keyboard::Space:
					tetris.drop();
					break;
				case sf::Keyboard::Enter:
					tetris.create_new_game();
					break;
				default:
					break;
				}

				break;
			default:
				break;
			}
		}

		tetris.move_sideways();
		tetris.fall_down(time);
		window.clear(sf::Color::White);
		tetris.draw(window);
		window.display();
	}
}
