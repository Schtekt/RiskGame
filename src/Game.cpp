#include "Game.h"

sf::Color Game::interpolateColor(float source, float target, sf::Color sourceColour, sf::Color targetColour)
{
	return sf::Color(sourceColour.r * source / target + targetColour.r * (target-source) / target, sourceColour.g * source / target + targetColour.g * (target - source) / target, sourceColour.b * source / target + targetColour.b * (target - source) / target, sourceColour.a * source / target + targetColour.a * (target - source) / target);
}

sf::Color Game::getColorHeatMap(float high, float curr)
{
	float part = high / 6;
	if (curr <= high && curr > high * 5 / 6) {
		return interpolateColor(curr-part*5, part, sf::Color(255, 0, 0, 255), sf::Color(255,255,255,255));
	}
	else if(curr <= high * 5 / 6 && curr > high * 4 / 6) {
		return interpolateColor(curr - part * 4, part, sf::Color(255, 255, 0, 255), sf::Color(255, 0, 0, 255));
	}
	else if (curr <= high * 4 / 6 && curr > high * 3 / 6) {
		return interpolateColor(curr - part * 3, part, sf::Color(0, 255, 0, 255), sf::Color(255, 255, 0, 255));
	}
	else if (curr <= high * 3 / 6 && curr > high * 2 / 6) {
		return interpolateColor(curr - part * 3, part, sf::Color(0, 255, 255, 255), sf::Color(0, 255, 0, 255));
	}
	else if (curr <= high * 2 / 6 && curr > high / 6) {
		return interpolateColor(curr - part * 2, part, sf::Color(0, 255, 255, 255), sf::Color(0, 0, 255, 255));
	}
	else if (curr <= high / 6 && curr > 0) {
		return interpolateColor(curr - part * 4, part, sf::Color(0, 0, 255, 255), sf::Color(0, 0, 0, 255));
	}
}

Game::Game()
{
	background.setFillColor(sf::Color::Yellow);
	background.setSize(sf::Vector2f(800, 800));
	background.setPosition(sf::Vector2f(0, 0));
	texture.loadFromFile("../assets/board.png");
	background.setTexture(&texture);

	font.loadFromFile("../arial.ttf");

	turnCounterText.setFont(font);
	turnCounterText.setPosition(sf::Vector2f(840, 100));
	turnCounterText.setCharacterSize(40);
	turnCounterText.setString("1");

	plusCounter.setFont(font);
	plusCounter.setPosition(sf::Vector2f(815, 150));
	plusCounter.setCharacterSize(40);
	plusCounter.setString("+");

	minusCounter.setFont(font);
	minusCounter.setPosition(sf::Vector2f(865, 150));
	minusCounter.setCharacterSize(40);
	minusCounter.setString("-");

	allTurn.setFont(font);
	allTurn.setPosition(sf::Vector2f(810, 200));
	allTurn.setCharacterSize(40);
	allTurn.setString("ALL");

	black.setFont(font);
	black.setPosition(sf::Vector2f(810, 250));
	black.setCharacterSize(40);
	black.setString("B");

	white.setFont(font);
	white.setPosition(sf::Vector2f(810, 300));
	white.setCharacterSize(40);
	white.setString("W");

	for (int i = 0; i < 50; i++) {
		highestStoneCount[0][i] = 0;
		highestStoneCount[1][i] = 0;
		totalNrOfStones[0][i] = 0;
		totalNrOfStones[1][i] = 0;
	}

	int radius = 18;

	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			stones[j][i].stone.setRadius(radius);
			stones[j][i].stone.setPosition(sf::Vector2f(22 + j * 42 - radius, 22 + i * 42 - radius));
			stones[j][i].stone.setFillColor(sf::Color(255, 255, 255, 0));
		}
	}
}

void Game::addStone(int x, int y, int colour, int turn)
{
	Stone* temp = &stones[x][y];

	temp->nrOfStones[colour][turn]++;
	totalNrOfStones[colour][turn]++;
	if (temp->nrOfStones[colour][turn] > highestStoneCount[colour][turn]) {
		highestStoneCount[colour][turn] = temp->nrOfStones[colour][turn];
	}
}

