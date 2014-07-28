#include "main.hpp"

using namespace DBL;

const char *Combatant::combatant = "Combatant";

const int Combatant::BASE_HEALTH = 15;
const int Combatant::BASE_MANA = 10;
const int Combatant::BASE_ACT_DELAY_MULTIPLIER = 5;

Combatant::Combatant():
ActorComponent(combatant),
isHostile(true),
level(1),
unspentPoints(0),
experience(0),
currentHealth(15), //arbitrary amount so they're not ded
currentMana(0)
{
	stats.assign(TOTAL_BATTLE_STATS, 5);
}

Combatant::Combatant(int str, int dex, int intel, int vit, int energy, 
bool _isHostile):
ActorComponent(combatant),
isHostile(_isHostile),
level(1),
unspentPoints(0),
experience(0),
targetBodyPartForAttacks(NO_PART_SPECIFIED)
{
	stats.assign(TOTAL_BATTLE_STATS, 5);
	stats[STRENGTH] = str;
	stats[DEXTERITY] = dex;
	stats[INTELLIGENCE] = intel;
	stats[VITALITY] = vit;
	stats[ENERGY] = energy;
	currentHealth = GetMaxHealth();
	currentMana = GetMaxMana();
}


void Combatant::Deserialize(std::vector<std::string> tokens)
{
	stats[STRENGTH] = atoi(tokens[STR_STAT].c_str());
	stats[DEXTERITY] = atoi(tokens[DEX_STAT].c_str());
	stats[INTELLIGENCE] = atoi(tokens[INT_STAT].c_str());
	stats[VITALITY] = atoi(tokens[VIT_STAT].c_str());
	stats[ENERGY] = atoi(tokens[NRG_STAT].c_str());
	isHostile = atoi(tokens[IS_HOSTILE].c_str());
	currentHealth = atoi(tokens[CURRENT_HEALTH].c_str());
	currentMana = atoi(tokens[CURRENT_MANA].c_str());
	experience = atoi(tokens[EXPERIENCE].c_str());
	unspentPoints = atoi(tokens[UNSPENT_LEVELUP_POINTS].c_str());
	targetBodyPartForAttacks = atoi(tokens[TARGET_BODY_PART].c_str());

	//calculate level
	level = LevelAtGivenExperience(experience);
}

std::string Combatant::Serialize()
{
	std::ostringstream serial;
	serial << ACT_COMP <<
		combatant << DELIM <<
		stats[STR_STAT-1] << DELIM <<
		stats[DEX_STAT-1] << DELIM <<
		stats[INT_STAT-1] << DELIM <<
		stats[VIT_STAT-1] << DELIM <<
		stats[NRG_STAT-1] << DELIM <<
		isHostile << DELIM <<
		currentHealth << DELIM <<
		currentMana << DELIM <<
		experience << DELIM <<
		unspentPoints << DELIM <<
		targetBodyPartForAttacks;

	return serial.str();
}

int Combatant::Base(int stat)
{
	return stats[stat];
}

void Combatant::Set(int stat, int value)
{
	stats[stat] = value;
}

int Combatant::Mod(int stat)
{
	//return (stats[stat]-10)/2;
	return (stats[stat])/2;
}

bool Combatant::AddExperience(int addedExp)
{
	if(addedExp < 0) addedExp = 0;

	experience += addedExp;
	if(int newLevel = LevelAtGivenExperience(experience) > level)
	{
		unspentPoints += newLevel - level;
		level = newLevel;
		return true;
	}
	else return false;
}

void Combatant::SetExperienceAndLevel(int exp)
{
	experience = exp;
	level = LevelAtGivenExperience(experience);	
}

int Combatant::TotalExperienceAtNextLevelUp()
{
	return LevelAtGivenExperience(level+1);
}

int Combatant::GetMaxHealth()
{

	return (BASE_HEALTH +						//base
			stats[STRENGTH] +					//strength
			(2*stats[VITALITY]) +				//vitality
			(level-1)*( (stats[VITALITY]/2)+2));//leveling
}

int Combatant::GetMaxMana()
{
	return BASE_MANA +						//base
			stats[INTELLIGENCE]/2 +			//intel
			stats[VITALITY]*3 +				//vitality
			(level-1) * stats[VITALITY];	//leveling
}

int Combatant::ActDelayReduction()
{
	return
		stats[DEXTERITY]*6 +			//dex
		(level-1)*stats[DEXTERITY] +	//level
		currentMana	+					//mana
		stats[ENERGY]/2 *				//energy
		BASE_ACT_DELAY_MULTIPLIER;		//base
}