#include "Grid.hpp"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <random>

Grid::Grid() : Cheat(32, std::vector<uint8_t>(32, 0)), Known(32, std::vector<uint8_t>(32, 9)), Width(0), Height(0) {}
Grid::~Grid() { Cheat.clear(); Known.clear(); }
Grid::Grid(const Grid &copy) : Cheat(copy.Cheat), Known(copy.Known), Width(copy.Width), Height(copy.Height) {}
Grid& Grid::operator=(const Grid &copy) { Cheat = copy.Cheat; Known = copy.Known; Width = copy.Width; Height = copy.Height; return (*this); }

bool Grid::isInside(const int32_t x, const int32_t y) const
{
	return ((x >= 0) && (x < Width) && (y >= 0) && (y < Height));
}

uint32_t Grid::getNeighbors(const int32_t x, const int32_t y, const uint8_t value) const
{
	uint32_t neighbors = 0;

	for(int32_t i = -1; i <= 1; ++i)
		for(int32_t j = -1; j <= 1; ++j)
			if(((i != 0 && j != 0) || (i != j)) && isInside(x + i, y + j) && (Known[x + i][y + j] == value))
				++neighbors;

	return neighbors;
}

void Grid::getBestMove(uint32_t &x, uint32_t &y) const
{
	std::mt19937 generator(time(nullptr));

	do
	{
		x = generator() % Width;
		generator.discard(1);
		y = generator() % Height;
		generator.discard(1);
	}
	while(Known[x][y] != 9);
}