void Game::run(sf::RenderWindow& window)
{
	sf::Event event;
	if (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

			if (mousePos.x > plusCounter.getPosition().x && mousePos.x < plusCounter.getPosition().x + plusCounter.getCharacterSize() && mousePos.y > plusCounter.getPosition().y && mousePos.y < plusCounter.getPosition().y + plusCounter.getCharacterSize()) {
				if (turnCounter < 50) {
					turnCounter++;
					turnCounterText.setString(std::to_string(turnCounter + 1));
					for (int i = 0; i < 19; i++) {
						for (int j = 0; j < 19; j++) {
							if (stones[j][i].nrOfStones[showColour][turnCounter] > 0)
								//stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 255 / (highestStoneCount[showColour][turnCounter] / stones[j][i].nrOfStones[showColour][turnCounter])));
								//stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 255 / (totalNrOfStones[showColour][turnCounter] / stones[j][i].nrOfStones[showColour][turnCounter])));
								stones[j][i].stone.setFillColor(getColorHeatMap(highestStoneCount[showColour][turnCounter], stones[j][i].nrOfStones[showColour][turnCounter]));
							else
								stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 0));
						}
					}
				}
			}
			else if (mousePos.x > minusCounter.getPosition().x && mousePos.x < minusCounter.getPosition().x + minusCounter.getCharacterSize() && mousePos.y > minusCounter.getPosition().y && mousePos.y < minusCounter.getPosition().y + minusCounter.getCharacterSize()) {
				if (turnCounter > 0) {
					turnCounter--;
					turnCounterText.setString(std::to_string(turnCounter + 1));
					for (int i = 0; i < 19; i++) {
						for (int j = 0; j < 19; j++) {
							if (stones[j][i].nrOfStones[showColour][turnCounter] > 0)
								//stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 255 / (highestStoneCount[showColour][turnCounter] / stones[j][i].nrOfStones[showColour][turnCounter])));
								//stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 255 / (totalNrOfStones[showColour][turnCounter] / stones[j][i].nrOfStones[showColour][turnCounter])));
								stones[j][i].stone.setFillColor(getColorHeatMap(highestStoneCount[showColour][turnCounter], stones[j][i].nrOfStones[showColour][turnCounter]));
							else
								stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 0));
						}
					}
				}
			}
			else if (mousePos.x > allTurn.getPosition().x && mousePos.x < allTurn.getPosition().x + allTurn.getCharacterSize() && mousePos.y > allTurn.getPosition().y && mousePos.y < allTurn.getPosition().y + allTurn.getCharacterSize()) {
				int highest = 0;
				for (int i = 0; i < 19; i++) {
					for (int j = 0; j < 19; j++) {
						int count = 0;
						for (int k = 0; k < 50; k++) {
							count += stones[j][i].nrOfStones[showColour][k];
						}
						if (count > highest) {
							highest = count;
						}
					}
				}
				for (int i = 0; i < 19; i++) {
					for (int j = 0; j < 19; j++) {
						int count = 0;
						for (int k = 0; k < 50; k++) {
							count += stones[j][i].nrOfStones[showColour][k];
						}
						if (count > 0)
							//stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 255 / (highest / count)));
							stones[j][i].stone.setFillColor(getColorHeatMap(highest, count));
						else
							stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 0));
					}
				}
			}
			else if (mousePos.x > black.getPosition().x && mousePos.x < black.getPosition().x + black.getCharacterSize() && mousePos.y > black.getPosition().y && mousePos.y < black.getPosition().y + black.getCharacterSize()) {
				showColour = 0;
				for (int i = 0; i < 19; i++) {
					for (int j = 0; j < 19; j++) {
						if (stones[j][i].nrOfStones[showColour][turnCounter] > 0)
							//stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 255 / (highestStoneCount[showColour][turnCounter] / stones[j][i].nrOfStones[showColour][turnCounter])));
							//stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 255 / (totalNrOfStones[showColour][turnCounter] / stones[j][i].nrOfStones[showColour][turnCounter])));
							stones[j][i].stone.setFillColor(getColorHeatMap(highestStoneCount[showColour][turnCounter], stones[j][i].nrOfStones[showColour][turnCounter]));
						else
							stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 0));
					}
				}
			}
			else if (mousePos.x > white.getPosition().x && mousePos.x < white.getPosition().x + white.getCharacterSize() && mousePos.y > white.getPosition().y && mousePos.y < white.getPosition().y + white.getCharacterSize()) {
				showColour = 1;
				for (int i = 0; i < 19; i++) {
					for (int j = 0; j < 19; j++) {
						if (stones[j][i].nrOfStones[showColour][turnCounter] > 0)
							//stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 255 / (highestStoneCount[showColour][turnCounter] / stones[j][i].nrOfStones[showColour][turnCounter])));
							//stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 255 / (totalNrOfStones[showColour][turnCounter] / stones[j][i].nrOfStones[showColour][turnCounter])));
							stones[j][i].stone.setFillColor(getColorHeatMap(highestStoneCount[showColour][turnCounter], stones[j][i].nrOfStones[showColour][turnCounter]));
						else
							stones[j][i].stone.setFillColor(sf::Color(255 * showColour, 255 * showColour, 255 * showColour, 0));
					}
				}
			}
		}
	}
}

void Game::render(sf::RenderWindow& window)
{
	window.draw(background);
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			window.draw(stones[j][i].stone);
		}
	}

	window.draw(turnCounterText);
	window.draw(plusCounter);
	window.draw(minusCounter);
	window.draw(allTurn);
	window.draw(black);
	window.draw(white);
}
