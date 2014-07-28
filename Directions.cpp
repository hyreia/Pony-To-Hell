#include "main.hpp"

void ConvertDirectionToVector(DIRECTION dir, int &dx, int &dy)
{
	switch(dir)
	{
	case DIR_UP:
		dx=0; dy=-1;
		break;
	case DIR_UPRIGHT:
		dx=1; dy=-1;
		break;
	case DIR_RIGHT:
		dx=1; dy=0;
		break;
	case DIR_DOWNRIGHT:
		dx=1; dy=1;
		break;
	case DIR_DOWN:
		dx=0; dy=1;
		break;
	case DIR_DOWNLEFT:
		dx=-1; dy=1;
		break;
	case DIR_LEFT:
		dx=-1; dy=0;
		break;
	case DIR_UPLEFT: 
		dx=-1; dy=-1;
		break;
	default:
		dx=0; dy=0;
		break;
	}
}