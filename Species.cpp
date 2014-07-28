#include "main.hpp"

using namespace DBL;

std::list<BodyPart> Species::GetPonyBodyPlan()
{
	std::list<BodyPart> horse;

	horse.push_back(BodyPart::NewHead());
	horse.push_back(BodyPart::NewEye(LEFT_ORIENTATED));
	horse.push_back(BodyPart::NewEye(RIGHT_ORIENTATED));
	horse.push_back(BodyPart::NewMuzzle());
	horse.push_back(BodyPart::NewLeg(LEFT_ORIENTATED));
	horse.push_back(BodyPart::NewLeg(RIGHT_ORIENTATED));
	horse.push_back(BodyPart::NewBackleg(LEFT_ORIENTATED));
	horse.push_back(BodyPart::NewBackleg(RIGHT_ORIENTATED));
	horse.push_back(BodyPart::NewTail());

	return horse;
}

std::list<BodyPart> Species::GetGriffonBodyPlan()
{
	std::list<BodyPart> griffon;
	griffon.push_back(BodyPart::NewHead());
	griffon.push_back(BodyPart::NewEye(LEFT_ORIENTATED));
	griffon.push_back(BodyPart::NewEye(RIGHT_ORIENTATED));
	griffon.push_back(BodyPart::NewBeak());
	griffon.push_back(BodyPart::NewClaw(LEFT_ORIENTATED));
	griffon.push_back(BodyPart::NewClaw(RIGHT_ORIENTATED));
	griffon.push_back(BodyPart::NewBackleg(LEFT_ORIENTATED));
	griffon.push_back(BodyPart::NewBackleg(RIGHT_ORIENTATED));
	griffon.push_back(BodyPart::NewTail());
	return griffon;
}

std::list<BodyPart> Species::GetKoboldBodyPlan()
{
	std::list<BodyPart> kobold;
	kobold.push_back(BodyPart::NewHead());
	kobold.push_back(BodyPart::NewEye(LEFT_ORIENTATED));
	kobold.push_back(BodyPart::NewEye(RIGHT_ORIENTATED));
	kobold.push_back(BodyPart::NewMouth());
	kobold.push_back(BodyPart::NewClaw(LEFT_ORIENTATED));
	kobold.push_back(BodyPart::NewClaw(RIGHT_ORIENTATED));
	kobold.push_back(BodyPart::NewLeg(LEFT_ORIENTATED));
	kobold.push_back(BodyPart::NewLeg(RIGHT_ORIENTATED));
	return kobold;
}

std::list<BodyPart> Species::CreateBodyPartsForSpecies(int species)
{
	std::list<BodyPart> ret;

	
	switch(species)
	{
	case EARTH_PONY:
		ret = GetPonyBodyPlan();
		break;
	case UNICORN_PONY:
		ret = GetPonyBodyPlan();
		ret.push_back(BodyPart::NewMagicHorn());
		break;
	case PEGASUS_PONY:
		ret = GetPonyBodyPlan();
		ret.push_back(BodyPart::NewWing(LEFT_ORIENTATED));
		ret.push_back(BodyPart::NewWing(RIGHT_ORIENTATED));
		break;
	case GRIFFON:
		ret = GetGriffonBodyPlan();
		break;
	case KOBOLD:
		ret = GetKoboldBodyPlan();
		break;
	default:
	case UNKNOWN:
		break;
	}


	return ret;
}


/* Weight is in grams, as all weights in the game will be*/
int Species::GetBaseWeightForSpecies(int species, int sex)
{
	int weight = 0;

	switch(species)
	{
	case EARTH_PONY:
		weight = 113*1000;
		break;
	case UNICORN_PONY:
		weight = 100*1000;
		break;
	case PEGASUS_PONY:
		weight = 68*1000;
		break;
	case GRIFFON:
		weight = 180*1000;
		break;
	case KOBOLD:
		weight = 40*1000;
		break;
	default:
		break;
	}

	return weight;
}

std::vector<int> Species::GetMainNaturalAttacks(int species)
{
	std::vector<int> attacks;

	switch(species)
	{
	case EARTH_PONY:
	case PEGASUS_PONY:
	case UNICORN_PONY:
		attacks.push_back(KICK);
		attacks.push_back(BUCK);
		break;
	case GRIFFON:
		attacks.push_back(SCRATCH);
		attacks.push_back(SCRATCH); //x2 more common
		attacks.push_back(PECK);
		break;
	default:
		attacks.push_back(TACKLE);
		break;
	}

	return attacks;
}