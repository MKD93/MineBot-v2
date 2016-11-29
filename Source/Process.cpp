/**
	File: "Process.cpp"

	Copyright (c) 2016 Michael K. Duncan (fruitless75@gmail.com)
	Distributed under the MIT License (MIT) (See accompanying file LICENSE
	or copy at http://opensource.org/licenses/MIT)
**/

#include "Process.hpp"

Process::Process() : Instances() {}
Process::~Process() { Instances.clear(); }
Process::Process(const Process &copy) : Instances(copy.Instances) {}
Process& Process::operator=(const Process &copy) { Instances = copy.Instances; return (*this); }

/*
	Searches through all active processes for instances of Minesweeper, and then
	stores relevant process information for each active instance so that the game
	state can be easily read, and so that input commands can be issued properly.
*/
bool Process::loadInstances()
{
	Instances.clear();

	TCHAR targetName[MAX_PATH] = "Minesweeper.exe", processName[MAX_PATH];
	DWORD processList[1024], processCount, moduleHandleSize;
	HANDLE processHandle;
	HMODULE moduleHandle;

	if(!EnumProcesses(processList, sizeof(DWORD) << 10, &processCount))
		return false;

	for(DWORD index = 0; index < (processCount >> 2); ++index)
	{
		processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processList[index]);

		if(processHandle > 0)
		{
			if(EnumProcessModules(processHandle, &moduleHandle, sizeof(moduleHandle), &moduleHandleSize))
			{
				GetModuleBaseName(processHandle, moduleHandle, &(processName[0]), MAX_PATH);

				if(strcmp(targetName, processName) == 0)
					Instances.push_back(Instance(processList[index]));
			}

			CloseHandle(processHandle);
		}
	}

	return (!Instances.empty());
}

/*
	Updates the game state for each active instance of Minesweeper by reading
	data directly from active process memory.
*/
bool Process::updateInstances()
{
	bool result = false;

	for(size_t index = 0; index < Instances.size(); ++index)
		if(Instances[index].updateGrid())
			result = true;

	return (result && (!Instances.empty()));
}

/*
	Solves the game for each active instance of Minesweeper depending on the
	current values for each game state.
*/
bool Process::solveInstances()
{
	bool result = false;

	for(size_t index = 0; index < Instances.size(); ++index)
		if(Instances[index].solveGrid())
			result = true;

	return (result && (!Instances.empty()));
}

/*
	"Cheats" on the game for each active instance of Minesweeper.
*/
bool Process::cheatInstances()
{
	bool result = false;

	for(size_t index = 0; index < Instances.size(); ++index)
		if(Instances[index].cheatGrid())
			result = true;

	return (result && (!Instances.empty()));
}
