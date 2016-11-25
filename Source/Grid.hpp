#ifndef Grid_H
#define Grid_H

#include <cstdint>
#include <vector>

class Grid
{
	public:
		std::vector<std::vector<uint8_t> > Cheat, Known;
		uint32_t Width, Height;

		Grid();
		~Grid();
		Grid(const Grid&);
		Grid& operator=(const Grid&);

		bool isInside(const int32_t, const int32_t) const;
		uint32_t getNeighbors(const int32_t, const int32_t, const uint8_t) const;
		void getBestMove(uint32_t&, uint32_t&) const;
};

#endif
