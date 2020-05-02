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

int main()
{
	std::vector<GoMove> match;
	std::filesystem::path p("../GoGames/");
	std::filesystem::directory_iterator start(p);

	for (const auto& entry : start)
	{
		std::cout << entry.path() << std::endl;
		std::string tmp = entry.path().string();
		ReadGoMatch(tmp.c_str(), &match);
	}
	return 0;
}

void ReadGoMatch(const char* path, std::vector<GoMove>* match)
{
	std::fstream f;
	f.open(path);

	if (f.is_open())
	{
		std::stringstream ss;
		char junk;
		char line[128];

		while (!f.eof())
		{
			f.getline(line, 128);
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