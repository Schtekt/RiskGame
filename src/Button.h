#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include <string.h>

class Button {
private:
	sf::RectangleShape box;
	sf::Text text;

public:
	Button();
	Button(sf::Vector2f pos, std::string string);

	void render(sf::RenderWindow* window);

	bool isClicked(sf::Vector2i mousePos);

	void setPosition(sf::Vector2f pos);
	void setString(std::string string);
	void setFont(sf::Font *font);
	void setColor(sf::Color color);
	void setOutline(float size, sf::Color color);

	std::string getString() const;

	sf::Vector2f getPos() const;
	sf::Vector2f getSize() const;
};

#endif