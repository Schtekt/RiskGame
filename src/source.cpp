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
struct node
{
	int movesMadeOnThis;
	std::vector<node> nextMoves;
};

void ReadGoMatch(const char* path, std::vector<GoMove>* match);
void ReadDirectory(const char* path, std::vector<std::vector<GoMove>>* match, std::string indentation);
void PrintToCSV(const char* path, std::vector<std::vector<GoMove>>* matches);
void PrintAllHeatMaps(const char* folder, std::vector<std::vector<GoMove>>* matches, int nrOfMoves = 0);
void PrintHeatMapsToCSV(const char* folder, const char* csvFileName, const char* gpFileName, std::vector<std::vector<GoMove>>* matches, bool playerBlack, bool blackWin, int nrOfMoves = 0);
void PrintAllQuadrantBars(const char* folder, std::vector<std::vector<GoMove>>* matches, int nrOfMoves = 0);
int PrintQuadrantBarsToCSV(const char* folder, const char* csvFileName, std::vector<std::vector<GoMove>>* matches, bool playerBlack, bool blackWin, int nrOfMoves = 0);
void PrintHistogramGP(const char* folder, const char* csvFileName1, const char* csvFileName2, const char* gpFileName, bool playerBlack, int nrOfMoves, int nrOfMatches);
void PrintAllMovesInQuadrants(const char* folder, std::vector<std::vector<GoMove>>* matches, int nrOfMoves = 0);
int PrintMovesInQuadrantsToCSV(const char* folder, const char* csvFileName, std::vector<std::vector<GoMove>>* matches, bool playerBlack, bool blackWin, int nrOfMoves);
void PrintTreeGP(const char* folder, const char* csvFileName, const char* gpFileName, bool playerBlack, bool blackWin, std::vector<std::vector<GoMove>>* matches, int nrOfMoves, int nrOfMatches);
void PrintTreeDOT(const char* folder, const char* dotFileName, node* root);

int main()
{
	std::vector<std::vector<GoMove>> matches;
	ReadDirectory("../GoGames/", &matches, "");
	PrintToCSV("../tmp.csv", &matches);
	PrintAllHeatMaps("../heatmaps/", &matches, 5);
	PrintAllQuadrantBars("../BarGraphs/", &matches, 5);
	PrintAllMovesInQuadrants("../Tree/", &matches, 5);
    sf::RenderWindow window(sf::VideoMode(900, 800), "GO!");

	Game game;
	for (unsigned int i = 0; i < 1; i++)
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

	int nrOfMatches;

	nrOfMatches = PrintQuadrantBarsToCSV(folder, "BlackWin.csv", matches, true, true, nrOfMoves);
	nrOfMatches += PrintQuadrantBarsToCSV(folder, "BlackLose.csv", matches, true, false, nrOfMoves);
	PrintQuadrantBarsToCSV(folder, "WhiteWin.csv", matches, false, false, nrOfMoves);
	PrintQuadrantBarsToCSV(folder, "WhiteLose.csv", matches, false, true, nrOfMoves);
	PrintHistogramGP(folder, "BlackWin.csv", "BlackLose.csv", "HistogramBlack.gp", true, nrOfMoves, nrOfMatches);
	PrintHistogramGP(folder, "WhiteWin.csv", "WhiteLose.csv", "HistogramWhite.gp", false, nrOfMoves, nrOfMatches);
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

int PrintQuadrantBarsToCSV(const char* folder, const char* csvFileName, std::vector<std::vector<GoMove>>* matches, bool playerBlack, bool blackWin, int nrOfMoves)
{
	std::ofstream csvFile;

	char csvPath[64];
	strcpy(csvPath, folder);
	strcat(csvPath, csvFileName);

	csvFile.open(csvPath, std::ios::out | std::ios::trunc);

	int quadrants[4] = { 0 };
	std::string quadtrantNames[] = { "Top Left", "Top Right", "Bottom Left", "Bottom Right" };
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

		//csvFile << ", Top Left, Top Right, Bottom Left, Bottom Right" << std::endl;
		for (int i = 0; i < 4; ++i)
		{
			csvFile << quadtrantNames[i] << ", " << quadrants[i] << std::endl;
		}
	}

	return nrOfMatches;
}

