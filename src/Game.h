#include <SFML/Graphics.hpp>

class Game {
private:

	class Stone {
	public:
		sf::CircleShape stone;
		int nrOfStones = 0;
	};

	sf::RectangleShape background;
	sf::Texture texture;

	Stone stones[19][19];
	int highestStoneCount = 0;
	int nrOfStones = 0;

public:
	Game();

	void addStone(int x, int y);

	void render(sf::RenderWindow& window);

};