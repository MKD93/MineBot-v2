#include "Instance.hpp"

#include <windows.h>
#include <psapi.h>

static HWND IdHandle = nullptr;

static BOOL CALLBACK EnumWindowsProc(HWND handle, LPARAM lparam)
{
	DWORD processID;
    GetWindowThreadProcessId(handle, &processID);

    if(processID == lparam)
    {
    	IdHandle = handle;
        return FALSE;
    }

    return TRUE;
}

Instance::Instance() : Data(), Id(0)
{
}

Instance::~Instance()
{
}

Instance::Instance(const uint32_t id) : Data(), Id(id)
{
}

Instance::Instance(const Instance &copy) : Data(copy.Data), Id(copy.Id)
{
}

Instance& Instance::operator=(const Instance &copy)
{
	Data = copy.Data;
	Id = copy.Id;

	return (*this);
}

bool Instance::updateGrid()
{
	bool insideGrid = false;
	uint8_t memoryBuffer[1 << 16];
	uint32_t x_Cheat = 0, y_Cheat = 0, x_Known = 0, y_Known = 0;

	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, Id);

	if(processHandle > 0)
	{
		if(ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(0x01000000), reinterpret_cast<LPVOID>(memoryBuffer), 1 << 16, nullptr))
		{
			Data = Grid();
			Data.Width = memoryBuffer[0x005334];
			Data.Height = memoryBuffer[0x005338];

			for(DWORD index = 0; index < (Data.Height << 5); ++index)
			{
				if(memoryBuffer[index + 0x005360] & 0x10)
				{
					if(!insideGrid)
					{
						x_Cheat = 0;
						x_Known = 0;

						++y_Cheat;
						++y_Known;
					}

					insideGrid = !insideGrid;
				}

				else if(insideGrid)
				{
					Data.Cheat[x_Cheat++][y_Cheat - 1] = ((memoryBuffer[index + 0x005360] & 0x80) != 0) ? 1 : 0;

					if(memoryBuffer[index + 0x005360] == 64)
						Data.Known[x_Known][y_Known - 1] = 0;

					else if(memoryBuffer[index + 0x005360] > 64 && memoryBuffer[index + 0x005360] < 91)
						Data.Known[x_Known][y_Known - 1] = memoryBuffer[index + 0x005360] - 64;

					++x_Known;
				}
			}

			CloseHandle(processHandle);
			return true;
		}

		CloseHandle(processHandle);
	}

	return false;
}

bool Instance::solveGrid()
{
	if(IdHandle == nullptr)
		EnumWindows(EnumWindowsProc, Id);

	if(IdHandle == nullptr)
		return false;

	uint32_t x = 0, y = 0;

	Data.getBestMove(x, y);
	SendMessage(IdHandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM((x << 4) + 20, (y << 4) + 70));
	SendMessage(IdHandle, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM((x << 4) + 20, (y << 4) + 70));

	return true;
}

bool Instance::cheatGrid()
{
	if(IdHandle == nullptr)
		EnumWindows(EnumWindowsProc, Id);

	if(IdHandle == nullptr)
		return false;

	for(uint32_t y = 0; y < Data.Height; ++y)
		for(uint32_t x = 0; x < Data.Width; ++x)
			if(Data.Cheat[x][y] == 0)
			{
				SendMessage(IdHandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM((x << 4) + 20, (y << 4) + 70));
				SendMessage(IdHandle, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM((x << 4) + 20, (y << 4) + 70));
			}

	return true;
}
