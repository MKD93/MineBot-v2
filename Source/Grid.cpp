/**
	File: "Grid.cpp"

	Copyright (c) 2016 Michael K. Duncan (fruitless75@gmail.com)
	Distributed under the MIT License (MIT) (See accompanying file LICENSE
	or copy at http://opensource.org/licenses/MIT)
**/

#include "Grid.hpp"

#include <chrono>
#include <random>

Grid::Grid() : Cheat(32, std::vector<uint8_t>(32, 0)), Known(32, std::vector<uint8_t>(32, 9)), Width(0), Height(0) {}
Grid::~Grid() { Cheat.clear(); Known.clear(); }
Grid::Grid(const Grid &copy) : Cheat(copy.Cheat), Known(copy.Known), Width(copy.Width), Height(copy.Height) {}
Grid& Grid::operator=(const Grid &copy) { Cheat = copy.Cheat; Known = copy.Known; Width = copy.Width; Height = copy.Height; return (*this); }

/*
	Calculates the best possible move, given the current game state.
	**Note: currently uses random guesses
*/
void Grid::getBestMove(uint32_t &x, uint32_t &y) const
{
	std::mt19937 generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	do
	{
		x = generator() % Width;
		generator.discard(1);
		y = generator() % Height;
		generator.discard(1);
	}
	while(Known[x][y] != 9);
}
