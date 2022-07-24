#include "Grid.h"
#include <iostream>
#include <list>

namespace buzi {
	Grid::Grid(sf::RenderWindow* window, int row, int column, int size) :window(window),
		row(row),column(column),tileSize(size) {
		for (int cRow = 0; cRow < row; cRow++) {
			for (int cColumn = 0; cColumn < column;cColumn++) {
				Tile tile;
				tile.row = cRow;
				tile.column = cColumn;
				tile.setSize(sf::Vector2f((float)size, (float)size));
				tile.setPosition(sf::Vector2f((float)cColumn * (float)size,(float)cRow * (float)size));
				tile.setFillColor(sf::Color::White);
				tile.setOutlineColor(sf::Color::Black);
				tile.setOutlineThickness(-5.f);
				tiles.push_back(new Tile(tile));
			}
		}

		for (int cRow = 0; cRow < row;cRow++) {
			for (int cColumn = 0; cColumn < column; cColumn++) {
				if (cRow > 0) {
					tiles[cRow * row + cColumn]->neighbours.push_back(tiles[(cRow - 1) * row + (cColumn + 0)]);
				}
				if (cRow < row - 1) {
					tiles[cRow * row + cColumn]->neighbours.push_back(tiles[(cRow + 1) * row + (cColumn + 0)]);
				}
				if (cColumn > 0) {
					tiles[cRow * row + cColumn]->neighbours.push_back(tiles[(cRow - 0) * row + (cColumn - 1)]);
				}
				if (cColumn < row - 1) {
					tiles[cRow * row + cColumn]->neighbours.push_back(tiles[(cRow - 0) * row + (cColumn + 1)]);
				}
			} 		
		}
		tileStart = tiles[((row * (row * 1 / 2))) + (column * 1 / 4)];
		tileEnd = tiles[((row * (row * 1 / 2))) + (column * 3 / 4)];
	}

	void Grid::placeStartTile(){
		sf::Vector2i mousePos = sf::Vector2i(sf::Mouse::getPosition(*window));
		sf::Vector2i tilePos = mousePos / tileSize;
		tileStart = tiles[tilePos.y * row + tilePos.x];
	}

	void Grid::placeEndTile() {
		sf::Vector2i mousePos = sf::Vector2i(sf::Mouse::getPosition(*window));
		sf::Vector2i tilePos = mousePos / tileSize;
		tileEnd = tiles[tilePos.y * row + tilePos.x];
	}
	void Grid::placebObstacle(){
		sf::Vector2i mousePos = sf::Vector2i(sf::Mouse::getPosition(*window));
		sf::Vector2i tilePos = mousePos / tileSize;
		tiles[tilePos.y * row + tilePos.x]->bObstacle = !tiles[tilePos.y * row
			+ tilePos.x]->bObstacle;

	}
	bool Grid::aStart() {
		for (int cRow = 0; cRow < row; cRow++) {
			for (int cColumn = 0; cColumn < column; cColumn++){
				tiles[cRow * row + cColumn]->bVisited = false;
				tiles[cRow * row + cColumn]->fGlobalGoal = INFINITY;
				tiles[cRow * row + cColumn]->fLocalGoal = INFINITY;
				tiles[cRow * row + cColumn]->parent = nullptr;
			}
		}
			
		auto distance = [](Tile* a, Tile* b)
		{
			return sqrtf((a->column - b->column) * (a->column - b->column) + 
				(a->row - b->row) * (a->row - b->row));
		};

		auto heuristic = [distance](Tile* a, Tile* b)
		{
			return distance(a, b);
		};

		Tile* currentTile = tileStart;
		tileStart->fLocalGoal = 0.0f;
		tileStart->fGlobalGoal = heuristic(tileStart, tileEnd);

		std::list<Tile*> listNotTestedTiles;
		listNotTestedTiles.push_back(tileStart);

		while (!listNotTestedTiles.empty() && currentTile != tileEnd){

			listNotTestedTiles.sort([](const Tile* lhs, const Tile* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

			while (!listNotTestedTiles.empty() && listNotTestedTiles.front()->bVisited)
				listNotTestedTiles.pop_front();

			if (listNotTestedTiles.empty())
				break;

			currentTile = listNotTestedTiles.front();
			currentTile->bVisited = true;

			for (auto nodeNeighbour : currentTile->neighbours){
				if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == false) {
					listNotTestedTiles.push_back(nodeNeighbour);
				}
					

				float fPossiblyLowerGoal = currentTile->fLocalGoal + distance(currentTile, nodeNeighbour);


				if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
				{
					nodeNeighbour->parent = currentTile;
					nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

					nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, tileEnd);
				}
			}
		}

		return true;
	}

	void Grid::update(){
		sf::Vector2i mousePos = sf::Vector2i(sf::Mouse::getPosition(*window));
		for (auto &i : tiles) {
			i->setFillColor(sf::Color::White);
		}
		if (tileEnd !=nullptr) {
			tileEnd->setFillColor(sf::Color::Yellow);
		}
		for (auto & i : tiles) {
			if (i->bObstacle) {
				i->setFillColor(sf::Color::Black);
			}
		}
		if (tileEnd != nullptr) {
			Tile* q = tileEnd;
			while (q->parent !=nullptr){
				q->parent->setFillColor(sf::Color(140, 140, 140));
				q = q->parent;
			}
		}
		if (tileStart != nullptr) {
			tileStart->setFillColor(sf::Color::Red);
		}
	}
	void Grid::draw(){
		for (auto &i: tiles) {
			window->draw(*i);
		}
	}

}