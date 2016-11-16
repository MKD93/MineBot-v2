#include "Process.hpp"

#include <windows.h>
#include <psapi.h>

Process::Process() : Instances(nullptr), Size(0)
{
}

Process::~Process()
{
	if(Instances != nullptr)
	{
		free(reinterpret_cast<void*>(Instances));

		Instances = nullptr;
		Size = 0;
	}
}

Process::Process(const Process &copy) : Instances(nullptr), Size(copy.Size)
{
	Instances = reinterpret_cast<Instance*>(calloc(Size, sizeof(Instance)));

	if(Instances == nullptr)
		Size = 0;

	else
		memcpy(reinterpret_cast<void*>(Instances), reinterpret_cast<const void*>(copy.Instances), Size * sizeof(Instance));
}

Process& Process::operator=(const Process &copy)
{
	Instances = reinterpret_cast<Instance*>(calloc(copy.Size, sizeof(Instance)));

	if(Instances == nullptr)
		Size = 0;

	else
	{
		Size = copy.Size;
		memcpy(reinterpret_cast<void*>(Instances), reinterpret_cast<const void*>(copy.Instances), Size * sizeof(Instance));
	}

	return (*this);
}

bool Process::loadInstances()
{
	DWORD processList[1024];
	DWORD processCount;

	if(!EnumProcesses(processList, sizeof(DWORD) << 10, &processCount))
		return false;

	processCount >>= 2;

	TCHAR targetName[MAX_PATH] = "Minesweeper.exe";
	TCHAR processName[MAX_PATH];
	HANDLE processHandle;
	HMODULE moduleHandle;
	DWORD moduleHandleSize;
	uint32_t offset = 0;

	for(uint8_t loop = 0; loop < 2; ++loop)
	{
		for(DWORD index = 0; index < processCount; ++index)
		{
			processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processList[index]);

			if(processHandle > 0)
			{
				if(EnumProcessModules(processHandle, &moduleHandle, sizeof(moduleHandle), &moduleHandleSize))
				{
					GetModuleBaseName(processHandle, moduleHandle, processName, MAX_PATH);

					if(strcmp(processName, targetName) == 0)
					{
						if(loop == 0)
							++Size;

						else
							Instances[offset++] = Instance(processList[index]);
					}
				}

				CloseHandle(processHandle);
			}
		}

		if(loop == 0)
		{
			Instances = reinterpret_cast<Instance*>(calloc(Size, sizeof(Instance)));

			if(Instances == nullptr)
			{
				Size = 0;
				return false;
			}
		}
	}

	return (Size != 0);
}

bool Process::updateInstances()
{
	bool result = true;

	for(size_t index = 0; index < Size; ++index)
		if(!Instances[index].updateGrid())
			result = false;

	return result;
}

bool Process::solveInstances()
{
	bool result = true;

	for(size_t index = 0; index < Size; ++index)
		if(!Instances[index].solveGrid())
			result = false;

	return result;
}

bool Process::cheatInstances()
{
	bool result = true;

	for(size_t index = 0; index < Size; ++index)
		if(!Instances[index].cheatGrid())
			result = false;

	return result;
}
