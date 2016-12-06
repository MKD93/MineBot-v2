/**
	File: "Main.cpp"

	Copyright (c) 2016 Michael K. Duncan (fruitless75@gmail.com)
	Distributed under the MIT License (MIT) (See accompanying file LICENSE
	or copy at http://opensource.org/licenses/MIT)
**/

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
		if(!instances.solveInstances())
			return -2;

		//if(!instances.cheatInstances())
			//return -2;

		else if(!instances.loadInstances())
			return -3;

		Sleep(69);
	}

	return 0;
}
