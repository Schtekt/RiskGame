#include "Menu.h"
#include "Button.h"

Button::Button()
{
	box.setSize(sf::Vector2f(100, 100));
	text.setFillColor(sf::Color::Black);
}

Button::Button(sf::Vector2f pos, std::string string)
{
	box.setPosition(pos);
	box.setSize(sf::Vector2f(100,100));
	text.setPosition(100, 100);
	text.setString(string);
}

void Button::render(sf::RenderWindow* window)
{
	window->draw(box);
	window->draw(text);
}

bool Button::isClicked(sf::Vector2i mousePos)
{
	if (box.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
		return true;
	}
	return false;
}

void Button::setPosition(sf::Vector2f pos)
{
	box.setPosition(pos);
	text.setPosition(pos);
}

void Button::setString(std::string string)
{
	text.setString(string);
}

void Button::setFont(sf::Font *font)
{
	text.setFont(*font);
}
