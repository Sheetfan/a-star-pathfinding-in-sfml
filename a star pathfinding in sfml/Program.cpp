#include "Program.h"
#include <iostream>
namespace buzi {
	Program::Program(int row, int column, int size) {
		window = new sf::RenderWindow(sf::VideoMode(column * size, row * size), "a* pathfinding", sf::Style::Close);
		grid = new Grid(window,row,column,size);
	}

	void Program::handleEvent() {
		sf::Event e;
		while (window->pollEvent(e)) {
			if (e.type == e.Closed) {
				window->close();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (sf::Keyboard::
				isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed
				(sf::Keyboard::RControl))) {
				grid->placeEndTile();
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				grid->placeStartTile();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				grid->placebObstacle();
			}

			if (e.type == e.MouseButtonReleased) {
				grid->aStart();

			}
					
		}
	}
	void Program::update(){
		grid->update();
	}

	void Program::draw(){
		window->clear();
		grid->draw();
		window->display();
	}
	void Program::run(){
		while (window->isOpen()) {
			handleEvent();
			update();
			draw();
		}
	}
}