/**
	File: "Process.hpp"

	Copyright (c) 2016 Michael K. Duncan (fruitless75@gmail.com)
	Distributed under the MIT License (MIT) (See accompanying file LICENSE
	or copy at http://opensource.org/licenses/MIT)
**/

#ifndef Process_H
#define Process_H

#include "Instance.hpp"

/*
	Process class manages active instances of Minesweeper tasks by providing
	methods that load and update data associated with each active game instances.
	Methods that solve and "cheat" on active instances are also provided.
*/
class Process
{
	public:
		std::vector<Instance> Instances;

		Process();
		~Process();
		Process(const Process&);
		Process& operator=(const Process&);
		
		//Loads all current active Minesweeper instances by storing relevant process data.
		bool loadInstances();

		//Updates the game state for each active Minesweeper instance
		bool updateInstances();

		//Solves each active Minesweeper instance
		bool solveInstances();

		//"Cheats" on each active Minesweeper instance
		bool cheatInstances();
};

#endif
