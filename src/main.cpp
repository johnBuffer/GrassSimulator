#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>
#include "solver.hpp"
#include "grass.hpp"
#include "tree.hpp"
#include "wind.hpp"


int main()
{
	srand(static_cast<uint32_t>(time(0)));

    constexpr uint32_t WinWidth = 1920;
	constexpr uint32_t WinHeight = 1080;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Tree", sf::Style::Fullscreen, settings);
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(144);

	float time = 0.0f;
	bool wind = false;
	float wind_x = 0.0f;
	float wind_speed = 7.0f;
	float wind_width = 200.0f;
	float wind_force = 50.0f;

	std::vector<Grass> grass;
	std::vector<Wind> winds(4);

	Solver solver;
	sf::VertexArray va(sf::Quads);

	for (float x(WinWidth * 0.0f); x < WinWidth; x += 1.0f) {
		grass.push_back(Grass::add(solver, x, WinHeight + 120.0f));
	}

	for (float x(WinWidth*0.0f); x < WinWidth; x += 100.0f) {
		Tree::add(solver, x, WinHeight);
	}

	bool force = false;

	while (window.isOpen())
	{
		auto mouse_pos = sf::Mouse::getPosition(window);

		time += 0.008f;
        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				else {
					wind = !wind;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				force = false;
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				force = true;
			}
		}

		if (force) {
			solver.applyForce(150.0f, static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y));
		}

		if (wind) {
			for (Wind& w : winds) {
				w.apply(solver);
				w.update(WinWidth);
			}
		}

		solver.update();

		va.clear();
		for (Grass& g : grass) {
			g.addToVa(va);
		}

		window.clear(sf::Color::Black);

		//solver.render(window);
		window.draw(va);

        window.display();
    }

    return 0;
}
