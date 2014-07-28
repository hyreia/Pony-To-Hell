#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "mt19937int.h"

namespace rng
{
	extern unsigned int seed;

	inline int Rand(int value)
	{
		int random_value;
		if(value==0)
			return 0;
		random_value= (int)(((float)genrand_int32() / (float)0xFFFFFFFF)*(value));
		return random_value;
	}
	inline void Seed(unsigned int s)
	{
		seed = s;
		init_genrand(seed);
		//BUG: For some reason this just generates zeros if I first use it in Map::InitToField, so use it here
		Rand(1); 
	}
	inline void Seed()
	{
		Seed((unsigned int)time(0));
	}
	
	/* Up to but not including max, max > min */
	inline int RandInRange(int min, int max)
	{
		int value = max-min;
		return Rand(value) + min;
	}

	inline int Dice(int num, int sides)
	{
		if(num<=0) return 0;
		int total = 0;
		while(num > 0)
		{
			total+= Rand(sides)+1;
			num--;
		}

		return total;
	}

	inline bool OneOutOf(int odds)
	{
		return 0==Rand(odds);
	}

	inline bool IsRandBoolTrue()
	{
		return (1==Rand(2));
	}

	/* 0% occuring will always fail, 100% will always succeed; (Rand is 0-99) */
	inline bool IsProbable(int probabilityPercentageOfOccuring)
	{
		
		return probabilityPercentageOfOccuring > Rand(100);
	}
};

#endif