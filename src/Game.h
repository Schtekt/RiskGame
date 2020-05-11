#include <SFML/Graphics.hpp>

//0 for Black
//1 for White

class Game {
private:

	class Stone {
	public:
		Stone() {
			for (int i = 0; i < 50; i++) {
				nrOfStones[0][i] = 0;
				nrOfStones[1][i] = 0;
			}
		}
		sf::CircleShape stone;
		int nrOfStones[2][50];
	};

	sf::RectangleShape background;
	sf::Texture texture;

	Stone stones[19][19];
	int highestStoneCount[2][50];
	int totalNrOfStones[2][50];
	int turnCounter = 0;
	int showColour = 0;

	sf::Font font;
	sf::Text turnCounterText;
	sf::Text plusCounter;
	sf::Text minusCounter;
	sf::Text allTurn;
	sf::Text black;
	sf::Text white;

	sf::Color interpolateColor(float source, float target, sf::Color sourceColour, sf::Color targetColour);
	sf::Color getColorHeatMap(float high, float curr);
public:
	Game();
	void addStone(int x, int y, int colour, int turn);

	void run(sf::RenderWindow& window);
	void render(sf::RenderWindow& window);

};