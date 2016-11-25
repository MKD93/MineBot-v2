#ifndef Process_H
#define Process_H

#include "Instance.hpp"

class Process
{
	public:
		std::vector<Instance> Instances;

		Process();
		~Process();
		Process(const Process&);
		Process& operator=(const Process&);

		bool loadInstances();
		bool updateInstances();
		bool solveInstances();
		bool cheatInstances();
};

#endif
