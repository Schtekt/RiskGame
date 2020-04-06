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
	text.setPosition(100, 100);
	text.setString(string);
	box.setSize(sf::Vector2f((float)(string.size()*20 + 4), (float)36));
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
	box.setSize(sf::Vector2f((float)(string.size() * 20 + 4), (float)36));
}

void Button::setFont(sf::Font *font)
{
	text.setFont(*font);
}

std::string Button::getString() const
{
	return text.getString();
}

sf::Vector2f Button::getPos() const
{
	return box.getPosition();
}

sf::Vector2f Button::getSize() const
{
	return box.getSize();
}
