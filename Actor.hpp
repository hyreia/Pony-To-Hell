#ifndef ACTOR_HPP
#define ACTOR_HPP

namespace DBL
{


class ActorImages;
class Position;
class Ai;
class Movement;
class Combatant;
class Levels;
class Villager;
class Contains;
class Carryable;
//class Hostile;
class Body;
//class Species;
class Name;
class Item;

class Actor
{
public:

	Actor(int id);
	~Actor();

	const int id;		//not directly serialized but stored so Actors know their own id
	int actDelay;

	bool Deserialize(std::vector<std::string> tokens);
	std::string Serialize();

	void ResetActDelay();

	ActorImages	*images;
	Position	*position;
	Ai			*ai;
	Movement	*movement;
	Combatant	*combatant;
	
	Villager	*villager;
	Contains	*contains;
	//Carryable	*carryable; //move size and weight to carryable stuff
	Item		*item;
	
	Body		*body;
	Name		*name;

};

};

#endif