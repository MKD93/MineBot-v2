/**
	File: "Grid.cpp"

	Copyright (c) 2016 Michael K. Duncan (fruitless75@gmail.com)
	Distributed under the MIT License (MIT) (See accompanying file LICENSE
	or copy at http://opensource.org/licenses/MIT)
**/

#include "Grid.hpp"

#include <algorithm>
#include <chrono>
#include <random>
#include <utility>

static uint32_t getRandom(const uint32_t min, const uint32_t max)
{
	static uint32_t seed = static_cast<uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	static std::mt19937 generator(seed);

	generator.discard(1);
	return std::uniform_int_distribution<uint32_t>(min, max)(generator);
}

/*
	Grid matrices store values for each tile on the Minesweeper grid.
	The "Cheat" matrix stores the definite values of the Minesweeper grid.
	The "Known" matrix stores the observed values found while simulating gameplay.

	Values: 0 is empty tile; 1 though 8 are the numeric values on a tile;
		9 is unknown tile value; 10 is flagged tile; 11 is question tile;
		12 is internal flag; 13 is out-of-bounds.
*/

Grid::Grid() : Cheat(32, std::vector<uint8_t>(32, 0)), Known(32, std::vector<uint8_t>(32, 9)), Solve(), Width(0), Height(0) {}
Grid::~Grid() { Cheat.clear(); Known.clear(); Solve.clear(); }
Grid::Grid(const Grid &copy) : Cheat(copy.Cheat), Known(copy.Known), Solve(copy.Solve), Width(copy.Width), Height(copy.Height) {}
Grid& Grid::operator=(const Grid &copy) { Cheat = copy.Cheat; Known = copy.Known; Solve = copy.Solve; Width = copy.Width; Height = copy.Height; return (*this); }

/*
	Determines if a coordinate is within the grid.
*/
bool Grid::isWithinGrid(const int8_t x, const int8_t y) const
{
	return ((x >= 0) && (x < static_cast<int8_t>(Width)) && (y >= 0) && (y < static_cast<int8_t>(Height)));
}

/*
	Determines if two tiles are neighboring.
*/
bool Grid::isNeighboring(const int8_t x0, const int8_t y0, const int8_t x1, const int8_t y1) const
{
	return std::max(std::abs(x0 - x1), std::abs(y0 - y1)) <= 1;
}

/*
	Counts the number of neighboring tiles with a specific value.
*/
uint8_t Grid::countNeighbors(const int8_t x, const int8_t y, const uint8_t value) const
{
	uint8_t result = 0;

	for(int8_t j = -1; j <= 1; ++j)
		for(int8_t i = -1; i <= 1; ++i)
			if(getKnown(x + i, y + j) == value)
				++result;

	return result;
}

/*
	Returns a tile value from the "Solve" grid.
*/
uint8_t Grid::getKnown(const int8_t x, const int8_t y) const
{
	return (isWithinGrid(x, y) ? Solve[x][y] : 13);
}

/*
	Sets a tile value on the "Solve" grid.
*/
void Grid::setKnown(const int8_t x, const int8_t y, const uint8_t value)
{
	if(isWithinGrid(x, y))
		Solve[x][y] = value;
}

/*
	Solves one layer of neighbors analytically.
*/
bool Grid::solveSingles()
{
	bool changed = false;
	uint8_t tile = 9, flagged = 0, unopened = 0;

	for(int8_t y = 0; y < static_cast<int8_t>(Height); ++y)
		for(int8_t x = 0; x < static_cast<int8_t>(Width); ++x)
		{
			tile = getKnown(x, y);

			if((tile < 1) || (tile > 8))
				continue;

			flagged = countNeighbors(x, y, 10);
			unopened = countNeighbors(x, y, 9);

			if((tile == flagged) && (unopened >= 1))
			{
				for(int8_t j = -1; j <= 1; ++j)
					for(int8_t i = -1; i <= 1; ++i)
						if(getKnown(x + i, y + j) == 9)
							setKnown(x + i, y + j, 12);

				changed = true;
			}

			else if((tile == (flagged + unopened)) && (unopened >= 1))
			{
				for(int8_t j = -1; j <= 1; ++j)
					for(int8_t i = -1; i <= 1; ++i)
						if(getKnown(x + i, y + j) == 9)
							setKnown(x + i, y + j, 10);

				changed = true;
			}
		}

	return changed;
}

