#include "Process.hpp"

Process::Process() : Instances() {}
Process::~Process() { Instances.clear(); }
Process::Process(const Process &copy) : Instances(copy.Instances) {}
Process& Process::operator=(const Process &copy) { Instances = copy.Instances; return (*this); }

bool Process::loadInstances()
{
	Instances.clear();

	TCHAR targetName[MAX_PATH] = "Minesweeper.exe";
	TCHAR processName[MAX_PATH];
	DWORD processList[1024];
	DWORD processCount;
	HANDLE processHandle;
	HMODULE moduleHandle;
	DWORD moduleHandleSize;

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

bool Process::updateInstances()
{
	bool result = false;

	for(size_t index = 0; index < Instances.size(); ++index)
		if(Instances[index].updateGrid())
			result = true;

	return (result && (!Instances.empty()));
}

bool Process::solveInstances()
{
	bool result = false;

	for(size_t index = 0; index < Instances.size(); ++index)
		if(Instances[index].solveGrid())
			result = true;

	return (result && (!Instances.empty()));
}

bool Process::cheatInstances()
{
	bool result = false;

	for(size_t index = 0; index < Instances.size(); ++index)
		if(Instances[index].cheatGrid())
			result = true;

	return (result && (!Instances.empty()));
}
