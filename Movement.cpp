#include "main.hpp"
#include <sstream>

using namespace DBL;

const char *Movement::movement = "Movement";

void Movement::Deserialize(std::vector<std::string> tokens)
{
	baseMoveSpeed = atof(tokens[BASE_MOVE_SPEED].c_str());
	destination.x = atoi(tokens[DESTINATION_X].c_str());
	destination.y = atoi(tokens[DESTINATION_Y].c_str());
	destinationDepth = atoi(tokens[DESTINATION_LEVEL].c_str());
	targetActorId = atoi(tokens[TARGET_ACTOR_ID].c_str());
	isMoving = atoi(tokens[IS_MOVING].c_str()); //converts fine to bool
}

std::string Movement::Serialize()
{
	std::ostringstream serial;
	serial << ACT_COMP << 
		componentType << DELIM <<
		baseMoveSpeed << DELIM <<
		destination.x << DELIM <<
		destination.y << DELIM <<
		destinationDepth << DELIM <<
		targetActorId << DELIM <<
		isMoving;

	return serial.str();
}