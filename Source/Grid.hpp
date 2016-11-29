/**
	File: "Grid.hpp"

	Copyright (c) 2016 Michael K. Duncan (fruitless75@gmail.com)
	Distributed under the MIT License (MIT) (See accompanying file LICENSE
	or copy at http://opensource.org/licenses/MIT)
**/

#ifndef Grid_H
#define Grid_H

#include <cstdint>
#include <vector>

/*
	Grid class stores game state data, including grid data and
	grid dimensions.
*/
class Grid
{
	public:
		std::vector<std::vector<uint8_t> > Cheat, Known;
		uint32_t Width, Height;

		Grid();
		~Grid();
		Grid(const Grid&);
		Grid& operator=(const Grid&);
		
		//Calculates the best possible move, given the current game state.
		void getBestMove(uint32_t&, uint32_t&) const;
};

#endif
