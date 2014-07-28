#include "main.hpp"

#include <algorithm>

using namespace DBL;

const double BodyPart::FINE_CONDITION		= 1.0;
const double BodyPart::SCRATCHED_CONDITION	= 0.9;
const double BodyPart::HURT_CONDITION		= 0.7;
const double BodyPart::DAMAGED_CONDITION	= 0.5;
const double BodyPart::INJURED_CONDITION	= 0.3;
const double BodyPart::BROKEN_CONDITION		= 0.0;

const char *DBL::BODY_PART_NAMES[NUMBER_OF_BODY_PART_TYPES] =
{
"Head",			//0
"Beak",	
"Mouth",
"Muzzle",
"Eye",
"Leg",
"Backleg",		//6
"Arm",
"Hand",
"Claw",
"Wing",
"Horn",
"Magic Horn"	//12
};

BodyPart::BodyPart(
	int _type, 
	bool _canHoldWeapon,
	double _calledShotPenalty,
	double _healthInArea,
	double _stateOfInjury,
	double _recoveryRate,
	int _orientation):
type(_type),
canHoldWeapon(_canHoldWeapon),
calledShotPenalty(_calledShotPenalty),
HealthPercentInArea(_healthInArea),
stateOfInjury(_stateOfInjury),
recoveryRate(_recoveryRate),
orientation(_orientation)
{}

BodyPart BodyPart::NewHead(double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	bool canWieldWeapon = false;

	BodyPart part(HEAD, canWieldWeapon, 0.75, 0.35, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS);
	part.AddAbility(CONSCIOUSNESS);
	part.AddAbility(HEADBUTT);

	return part;
}

BodyPart BodyPart::NewBeak(double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	bool canWieldWeapon = false;

	BodyPart part(BEAK, true, 0.25, 0.2, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS);
	part.AddAbility(PECK);
	return part;
}

BodyPart BodyPart::NewMouth(double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = MOUTH;
	bool canWieldWeapon = false;
	double calledshotPenalty = 0.25;
	double healthInArea = 0.2;

	BodyPart part(MOUTH, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS);
	part.AddAbility(BITE);
	return part;
}

BodyPart BodyPart::NewMuzzle(double integrity)
{
	BodyPart part = NewMouth(integrity);
	part.type = MUZZLE;
	bool calledshotPenalty = 0.30; //slightly easier to hit
	part.canHoldWeapon = true;

	return part;
}

BodyPart BodyPart::NewEye(bool orientation, double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = EYE;
	bool canWieldWeapon = false;
	double calledshotPenalty = 0.25;
	double healthInArea = 0.05;

	BodyPart part(bodyPartType, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS, orientation);
	part.AddAbility(SIGHT);
	return part;
}

BodyPart BodyPart::NewLeg(bool orientation, double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = LEG;
	bool canWieldWeapon = false;
	double calledshotPenalty = 0.85;
	double healthInArea = 0.20;

	BodyPart part(bodyPartType, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS, orientation);
	part.AddAbility(MOVEMENT);
	part.AddAbility(KICK);
	return part;
}

BodyPart BodyPart::NewBackleg(bool orientation, double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = BACKLEG;
	bool canWieldWeapon = false;
	double calledshotPenalty = 0.75;
	double healthInArea = 0.15;

	BodyPart part(bodyPartType, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS, orientation);
	part.AddAbility(MOVEMENT);
	part.AddAbility(BUCK);
	return part;
}

BodyPart BodyPart::NewArm(bool orientation, double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = ARM;
	bool canWieldWeapon = false;
	double calledshotPenalty = 0.85;
	double healthInArea = 0.20;

	BodyPart part(bodyPartType, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS, orientation);
	part.AddAbility(INCREASE_CARRY);
	return part;
}

BodyPart BodyPart::NewHand(bool orientation, double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = HAND;
	bool canWieldWeapon = true;
	double calledshotPenalty = 0.60;
	double healthInArea = 0.10;

	BodyPart part(bodyPartType, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS, orientation);
	part.AddAbility(INCREASE_CARRY);
	part.AddAbility(PUNCH);
	return part;
}

BodyPart BodyPart::NewClaw(bool orientation, double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = CLAW;
	bool canWieldWeapon = true;
	double calledshotPenalty = 0.60;
	double healthInArea = 0.10;

	BodyPart part(bodyPartType, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS, orientation);
	part.AddAbility(SCRATCH);
	return part;
}

BodyPart BodyPart::NewWing(bool orientation, double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = WING;
	bool canWieldWeapon = false;
	double calledshotPenalty = 0.80;
	double healthInArea = 0.15;

	BodyPart part(bodyPartType, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS, orientation);
	part.AddAbility(FLIGHT);
	return part;
}

BodyPart BodyPart::NewHorn(bool orientation, double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = HORN;
	bool canWieldWeapon = false;
	double calledshotPenalty = 0.30;
	double healthInArea = 0.10;

	BodyPart part(bodyPartType, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS, orientation);
	part.AddAbility(IMPALE);
	return part;
}

BodyPart BodyPart::NewMagicHorn(bool orientation, double integrity)
{
	BodyPart part = NewHorn(orientation, integrity);
	part.type = MAGIC_HORN;
	part.canHoldWeapon = true;
	part.AddAbility(MAGIC);

	return part;
}

BodyPart BodyPart::NewTail(double integrity)
{
	const double SECONDS_UNTIL_FULL_RECOVERY = 60;
	int bodyPartType = TAIL;
	bool canWieldWeapon = true;
	double calledshotPenalty = 0.70;
	double healthInArea = 0.10;

	BodyPart part(bodyPartType, canWieldWeapon, calledshotPenalty, healthInArea, integrity,
		1.0/SECONDS_UNTIL_FULL_RECOVERY*Game::FPS);
	return part;
}

/*
BodyPart BodyPart::NewBodyPartById(int typeIndex, double integrity)
{
	switch(typeIndex)
	{
	case HEAD: return NewHead(integrity);
		break;
	case BEAK: return NewBeak(integrity);
		break;
	case MOUTH: return NewMouth(integrity);
		break;
	case MUZZLE: return NewMuzzle(integrity);
		break;
	case EYE: return NewEye(NO_ORIENTATION, integrity);
		break;
	case LEG: return NewLeg(NO_ORIENTATION, integrity);
		break;
	case BACKLEG: return NewBackleg(NO_ORIENTATION, integrity);
		break;
	case ARM: return NewArm(NO_ORIENTATION, integrity);
		break;
	case HAND: return NewHand(NO_ORIENTATION, integrity);
		break;
	case CLAW: return NewClaw(NO_ORIENTATION, integrity);
		break;
	case WING: return NewWing(NO_ORIENTATION, integrity);
		break;
	case HORN: return NewHorn(NO_ORIENTATION, integrity);
		break;
	case MAGIC_HORN: return NewMagicHorn(NO_ORIENTATION, integrity);
		break;
	default:
		break;
	}
	//Not all control paths originally returned a value. This was intentional.
	//Be sure to check the typeIndex is valid before calling this!

}*/

bool BodyPart::HasAttackOrAbility(int attackOrAbilityIndex)
{
	return (stateOfInjury > DAMAGED_CONDITION && 
		std::find(attacksAndAbilitiesGranted.begin(), attacksAndAbilitiesGranted.end(), 
		attackOrAbilityIndex) != attacksAndAbilitiesGranted.end());
}