void PrintHistogramGP(const char * folder, const char * csvFileName1, const char * csvFileName2, const char * gpFileName, bool playerBlack, int nrOfMoves, int nrOfMatches)
{
	std::ofstream gpFile;

	char gpPath[64];
	strcpy(gpPath, folder);
	strcat(gpPath, gpFileName);

	gpFile.open(gpPath, std::ios::out | std::ios::trunc);

	gpFile <<
		"set terminal wxt size 1200, 600 background rgb \'#202030\'" << std::endl <<
		"set xrange[-0.5:3.5]" << std::endl <<
		"set yrange[0:350]" << std::endl <<
		"set title \"Number of stones placed in each quadrant during " << (nrOfMoves == 0 ? "all" : "the first " + std::to_string(nrOfMoves)) << " moves of "<< nrOfMatches << " games of Go by " << (playerBlack ? "Black" : "White") << " player\" font \",16\" tc \"white\"" << std::endl <<
		"set xlabel \"Quadrant\" tc \"white\"" << std::endl <<
		"set ylabel \"Stones\" tc \"white\"" << std::endl <<
		"set xtic 1 tc \"white\"" << std::endl <<
		"set ytic 10 tc \"white\"" << std::endl <<
		"set grid lc \"white\"" << std::endl <<
		"set boxwidth 0.5" << std::endl <<
		"set key tc \"white\"" << std::endl <<
		"fileWin = \"" << csvFileName1 << "\"" << std::endl <<
		"fileLose = \"" << csvFileName2 << "\"" << std::endl <<
		"set datafile separator comma" << std::endl <<
		"set style data histogram" << std::endl <<
		"set style histogram cluster" << std::endl <<
		"set style fill solid 0.5" << std::endl <<
		"plot fileWin using 2:xtic(1) title \"Stones placed during winning game\" lc \"green\", fileLose using 2 title \"Stones placed during losing game\" lc \"red\"" << std::endl;
}

void PrintAllMovesInQuadrants(const char* folder, std::vector<std::vector<GoMove>>* matches, int nrOfMoves)
{
	mkdir(folder);

	int nrOfMatches;

	nrOfMatches = PrintMovesInQuadrantsToCSV(folder, "BlackWin.csv", matches, true, true, nrOfMoves);
	nrOfMatches += PrintMovesInQuadrantsToCSV(folder, "BlackLose.csv", matches, true, false, nrOfMoves);
	PrintMovesInQuadrantsToCSV(folder, "WhiteWin.csv", matches, false, false, nrOfMoves);
	PrintMovesInQuadrantsToCSV(folder, "WhiteLose.csv", matches, false, true, nrOfMoves);

	PrintTreeGP(folder, "BlackWin.csv", "BlackWin.gp", true, true, matches, nrOfMoves, nrOfMatches);

	system("dot -Tsvg ..\\Tree\\BlackWin.dot -o..\\Tree\\BlackWin.svg");
	system("dot -Tsvg ..\\Tree\\BlackLose.dot -o..\\Tree\\BlackLose.svg");
	system("dot -Tsvg ..\\Tree\\WhiteWin.dot -o..\\Tree\\WhiteWin.svg");
	system("dot -Tsvg ..\\Tree\\WhiteLose.dot -o..\\Tree\\WhiteLose.svg");

}

void initMoves(node* move, int currLevel, int maxLevel)
{
	for (int i = 0; i < 4; i++)
	{
		node nextMove;
		nextMove.movesMadeOnThis = 0;
		move->nextMoves.push_back(nextMove);
	}

	if (currLevel++ < maxLevel)
	{
		for (int i = 0; i < 4; i++)
		{
			initMoves(&move->nextMoves[i], currLevel, maxLevel);
		}
	}
}

void printMoves(node* move, int currLevel, int maxLevel, std::stringstream* ss, int xPos)
{
	if (currLevel++ < maxLevel)
	{
		*ss << xPos << "," << (5*30 - currLevel * 30) << "," <<  move->movesMadeOnThis << std::endl;

		int size = move->nextMoves.size();
		for (int i = 0; i < size; i++)
		{
			printMoves(&move->nextMoves[i], currLevel, maxLevel, ss, xPos + (4098 / (pow(currLevel, maxLevel - 1))) * (i - 1.5));
		}
	}

}

int PrintMovesInQuadrantsToCSV(const char* folder, const char* csvFileName, std::vector<std::vector<GoMove>>* matches, bool playerBlack, bool blackWin, int nrOfMoves)
{
	std::ofstream csvFile;

	char csvPath[64];
	strcpy(csvPath, folder);
	strcat(csvPath, csvFileName);

	bool addMatch;
	int nrMove = 0;
	int nrOfMatches = 0;
	csvFile.open(csvPath, std::ios::out | std::ios::trunc);

	node root;
	initMoves(&root, 0, nrOfMoves);

	node* pMove;
	for (auto& match : *matches)
	{
		pMove = &root;
		nrMove = 0;
		addMatch = false;
		for (auto move : match)
		{
			if (move.player == playerBlack && move.winner == blackWin)
			{
				int hor = move.horisontal / 10;
				int vert = move.vertical / 10;

				pMove = &pMove->nextMoves[hor + vert * 2];
				pMove->movesMadeOnThis++;
				addMatch = true;
			}
			nrMove++;
			if (nrMove >= nrOfMoves * 2)
				break;
		}
		if (addMatch)
			nrOfMatches++;
	}

	std::string dotFileName = csvFileName;
	dotFileName = dotFileName.substr(0,dotFileName.find(".", 0));
	dotFileName += ".dot";

	PrintTreeDOT(folder, dotFileName.c_str(), &root);

	// Assuming we ONLY make 5 moves, the last row will hold 1024 points...
	if (csvFile.is_open())
	{
		std::stringstream ss;
		// middle, where the root is will be 1024 / 2 = 512
		for (int i = 0; i < 4; i++)
			printMoves(&root.nextMoves[i], 0, nrOfMoves, &ss, 16392*i);

		csvFile << ss.rdbuf();
	}
	return nrOfMatches;
}

