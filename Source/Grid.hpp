#ifndef Grid_H
#define Grid_H

#include <cstdint>

class Grid
{
	public:
		int8_t Cheat[32][32], Known[32][32];
		uint32_t Width, Height;

		Grid();
		~Grid();
		Grid(const Grid&);
		Grid& operator=(const Grid&);

		bool isInside(const int8_t, const int8_t) const;
		int8_t getKnown(const int8_t, const int8_t) const;
		void setKnown(const int8_t, const int8_t, const int8_t);
		int8_t getNeighbors(const int8_t, const int8_t, const int8_t) const;
		bool isNeighbor(const int8_t, const int8_t, const int8_t, const int8_t) const;

		bool solveSingle(uint32_t&, uint32_t&);
		bool solveDouble(uint32_t&, uint32_t&);
		void getBestMove(uint32_t&, uint32_t&);
};

#endif
