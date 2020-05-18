#include <SFML/Graphics.hpp>
#include "Game.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <direct.h>

struct GoMove
{
	bool player;
	unsigned int horisontal;
	unsigned int vertical;
	bool winner;
};

void ReadGoMatch(const char* path, std::vector<GoMove>* match);
void ReadDirectory(const char* path, std::vector<std::vector<GoMove>>* match, std::string indentation);
void PrintToCSV(const char* path, std::vector<std::vector<GoMove>>* matches);
void PrintAllHeatMaps(const char* folder, std::vector<std::vector<GoMove>>* matches, int nrOfMoves = 0);
void PrintHeatMapsToCSV(const char* folder, const char* csvFileName, const char* gpFileName, std::vector<std::vector<GoMove>>* matches, bool playerBlack, bool blackWin, int nrOfMoves = 0);
void PrintAllQuadrantBars(const char* folder, std::vector<std::vector<GoMove>>* matches, int nrOfMoves = 0);
void PrintQuadrantBarsToCSV(const char* folder, const char* csvFileName, const char* gpFileName, std::vector<std::vector<GoMove>>* matches, bool playerBlack, bool blackWin, int nrOfMoves = 0);

int main()
{
	std::vector<std::vector<GoMove>> matches;
	ReadDirectory("../GoGames/", &matches, "");
	PrintToCSV("../tmp.csv", &matches);
	PrintAllHeatMaps("../heatmaps/", &matches, 5);
	PrintAllQuadrantBars("../BarGraphs/", &matches, 5);
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
		char winner;

		while (!f.eof())
		{
			f.getline(line, 1000, ']');
			ss << line;
			ss >> junk;
			if (junk == 'R')
			{
				ss >> junk;
				if (junk == 'E')
				{
					ss >> junk >> winner;
				}
			}
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
				move.winner = winner == 'B' ? true : false;

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

void PrintAllHeatMaps(const char* folder, std::vector<std::vector<GoMove>>* matches, int nrOfMoves)
{
	mkdir(folder);

	PrintHeatMapsToCSV(folder, "BlackWin.csv", "heatmapBlackWin.gp", matches, true, true, nrOfMoves);
	PrintHeatMapsToCSV(folder, "BlackLose.csv", "heatmapBlackLose.gp", matches, true, false, nrOfMoves);
	PrintHeatMapsToCSV(folder, "WhiteWin.csv", "heatmapWhiteWin.gp", matches, false, false, nrOfMoves);
	PrintHeatMapsToCSV(folder, "WhiteLose.csv", "heatmapWhiteLose.gp", matches, false, true, nrOfMoves);
}

void PrintAllQuadrantBars(const char* folder, std::vector<std::vector<GoMove>>* matches, int nrOfMoves)
{
	mkdir(folder);

	PrintQuadrantBarsToCSV(folder, "BlackWin.csv", "BarsBlackWin.gp", matches, true, true, nrOfMoves);
	PrintQuadrantBarsToCSV(folder, "BlackLose.csv", "BarsBlackLose.gp", matches, true, false, nrOfMoves);
	PrintQuadrantBarsToCSV(folder, "WhiteWin.csv", "BarsWhiteWin.gp", matches, false, false, nrOfMoves);
	PrintQuadrantBarsToCSV(folder, "WhiteLose.csv", "BarsWhiteLose.gp", matches, false, true, nrOfMoves);
}

void PrintHeatMapsToCSV(const char* folder, const char* csvFileName, const char* gpFileName, std::vector<std::vector<GoMove>>* matches, bool playerBlack, bool blackWin, int nrOfMoves)
{
	std::ofstream csvFile, gpFile;

	char csvPath[64], gpPath[64];
	strcpy(csvPath, folder);
	strcat(csvPath, csvFileName);
	strcpy(gpPath, folder);
	strcat(gpPath, gpFileName);

	csvFile.open(csvPath, std::ios::out | std::ios::trunc);
	gpFile.open(gpPath, std::ios::out | std::ios::trunc);

	int stones[19][19];

	for (int i = 0; i < 19; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			stones[i][j] = 0;
		}
	}

	int nrMove;
	int nrOfMatches = 0;
	bool addMatch = false;

	for (auto& match : *matches)
	{
		nrMove = 0;
		addMatch = false;
		if (match.size() <= 1)
		{
			continue;
		}
		for (auto& move : match)
		{
			if (move.player == playerBlack && move.winner == blackWin)
			{
				++stones[move.horisontal][move.vertical];
				addMatch = true;
			}

			++nrMove;
			if (nrMove == (nrOfMoves * 2))
			{
				break;
			}
		}
		if (addMatch)
		{
			++nrOfMatches;
		}
	}

	int highestAmount = 0;
	if (csvFile.is_open())
	{
		csvFile.clear();

		csvFile << ", A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, R, S, T" << std::endl;

		for (int j = 1; j <= 19; ++j)
		{
			csvFile << j;
			for (int i = 0; i < 19; ++i)
			{
				csvFile << ", " << stones[i][19 - j];
				if (stones[i][19 - j] > highestAmount)
				{
					highestAmount = stones[i][19 - j];
				}
			}
			csvFile << std::endl;
		}
	}

	gpFile <<
		"set terminal wxt size 660, 600" << std::endl <<
		"set title \"Placement of " << (nrOfMoves == 0 ? "all" : "first " + std::to_string(nrOfMoves)) << " stones placed by " << (playerBlack ? "Black" : "White") << " player during " << nrOfMatches << (playerBlack == blackWin ? " winning" : " losing") << " matches of Go\"" << std::endl <<
		"unset key" << std::endl <<
		"set tic scale 0" << std::endl <<
		"set palette defined ( 0 \"white\", 1 \"" << (playerBlack == blackWin ? "green" : "red") << "\")" << std::endl <<
		"set cbrange [0:" << highestAmount << "]" << std::endl <<
		"set cblabel \"Number of stones\"" << std::endl <<
		"unset cbtics" << std::endl <<
		"set xrange [-0.5:18.5]" << std::endl <<
		"set yrange [-0.5:18.5]" << std::endl <<
		"set grid front linetype - 1" << std::endl <<
		"set view map" << std::endl <<
		"file = \"" << csvFileName << "\"" << std::endl <<
		"set datafile separator comma" << std::endl <<
		"plot file matrix rowheaders columnheaders using 1:2:3 with image" << std::endl;
}

void PrintQuadrantBarsToCSV(const char* folder, const char* csvFileName, const char* gpFileName, std::vector<std::vector<GoMove>>* matches, bool playerBlack, bool blackWin, int nrOfMoves)
{
	std::ofstream csvFile, gpFile;

	char csvPath[64], gpPath[64];
	strcpy(csvPath, folder);
	strcat(csvPath, csvFileName);
	strcpy(gpPath, folder);
	strcat(gpPath, gpFileName);

	csvFile.open(csvPath, std::ios::out | std::ios::trunc);
	gpFile.open(gpPath, std::ios::out | std::ios::trunc);

	int quadrants[4] = { 0 };

	int nrMove;
	int nrOfMatches = 0;
	bool addMatch;
	for (auto& match : *matches)
	{
		nrMove = 0;
		addMatch = false;
		if (match.size() <= 1)
		{
			continue;
		}
		for (auto& move : match)
		{
			if (move.player == playerBlack && move.winner == blackWin)
			{
				int hor = move.horisontal / 10;
				int vert = move.vertical / 10;
				quadrants[hor + vert *2]++;
				addMatch = true;
			}

			++nrMove;
			if (nrMove == (nrOfMoves * 2))
			{
				break;
			}
		}
		if (addMatch)
		{
			++nrOfMatches;
		}
	}

	if (csvFile.is_open())
	{
		csvFile.clear();

		csvFile << ", Top Left, Top Right, Bottom Left, Bottom Right" << std::endl;

		for (int j = 1; j <= 19; ++j)
		{
			csvFile << j;
			for (int i = 0; i < 4; ++i)
			{
				csvFile << ", " << quadrants[i];
			}
			csvFile << std::endl;
		}
	}

}