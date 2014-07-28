#ifndef COMBATANT_HPP
#define COMBATANT_HPP

namespace DBL
{
	/* Combatant lists the stats used in base stats used in combat, detailed below.
	In addition to five stats it also has derived stats*/
	class Combatant: public ActorComponent
	{
	public:
		Combatant();
		Combatant(int str, int dex, int intel, int vit, int energy, 
			bool isHostile = true);

		virtual ~Combatant(){}

			void Deserialize(std::vector<std::string> tokens) override;
			std::string Serialize() override;

			static const char *combatant;

			int  Base(int stat);
			void Set(int stat, int value);
			int  Mod(int stat);

			/* Whether your default action against this actor should be combat */
			bool isHostile;

			/* Returns true if levels up from experience, adds 1 to unspent points on leveling */
			bool AddExperience(int experience);
			void SetExperienceAndLevel(int exp);
			int TotalExperienceAtNextLevelUp();

			int level;
			int unspentPoints; //used to level a stat
			int experience;

			int currentHealth;

			int GetMaxHealth();

			int currentMana;
			int GetMaxMana();

			/* [ (DEX*6) + (level-1)*DEX + currentMana + energy/2]*5 */
			int ActDelayReduction();

			/* DEX + max(INT-3, 1)*/
			int Evade();

			//calculates level an actor would be at given their experience, see body for formula
			static int LevelAtGivenExperience(int experience)
			{
				int lvl = 1;

				//EXP for level n = (n*(n-1)/2) * 1000
				while(experience < (lvl*(lvl-1)/2) * 1000)
				{
					lvl++;
				}

				return lvl;
			}

			static const int BASE_HEALTH;
			static const int BASE_MANA;
			static const int BASE_ACT_DELAY_MULTIPLIER;

			int targetBodyPartForAttacks;
	private:
		/* strength: modifies how strong physical attacks are, carrying capacity, 
		heaviness of armor that can be worn, ability to parry, 
		ability to wield heavy weapons effectively, */
		
		/* dexterity: act delay, ability to evade, factors into range attacks, 
		ability to wield heavy weapons effectively, */
		
		/* intelligence: magical attack power, magic fail rate, ability to dodge */
		
		/* vitality: amount of damage body part can sustain, minimum blood loss before death occurs, 
		resistance to negative status effects, recovery speed of body parts, 
		recovery rate of mana, */ 

		/* energy: amount of mana present, recovery rate of mana */



		std::vector<int> stats;

	};

	enum BATTLE_STATS
	{
		STRENGTH,
		DEXTERITY,
		INTELLIGENCE,
		VITALITY,
		ENERGY,
		TOTAL_BATTLE_STATS
	};

	enum SERIAL_COMBATANT_ENUMS
	{
		STR_STAT=1,
		DEX_STAT,
		INT_STAT,
		VIT_STAT,
		NRG_STAT,
		IS_HOSTILE,
		CURRENT_HEALTH,
		CURRENT_MANA,
		EXPERIENCE,
		UNSPENT_LEVELUP_POINTS,
		TARGET_BODY_PART,
	};
};

#endif