/*
	Solves two layer of neighbors analytically.
*/
bool Grid::solveDoubles()
{
	bool changed = false, jump = false;
	uint8_t tile = 9, neighborTile = 9;

	for(int8_t y = 0; y < static_cast<int8_t>(Height); ++y)
		for(int8_t x = 0; x < static_cast<int8_t>(Width); ++x)
		{
			tile = getKnown(x, y);

			if((tile < 1) || (tile > 8))
				continue;

			tile -= countNeighbors(x, y, 10);

			for(int8_t j = -1; j <= 1; ++j)
			{
				jump = false;

				for(int8_t i = -1; i <= 1; ++i)
				{
					neighborTile = getKnown(x + i, y + j);

					if(((i == 0) && (j == 0)) || (neighborTile < 1) || (neighborTile > 8))
						continue;

					neighborTile -= countNeighbors(x + i, y + j, 10);

					for(int8_t v = -1; v <= 1; ++v)
						for(int8_t u = -1; u <= 1; ++u)
							if((getKnown(x + u, y + v) == 9) && !isNeighboring(x + u, y + v, x + i, y + j))
								jump = true;

					if(jump)
						break;

					else if(tile == neighborTile)
					{
						for(int8_t v = -1; v <= 1; ++v)
							for(int8_t u = -1; u <= 1; ++u)
								if((getKnown(x + i + u, y + j + v) == 9) && !isNeighboring(x + i + u, y + j + v, x, y))
								{
									setKnown(x + i + u, y + j + v, 12);
									changed = true;
								}
					}

					else if((neighborTile - tile) == (countNeighbors(x + i, y + j, 9) - countNeighbors(x, y, 9)))
					{
						for(int8_t v = -1; v <= 1; ++v)
							for(int8_t u = -1; u <= 1; ++u)
								if((getKnown(x + i + u, y + j + v) == 9) && !isNeighboring(x + i + u, y + j + v, x, y))
								{
									setKnown(x + i + u, y + j + v, 10);
									changed = true;
								}
					}
				}
			}
		}

	return changed;
}

/*
	Calculates best move from probabilities for each tile being safe.
*/
bool Grid::computeCosts(uint32_t &x0, uint32_t &y0) const
{
	std::vector<std::pair<int8_t, int8_t>> moves;
	std::vector<int8_t> costs;
	int8_t minCost = 127;

	for(int8_t y = 0; y < static_cast<int8_t>(Height); ++y)
		for(int8_t x = 0; x < static_cast<int8_t>(Width); ++x)
			if(Solve[x][y] == 12)
			{
				moves.push_back(std::pair<int8_t, int8_t>(x, y));
				costs.push_back(127);

				for(int8_t j = -1; j <= 1; ++j)
					for(int8_t i = -1; i <= 1; ++i)
						if((getKnown(x + i, y + j) >= 1) && (getKnown(x + i, y + j) <= 8))
						{
							if(costs[costs.size() - 1] == 127)
								costs[costs.size() - 1] = 0;

							costs[costs.size() - 1] += getKnown(x + i, y + j);

							for(int8_t v = -1; v <= 1; ++v)
								for(int8_t u = -1; u <= 1; ++u)
									if((u != 0) && (v != 0) && (getKnown(x + i + u, y + j + v) >= 1) && (getKnown(x + i + u, y + j + v) <= 8))
										costs[costs.size() - 1] -= getKnown(x + i + u, y + j + v);
						}
			}

	for(uint32_t i = 0; i < costs.size(); ++i)
		if(costs[i] < minCost)
		{
			x0 = std::get<0>(moves[i]);
			y0 = std::get<1>(moves[i]);
			minCost = costs[i];
		}

	return (minCost != 127);
}

/*
	Calculates a random move that is always on an unknown tile.
*/
void Grid::getRandomMove(uint32_t &x0, uint32_t &y0) const
{
	std::vector<std::pair<int8_t, int8_t>> moves;
	std::vector<int8_t> costs;
	int8_t minCost = 127;

	for(int8_t y = 0; y < static_cast<int8_t>(Height); ++y)
		for(int8_t x = 0; x < static_cast<int8_t>(Width); ++x)
			if((Known[x][y] == 9) && (Solve[x][y] != 10))
			{
				moves.push_back(std::pair<int8_t, int8_t>(x, y));
				costs.push_back(127);

				for(int8_t j = -1; j <= 1; ++j)
					for(int8_t i = -1; i <= 1; ++i)
						if((getKnown(x + i, y + j) >= 1) && (getKnown(x + i, y + j) <= 8))
						{
							if(costs[costs.size() - 1] == 127)
								costs[costs.size() - 1] = 0;

							costs[costs.size() - 1] += getKnown(x + i, y + j);

							for(int8_t v = -1; v <= 1; ++v)
								for(int8_t u = -1; u <= 1; ++u)
									if((u != 0) && (v != 0) && (getKnown(x + i + u, y + j + v) >= 1) && (getKnown(x + i + u, y + j + v) <= 8))
										costs[costs.size() - 1] -= getKnown(x + i + u, y + j + v);
						}
			}

	for(uint32_t i = 0; i < costs.size(); ++i)
		if(costs[i] < minCost)
		{
			x0 = std::get<0>(moves[i]);
			y0 = std::get<1>(moves[i]);
			minCost = costs[i];
		}

	if(minCost == 127)
	{
		uint32_t index = 0;
		moves.clear();

		for(uint8_t y = 0; y < Height; ++y)
			for(uint8_t x = 0; x < Width; ++x)
				if((Known[x][y] == 9) && (Solve[x][y] != 10))
					moves.push_back(std::pair<uint8_t, uint8_t>(x, y));

		if(!moves.empty())
		{
			index = getRandom(0, moves.size() - 1);
			x0 = std::get<0>(moves[index]);
			y0 = std::get<1>(moves[index]);
		}
	}
}

/*
	Calculates the best possible move, given the current game state.
	If there is no known best possible move, then a move is selected randomly.
*/
void Grid::getBestMove(uint32_t &x0, uint32_t &y0)
{
	bool changed = false;

	if(Solve.empty())
		Solve = Known;

	while(solveSingles() || solveDoubles())
		changed = true;

	if(changed && !computeCosts(x0, y0))
		getRandomMove(x0, y0);

	else if(!changed)
		getRandomMove(x0, y0);
}
