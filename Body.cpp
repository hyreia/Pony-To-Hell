#include "main.hpp"

using namespace DBL;

const char *Body::body = "Body";

Body::Body():
ActorComponent(body),
sex(NO_SEX),
species(UNKNOWN)
{}

Body::Body(int _species, int _sex):
ActorComponent(body),
species(_species),
sex(_sex)
{
	bodyParts = game->world.species.CreateBodyPartsForSpecies(species);
	//CreateBodyPlanFromSpecies();
}

void Body::Deserialize(std::vector<std::string> tokens)
{
	//Name of species at 1
	species = atoi(tokens[SPECIES].c_str());
	sex = atoi(tokens[SEX].c_str());
	bodyParts = game->world.species.CreateBodyPartsForSpecies(species);

	int index = BODY_PART_INTEGRITY;
	//For each body part
	for(auto part = bodyParts.begin(); part != bodyParts.end(); part++)
	{	//Get integrity of each
		if(index < (int)tokens.size()) part->stateOfInjury = atoi(tokens[index++].c_str());
		else break;
	}

	//Statuses
	if(index < (int)tokens.size())
	{
		statusEffects.assign( atoi(tokens[index++].c_str()), StatusEffect(-1));
		for(auto status = statusEffects.begin(); status != statusEffects.end(); status++)
		{	//Get integrity of each
			if(index < (int)tokens.size()) status->type = atoi(tokens[index++].c_str());
			else break;
			if(index < (int)tokens.size()) status->value = atoi(tokens[index++].c_str());
			else break;
			if(index < (int)tokens.size()) status->framesLeft = atoi(tokens[index++].c_str());
			else break;
		}
	}

	//Equipment
	if(index < (int)tokens.size())
	{
		equipment.assign( atoi(tokens[index++].c_str()), NULL);
		for(auto item = equipment.begin(); item != equipment.end(); item++)
		{	//Get integrity of each
			if(index < (int)tokens.size()) (*item) = atoi(tokens[index++].c_str());
			else break;
		}
	}
}

std::string Body::Serialize()
{
	std::ostringstream serial;
	serial << ACT_COMP << 
		componentType << DELIM <<
		species << DELIM <<
		sex;

	for(auto part = bodyParts.begin(); part != bodyParts.end(); part++)
	{
		//... :0.44 ...
		serial << DELIM <<
		(*part).stateOfInjury;
	}

	serial << DELIM << statusEffects.size();
	for(auto status = statusEffects.begin(); status != statusEffects.end(); status++)
	{
		//"... :STATUS:4:3 ..."
		serial << DELIM <<
			status->type << DELIM <<
			status->value << DELIM <<
			status->framesLeft;
	}

	///TODO: equipment
	serial << DELIM << equipment.size();
	for(auto equip = equipment.begin(); equip != equipment.end(); equip++)
	{
		//"... :0 ..."
		serial << DELIM << (*equip);
	}

	return serial.str();
}


bool Body::HasAttackOrAbility(int attackOrAbilityIndex)
{

	for(auto part = bodyParts.begin(); part != bodyParts.end(); part++)
	{
		if( (*part).HasAttackOrAbility(attackOrAbilityIndex)) return true;
	}

	return false;
}

BodyPart *Body::GetBodyPartOfType(int bodyPartType, int orientation)
{
	BodyPart *bodyPart = NULL;

	for(auto part = bodyParts.begin(); part != bodyParts.end(); part++)
	{
		if( (*part).type == bodyPartType)
		{
			if(orientation == NO_ORIENTATION ||
				orientation == part->orientation)
			{
				bodyPart = &(*part);
				break;
			}
			//if(orientation == part->orientation) bodyPart
		}
	}

	return bodyPart;
}