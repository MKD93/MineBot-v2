/**
	File: "Instance.hpp"

	Copyright (c) 2016 Michael K. Duncan (fruitless75@gmail.com)
	Distributed under the MIT License (MIT) (See accompanying file LICENSE
	or copy at http://opensource.org/licenses/MIT)
**/

#ifndef Instance_H
#define Instance_H

#include "Grid.hpp"

#include <windows.h>
#include <psapi.h>

/*
	Instance class stores data related to a single active instance of
	a Minesweeper process; including process related data and a
	representation of the game state.
*/
class Instance
{
		static HWND IdHandle;
		static BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);

	public:
		Grid Data;
		uint32_t Id;

		Instance();
		~Instance();
		Instance(const uint32_t);
		Instance(const Instance&);
		Instance& operator=(const Instance&);

		//Updates the game state for a single active Minesweeper instance
		bool updateGrid();

		//Solves a single active Minesweeper instance
		bool solveGrid();

		//"Cheats" on a single Minesweeper instance
		bool cheatGrid();
};

#endif
