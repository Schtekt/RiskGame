#include "DraftPhase.h"

DraftPhase::DraftPhase(Game* game, Player* player): Phase(game, player)
{
	toDeploy = std::max((unsigned int)3, player->GetNrOfTerritories() / 3);
	// TODO, add check for continents.
}

void DraftPhase::run(sf::RenderWindow* window, sf::Event event)
{
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
		if (event.type == sf::Event::MouseButtonPressed)
		{

		}
	}
}

void DraftPhase::render(sf::RenderWindow* window)
{
}
