#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.h"

namespace buzi {
	class Program {
		sf::RenderWindow* window = nullptr;
		Grid* grid = nullptr;
	public:
		Program(int row, int column,int size);

		void handleEvent();

		void update();

		void draw();

		void run();


	};
}


	