void PrintTreeGP(const char* folder, const char* csvFileName, const char* gpFileName, bool playerBlack, bool blackWin, std::vector<std::vector<GoMove>>* matches, int nrOfMoves, int nrOfMatches)
{
	std::ofstream gpFile;

	char gpPath[64];
	strcpy(gpPath, folder);
	strcat(gpPath, gpFileName);

	gpFile.open(gpPath, std::ios::out | std::ios::trunc);

	gpFile <<
		"set terminal wxt size 1200, 600 background rgb \'#202030\'" << std::endl <<
		"set xrange[0:2048]" << std::endl <<
		"set yrange[0:200]" << std::endl <<
		"set title \"Number of stones placed in each quadrant during " << (nrOfMoves == 0 ? "all" : "the first " + std::to_string(nrOfMoves)) << " moves of " << nrOfMatches << " games of Go by " << (playerBlack ? "Black" : "White") << " player\" font \",16\" tc \"white\"" << std::endl <<
		"set xlabel \"Quadrant\" tc \"white\"" << std::endl <<
		"set ylabel \"Stones\" tc \"white\"" << std::endl <<
		"set xtic 1 tc \"white\"" << std::endl <<
		"set ytic 10 tc \"white\"" << std::endl <<
		"file = \"" << csvFileName << "\"" << std::endl <<
		"set datafile separator comma" << std::endl <<
		"plot file using 1:2:3 with points lt 1 pt 6 ps 3" << std::endl;
}

void PrintTreeDOTHelper(std::stringstream* ss, node* parent, const char* parentName)
{
	if (parent->nextMoves.size() > 0)
	{
		if ((parent->nextMoves[0].movesMadeOnThis > 0) || (parent->nextMoves[1].movesMadeOnThis > 0) || (parent->nextMoves[2].movesMadeOnThis > 0) || (parent->nextMoves[3].movesMadeOnThis > 0))
		{
			//*ss << parentName << " -> {";

			for (int i = 0; i < parent->nextMoves.size(); i++)
			{
				if (parent->nextMoves[i].movesMadeOnThis > 0)
				{
					*ss << parentName << " -> " << parentName << (char)('A' + i) << "[label = ";
					switch (i)
					{
						// top left
					case 0:
						*ss << "TL, color = red]" << std::endl;
						break;
						// top right
					case 1:
						*ss << "TR, color = green]" << std::endl;
						break;
						// bot left
					case 2:
						*ss << "BL, color = blue]" << std::endl;
						break;
						// bot right
					case 3:
						*ss << "BR, color = yellow]" << std::endl;
						break;
					}
				}
			}


			for (int i = 0; i < parent->nextMoves.size(); i++)
			{
				if (parent->nextMoves[i].movesMadeOnThis > 0)
				{
					*ss << parentName << (char)('A' + i) << "[ label = " << parent->nextMoves[i].movesMadeOnThis << ", color = ";
					
						switch (i)
						{
							// top left
						case 0:
							*ss << "red";
							break;
							// top right
						case 1:
							*ss << "green";
							break;
							// bot left
						case 2:
							*ss << "blue";
							break;
							// bot right
						case 3:
							*ss << "yellow";
							break;
						}
					*ss << "];" << std::endl;
				}
			}

			std::string tst = parentName;
			for (int i = 0; i < parent->nextMoves.size(); i++)
			{
				PrintTreeDOTHelper(ss, &parent->nextMoves[i], (tst + (char)('A' + i)).c_str());
			}
		}
	}
}

void PrintTreeDOT(const char* folder, const char* dotFileName, node* root)
{
	std::ofstream dotFile;

	char dotPath[64];
	strcpy(dotPath, folder);
	strcat(dotPath, dotFileName);

	dotFile.open(dotPath, std::ios::out | std::ios::trunc);

	if (dotFile.is_open())
	{
		dotFile <<
			"digraph sample {" << std::endl;
		std::stringstream ss;

		PrintTreeDOTHelper(&ss, root, "S");
		dotFile << ss.rdbuf();
		dotFile << "S [label = \"Start\"]" << std::endl;

		dotFile << "}";

	}
}