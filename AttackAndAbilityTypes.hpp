#ifndef ATTACK_AND_ABILITY_TYPES_HPP
#define ATTACK_AND_ABILITY_TYPES_HPP

namespace DBL
{

	enum ABILITIES_FROM_BODYPARTS
	{
		MOVEMENT,
		SIGHT,
		FLIGHT, //takes at least 1 left and 1 right orientated wing
		MAGIC,
		CONSCIOUSNESS, //i.e. being able to be knocked out
		INCREASE_CARRY,
		TOTAL_ABILITIES
	};

	enum ATTACK_TYPES //these help us identify damage and determine the formula used
	{
		FIRST_ATTACK_TYPE = TOTAL_ABILITIES,
		TACKLE  = TOTAL_ABILITIES, //start numbering after abilities
		HEADBUTT,
		IMPALE,
		BITE,
		SCRATCH,
		PECK,
		KICK,
		BUCK,
		PUNCH,
		SHORT_SWORD,
		//swords, whips, arrows, etc
		TOTAL_ATTACK_TYPES

	};

	enum DAMAGE_TYPES
	{
		UNTYPED_DAMAGE		= 0,
		BLUDGEON_DAMAGE		= 1 << 0,
		PIERCING_DAMAGE		= 1 << 1,
		SLASHING_DAMAGE		= 1 << 2,
		FIRE_DAMAGE			= 1 << 3,
		LIGHTNING_DAMAGE	= 1 << 4,
		POISON_DAMAGE		= 1 << 5,
		MAGIC_DAMAGE		= 1 << 6
	};

	enum COMBAT_RANGE
	{
		SELF				= 1,
		FRIENDLY_COMBATANTS = 2,
		HOSTILE_COMBATANTS	= 4,
		AREA				= 8
	};


};

#endif