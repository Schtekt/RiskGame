#include <SFML/Graphics.hpp>
#include "Game.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct GoMove
{
	bool player;
	unsigned int horisontal;
	unsigned int vertical;
};

void ReadGoMatch(const char* path, std::vector<GoMove>* match);
void ReadDirectory(const char* path, std::vector<std::vector<GoMove>>* match, std::string indentation);

int main()
{
	std::vector<std::vector<GoMove>> matches;
	ReadDirectory("../GoGames/", &matches, "");
    sf::RenderWindow window(sf::VideoMode(800, 800), "GO!");
    sf::RenderWindow window(sf::VideoMode(900, 800), "GO!");

	Game game;

	for (const auto& match : matches)
	{
		//if(match.size() > 1)
		game.addStone(match[0].horisontal, match[0].vertical);
	}
    while (window.isOpen())
    {

        window.clear();

		game.run(window);
		game.render(window);

        window.display();
    }

    return 0;
}
void ReadDirectory(const char* path, std::vector<std::vector<GoMove>>* match, std::string indentation)
{
	std::filesystem::path p(path);
	std::filesystem::directory_iterator start(p);
	for (const auto& entry : start)
	{
		std::string tmp = entry.path().string();
		if (entry.is_directory())
		{
			std::cout << indentation << "Reading in folder: " << entry.path() << std::endl;
			ReadDirectory(tmp.c_str(), match, indentation + "\t");
		}
		else
		{
			std::cout << indentation << "\tReading match file: " << entry.path() << std::endl;
			std::vector<GoMove> tmpMatch;
			ReadGoMatch(tmp.c_str(), &tmpMatch);
			match->push_back(tmpMatch);
		}
	}
}

void ReadGoMatch(const char* path, std::vector<GoMove>* match)
{
	std::fstream f;
	f.open(path);

	if (f.is_open())
	{
		std::stringstream ss;
		char junk;
		char line[1000];

		while (!f.eof())
		{
			f.getline(line, 1000, ']');
			ss << line;
			ss >> junk;
			if (junk == ';')
			{
				GoMove move;
				
				char player;
				char horisontal;
				char vertical;

				ss >> player >> junk >> horisontal >> vertical;

				move.player = player == 'B' ? true : false;
				move.horisontal = horisontal;
				move.horisontal -= 'a' + (move.horisontal > 'i');
				move.vertical = vertical;
				move.vertical -= 'a' + (move.horisontal > 'i');

				match->push_back(move);
			}
			ss.str("");
			ss.clear();
		}
	}
}