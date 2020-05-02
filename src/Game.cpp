#include "Game.h"

Game::Game()
{
	background.setFillColor(sf::Color::Yellow);
	background.setSize(sf::Vector2f(800, 800));
	background.setPosition(sf::Vector2f(0, 0));
	texture.loadFromFile("../assets/board.png");
	background.setTexture(&texture);

	int radius = 18;

	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			stones[j][i].stone.setRadius(radius);
			stones[j][i].stone.setPosition(sf::Vector2f(22 + j * 42 - radius, 22 + i * 42 - radius));
			stones[j][i].stone.setFillColor(sf::Color(255, 255, 255, 0));
		}
	}

	/*addStone(15, 15);
	addStone(15, 15);
	addStone(15, 15);

	addStone(1, 1);
	addStone(1, 1);
	addStone(1, 1);
	addStone(1, 1);
	addStone(1, 1);
	addStone(1, 1);

	addStone(5, 5);
	addStone(5, 5);
	addStone(5, 5);

	addStone(4, 4);
	addStone(4, 4);
	addStone(4, 4);

	addStone(3, 3);*/
}

void Game::addStone(int x, int y)
{
	Stone* temp = &stones[x][y];

	temp->nrOfStones++;
	nrOfStones++;
	if (temp->nrOfStones > highestStoneCount) {
		highestStoneCount = temp->nrOfStones;

		for (int i = 0; i < 19; i++) {
			for (int j = 0; j < 19; j++) {
				if(stones[j][i].nrOfStones > 0)
					stones[j][i].stone.setFillColor(sf::Color(0, 0, 0, 255 / (nrOfStones / stones[j][i].nrOfStones)));
			}
		}
	}

	temp->stone.setFillColor(sf::Color(0, 0, 0, 255 / (nrOfStones / temp->nrOfStones)));
}

void Game::render(sf::RenderWindow& window)
{
	window.draw(background);
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			window.draw(stones[j][i].stone);
		}
	}
}
