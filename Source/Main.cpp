/**
	File: "Main.cpp"

	Copyright (c) 2016 Michael K. Duncan (fruitless75@gmail.com)
	Distributed under the MIT License (MIT) (See accompanying file LICENSE
	or copy at http://opensource.org/licenses/MIT)
**/

#define MINEBOT_ENABLE_CHEATING 0

#include "Grid.hpp"
#include "Instance.hpp"
#include "Process.hpp"

#include <windows.h>

int main()
{
	Process instances;

	if(!instances.loadInstances())
		return -1;

	while(instances.updateInstances())
	{
		#if MINEBOT_ENABLE_CHEATING == 0
			if(!instances.solveInstances())
				return -2;
		#else
			if(!instances.cheatInstances())
				return -2;
		#endif

		else if(!instances.loadInstances())
			return -3;

		#if MINEBOT_ENABLE_CHEATING == 0
			Sleep(69);
		#else
			Sleep(1000);
		#endif
	}

	return 0;
}
