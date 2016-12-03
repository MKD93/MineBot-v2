/**
	File: "Grid.cpp"

	Copyright (c) 2016 Michael K. Duncan (fruitless75@gmail.com)
	Distributed under the MIT License (MIT) (See accompanying file LICENSE
	or copy at http://opensource.org/licenses/MIT)
**/

#include "Grid.hpp"

#include <chrono>
#include <random>
#include <utility>

static uint8_t getRandom(const uint8_t min, const uint8_t max)
{
	static std::mt19937 generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	generator.discard(1);
	return std::uniform_int_distribution<uint32_t>(min, max)(generator);
}

/*
	Grid matrices store values for each tile on the Minesweeper grid.
	The "Cheat" matrix stores the definite values of the Minesweeper grid.
	The "Known" matrix stores the observed values found while simulating gameplay.

	Values: 0 is empty tile; 1 though 8 are the numeric values on a tile;
		9 is unknown tile value; 10 is tile without mine; 11 is tile with mine.
*/

Grid::Grid() : Cheat(32, std::vector<uint8_t>(32, 0)), Known(32, std::vector<uint8_t>(32, 9)), Solve(32, std::vector<uint8_t>(32, 'o')), Width(0), Height(0) {}
Grid::~Grid() { Cheat.clear(); Known.clear(); Solve.clear(); }
Grid::Grid(const Grid &copy) : Cheat(copy.Cheat), Known(copy.Known), Solve(copy.Solve), Width(copy.Width), Height(copy.Height) {}
Grid& Grid::operator=(const Grid &copy) { Cheat = copy.Cheat; Known = copy.Known; Solve = copy.Solve; Width = copy.Width; Height = copy.Height; return (*this); }

/*
	Determines if a coordinate is within the grid.
*/
bool Grid::isWithinGrid(const int8_t x, const int8_t y) const
{
	return ((x >= 0) && (x < Width) && (y >= 0) && (y < Height));
}

/*
	Calculates a random move that is always on an unknown tile.
*/
void Grid::getRandomMove(uint32_t &x, uint32_t &y) const
{
	std::vector<std::pair<uint8_t, uint8_t>> moves;
	uint8_t index = 0;

	for(uint8_t y = 0; y < Height; ++y)
		for(uint8_t x = 0; x < Width; ++x)
			if(Known[x][y] == 9)
				moves.push_back(std::pair<uint8_t, uint8_t>(x, y));

	index = getRandom(0, moves.size() - 1);
	x = std::get<0>(moves[index]);
	y = std::get<1>(moves[index]);
}

/*
	Calculates the best possible move, given the current game state.
	If there is no known best possible move, then a move is selected randomly.

	**Note: current implementation is not completed and is based off of
		original MineBot solver
*/
void Grid::getBestMove(uint32_t &x, uint32_t &y)
{
	std::vector<std::pair<uint8_t, uint8_t>> moves;
	uint8_t count = 0;
	bool changed = false;

	for(uint8_t y = 0; y < Height; ++y)
		for(uint8_t x = 0; x < Width; ++x)
		{
			if(Known[x][y] == 0)
				Solve[x][y] = ' ';

			else if((Known[x][y] >= 1) && (Known[x][y] <= 8))
				Solve[x][y] = Known[x][y] + 48;

			else
				Solve[x][y] = 'o';
		}

	for(int8_t y = 0; y < Height; ++y)
	{
		for(int8_t x = 0; x < Width; ++x)
		{
			if(Solve[x][y] == '0')
				for(int8_t v = -1; v <= 1; ++v)
					for(int8_t u = -1; u <= 1; ++u)
						if(isWithinGrid(x + u, y + v) && (Solve[x + u][y + v] == 'o'))
							moves.push_back(std::pair<uint8_t, uint8_t>(x + v, y + v));

			for(int8_t v = -1; v <= 1; ++v)
				for(int8_t u = -1; u <= 1; ++u)
					if(isWithinGrid(x + u, y + v) && (Solve[x + u][y + v] == 'o'))
						++count;

			if(count == (Solve[x][y] - 48))
			{
				for(int8_t v = -1; v <= 1; ++v)
					for(int8_t u = -1; u <= 1; ++u)
						if(isWithinGrid(x + u, y + v) && (Solve[x + u][y + v] == 'o'))
						{
							Solve[x + u][y + v] = 'x';
							changed = true;

							for(int8_t j = -1; j <= 1; ++j)
								for(int8_t i = -1; i <= 1; ++i)
									if(isWithinGrid(x + u + i, y + v + j) && (Solve[x + u + i][y + v + j] >= '1') && (Solve[x + u + i][y + v + j] <= '9'))
										--Solve[x + u + i][y + v + j];
						}

                if(changed)
                {
                    x = -1;
                    y = 0;

                    break;
                }
			}

			count = 0;
			changed = false;
		}
	}

	if(!moves.empty())
	{
		count = getRandom(0, moves.size() - 1);
		x = std::get<0>(moves[count]);
		y = std::get<1>(moves[count]);
	}

	else
		getRandomMove(x, y);
}
