#include "Grid.hpp"

#include <ctime>
#include <random>

static int8_t deltaAbs(const int8_t x, const int8_t y) { return ((x > y) ? (x - y) : (y - x)); }
static int8_t Max(const int8_t x, const int8_t y) { return ((x > y) ? x : y); }

Grid::Grid() : Width(0), Height(0)
{
	for(uint8_t i = 0; i < 32; ++i)
		for(uint8_t j = 0; j < 32; ++j)
		{
			Cheat[i][j] = 0;
			Known[i][j] = 9;
		}
}

Grid::~Grid()
{
}

Grid::Grid(const Grid &copy) : Width(copy.Width), Height(copy.Height)
{
	for(uint8_t i = 0; i < 32; ++i)
		for(uint8_t j = 0; j < 32; ++j)
		{
			Cheat[i][j] = copy.Cheat[i][j];
			Known[i][j] = copy.Known[i][j];
		}
}

Grid& Grid::operator=(const Grid &copy)
{
	for(uint8_t i = 0; i < 32; ++i)
		for(uint8_t j = 0; j < 32; ++j)
		{
			Cheat[i][j] = copy.Cheat[i][j];
			Known[i][j] = copy.Known[i][j];
		}

	Width = copy.Width;
	Height = copy.Height;

	return (*this);
}

bool Grid::isInside(const int8_t x, const int8_t y) const
{
	return ((x >= 0) && (x < Width) && (y >= 0) && (y < Height));
}

int8_t Grid::getKnown(const int8_t x, const int8_t y) const
{
	return (isInside(x, y) ? Known[x][y] : 13);
}

void Grid::setKnown(const int8_t x, const int8_t y, const int8_t value)
{
	if(isInside(x, y))
		Known[x][y] = value;
}

int8_t Grid::getNeighbors(const int8_t x, const int8_t y, const int8_t value) const
{
	int8_t count = 0;

	for(int8_t i = -1; i <= 1; ++i)
		for(int8_t j = -1; j <= 1; ++j)
			if(getKnown(x + i, y + j) == value)
				++count;

	return count;
}

bool Grid::isNeighbor(const int8_t x0, const int8_t y0, const int8_t x1, const int8_t y1) const
{
	return (Max(deltaAbs(x0, x1), deltaAbs(y0, y1)) <= 1);
}

bool Grid::solveSingle(uint32_t &x, uint32_t &y)
{
	return false;
}

bool Grid::solveDouble(uint32_t &x, uint32_t &y)
{
	return false;
}

void Grid::getBestMove(uint32_t &x, uint32_t &y)
{
	if(solveSingle(x, y))
		return;

	else if(solveDouble(x, y))
		return;

	std::mt19937 Generator(time(nullptr));

	do
	{
		x = Generator() % Width;
		Generator.discard(1);
		y = Generator() % Height;
		Generator.discard(1);
	}
	while(Known[x][y] != 9);
}
