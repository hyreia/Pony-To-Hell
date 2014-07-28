#ifndef DIRECTION_HPP
#define DIRECTION_HPP

enum DIRECTION
{
	DIR_UP			=0,
	DIR_UPRIGHT		=1,
	DIR_RIGHT		=2,
	DIR_DOWNRIGHT	=3,
	DIR_DOWN		=4,
	DIR_DOWNLEFT	=5,
	DIR_LEFT		=6,
	DIR_UPLEFT		=7,
	TOTAL_DIRECTIONS=8,
	NO_DIRECTION=9
};

/* For example, DIR_UP = (0,-1), DIR_DOWNLEFT=(-1,1)*/
void ConvertDirectionToVector(DIRECTION dir, int &dx, int &dy);



#endif