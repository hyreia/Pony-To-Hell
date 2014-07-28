#include "main.hpp"

using namespace DBL;

Actor::Actor(int _id):
id(_id),
images(NULL),
position(NULL),
ai(NULL),
movement(NULL),
combatant(NULL),
//level(NULL),
villager(NULL),
contains(NULL),
item(NULL),
//hostile(NULL),
body(NULL),
//species(NULL),
name(NULL)//,
//equipment(NULL)
{
}

Actor::~Actor()
{
	if(images) delete images;
	if(position) delete position;
	//if(ai) delete ai;
	if(movement) delete movement;
	//if(combatant) delete combatant;
	//if(level) delete level;
	if(villager) delete villager;
	//if(contains) delete contains;
	//if(carryable) delete carryable;
	//if(hostile) delete hostile;
	if(body) delete body;
	//if(species) delete species;
	//if(name) delete name;
	//if(equipment) delete equipment;
}

void Actor::ResetActDelay()
{
	int delay = 5900;


	//Other stuff
	if(combatant) delay -= combatant->ActDelayReduction();


	actDelay = delay;
}

bool Actor::Deserialize(std::vector<std::string> tokens)
{
	actDelay = atoi(tokens[1].c_str());
	return true;
}

std::string Actor::Serialize()
{
	const char DELIM = ':';
	const char ActorWord[] = "Actor";
	std::ostringstream serial;

	serial << ActorWord << DELIM <<
		actDelay;

	return serial.str();
}