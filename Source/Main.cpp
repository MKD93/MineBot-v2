#include "Grid.hpp"
#include "Instance.hpp"
#include "Process.hpp"

#include <windows.h>

int main(int argc, char **argv)
{
	Process instances;

	if(!instances.loadInstances())
		return 1;

	while(instances.updateInstances())
	{
		if(!instances.solveInstances())
			return 2;

		Sleep(1000);
	}

	return 0;
}
