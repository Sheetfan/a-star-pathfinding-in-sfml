#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
namespace buzi {

	class Grid {
		struct Tile : public sf::RectangleShape {
			int row; //which row is the tile in (y)
			int column;//which column is the tile in (x)
			
			bool bVisited = false;
			bool bObstacle = false;

			float fGlobalGoal = 0.0f;
			float fLocalGoal = 0.f;

			std::vector<Tile*> neighbours;
			Tile* parent = nullptr;
		};
		sf::RenderWindow* window;
		int row;//total rows
		int column;//total columns
		int tileSize = 0.f; //each tile size

		std::vector <Tile*> tiles;

		Tile* tileStart = nullptr;
		Tile* tileEnd = nullptr;
	public:
		Grid(sf::RenderWindow* window,int row, int column, int size);

		void placeStartTile();
		void placeEndTile();
		void placebObstacle();
		void eraseObstacle();
		bool aStart();

		void update();

		void draw();
	};

}

