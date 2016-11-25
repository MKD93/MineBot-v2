#include "Grid.hpp"
#include "Instance.hpp"
#include "Process.hpp"

#include <windows.h>

int main(int argc, char **argv)
{
	Process instances;

	if(!instances.loadInstances())
		return -1;

	while(instances.updateInstances())
	{
		//if(!instances.solveInstances())
			//return -2;

		if(!instances.cheatInstances())
			return -2;

		else if(!instances.loadInstances())
			return -3;

		Sleep(500);
	}

	return 0;
}
