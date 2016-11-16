#ifndef Instance_H
#define Instance_H

#include "Grid.hpp"
#include <cstdint>

class Instance
{
	public:
		Grid Data;
		uint32_t Id;

		Instance();
		~Instance();
		Instance(const uint32_t);
		Instance(const Instance&);
		Instance& operator=(const Instance&);

		bool updateGrid();
		bool solveGrid();
		bool cheatGrid();
};

#endif
