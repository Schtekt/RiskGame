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
void PrintToCSV(const char* path, std::vector<std::vector<GoMove>>* matches);

int main()
{
	std::vector<std::vector<GoMove>> matches;
	ReadDirectory("../GoGames/", &matches, "");
	PrintToCSV("../tmp.csv", &matches);
    sf::RenderWindow window(sf::VideoMode(900, 800), "GO!");

	Game game;
	for (unsigned int i = 0; i < 10; i++)
	{
		for (const auto& match : matches)
		{
			if (match.size() > i)
			{
				game.addStone(match[i].horisontal, match[i].vertical, !match[i].player, i/2);
			}
		}
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
				move.horisontal -= 'a';
				move.vertical = vertical;
				move.vertical -= 'a';

				match->push_back(move);
			}
			ss.str("");
			ss.clear();
		}
	}
}

void PrintToCSV(const char* path, std::vector<std::vector<GoMove>>* matches)
{
	std::ofstream file;
	file.open(path, std::ios::out | std::ios::trunc);
	if (file.is_open())
	{
		file.clear();
		
		for (auto& match : *matches)
		{
			for (auto& move : match)
			{
				file << (move.player ? 'b' : 'w') << ";" << move.horisontal << ";" << move.vertical << ";";
			}
			file << std::endl;
		}
	}
}