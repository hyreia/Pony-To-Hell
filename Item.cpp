#include "main.hpp"

using namespace DBL;

const char *Item::item = "Item";

Item::Item(int _instance, int _type):
ActorComponent(item),
instanceId(_instance), itemType(_type),
usesLeft(1.0)
{}

void Item::Deserialize(std::vector<std::string> tokens)
{
	instanceId		= atoi(tokens[INSTANCE_ID].c_str());
	itemType	= atoi(tokens[ITEM_TYPE].c_str());
	usesLeft	= atof(tokens[USES_LEFT].c_str());
}

std::string Item::Serialize()
{
	std::ostringstream serial;
		serial << 
		ACT_COMP <<
		item		<< DELIM <<
		instanceId	<< DELIM <<
		itemType	<< DELIM <<
		usesLeft;

	return serial.str();
}