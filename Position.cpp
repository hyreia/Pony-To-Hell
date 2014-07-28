#include "main.hpp"


using namespace DBL;

const char *Position::position = "Position";

void Position::Deserialize(std::vector<std::string> tokens)
{
	tile.x = atoi(tokens[TILE_POSX].c_str());
	tile.y = atoi(tokens[TILE_POSY].c_str());
	absolutePos.x = atof(tokens[ABS_POSX].c_str());
	absolutePos.y = atof(tokens[ABS_POSY].c_str());
	elevation = atoi(tokens[ELEVATION].c_str());
	depth = atoi(tokens[LEVEL].c_str());
	
}

std::string Position::Serialize()
{
	std::ostringstream serial;

	serial << ACT_COMP << 
		componentType << DELIM <<
		tile.x << DELIM <<
		tile.y << DELIM <<
		absolutePos.x << DELIM <<
		absolutePos.y << DELIM <<
		elevation << DELIM <<
		depth;

	return serial.str();
}