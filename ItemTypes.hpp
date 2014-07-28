#ifndef ITEM_TYPES_HPP
#define ITEM_TYPES_HPP

namespace DBL
{

	enum CONSUMMABLE_EFFECTS
	{
		DO_NOTHING,
		RESTORE_HEALTH, 
		RESTORE_MANA,
		REMOVE_HUNGER, REMOVE_THIRST, ADD_SATION,
		HEAL_MINOR_WOUNDS
	};
	struct ConsummableEffect
	{
		ConsummableEffect(): effect(0), value(0){}
		ConsummableEffect(int e, int v): effect(e), value(v){}
		int effect;
		int value;
	};

	struct SpellCharged
	{
		SpellCharged(): spellIndex(0), chargesLeft(0), maxCharges(0){}
		SpellCharged(int spell, int totalCharges): spellIndex(spell), chargesLeft(totalCharges),
			maxCharges(totalCharges){}

		int spellIndex, chargesLeft, maxCharges;
	};

	class ItemType
	{
	public:
		//id
		int typeId;

		//identifying traits
		std::string name;
		std::string description;
		IndexedImage itemTileImage;

		//members in common to all items
		double weight;
		double size;
		int monetaryValue;

		//identifies the class of item (weapon, armor, consummable, scroll, book)
		int itemUsage;

		ItemType(int id):
		typeId(typeId),
		itemTileImage(),
		weight(0),
		size(0),
		monetaryValue(0),
		itemUsage(ITEM_TYPE_NONE)
		{}

		void Deserialize(std::vector<std::string> tokens);
		std::string Serialize();
		virtual ~ItemType(){}
		
		virtual void DeserializeItemType(std::vector<std::string> tokens) = 0;
		virtual std::string SerializeItemType() = 0;

		enum ITEM_TYPE_ENUM
		{
			TYPE_ID=1, 
			ITEM_USAGE,
			ITEM_NAME,
			ITEM_DESC,
			ITEM_TILE_IMAGE_INDEX,
			ITEM_TILE_IMAGE_R, ITEM_TILE_IMAGE_G, ITEM_TILE_IMAGE_B,
			ITEM_WEIGHT,
			ITEM_SIZE,
			ITEM_MONETARY_VALUE,
			
			START_OF_SUB_ITEMTYPE
		};

		enum ITEM_USUAGE_ENUM
		{
			WEAPON_USAGE=1,
			ARMOR_USAGE,
			CONSUMMABLE_USAGE,
			SCROLL_USAGE,
			BOOK_USAGE
		};

		static const char DELIM = ':';
	};


	class Weapon: public ItemType
	{
	public:
		Weapon(int id): ItemType(id){}
		~Weapon(){}
		void DeserializeItemType(std::vector<std::string> tokens) override;
		std::string SerializeItemType() override;

		int typeOfDamage;
		int attackTypeAndFormula;
		int attackBonus;
		int durabilityLeft;
		int strengthRequirement;
		std::list<int> equippableBodyParts;
		SpellCharged spellCharges;

		enum WEAPON_TYPE_ENUM
		{
			TYPE_OF_DAMAGE=START_OF_SUB_ITEMTYPE,
			ATTACK_TYPE_AND_FORMULA,
			ATTACK_BONUS,
			DURABILITY_LEFT,
			STRENGTH_REQUIREMENT,
			SPELL_CHARGES_ID, SPELL_CHARGES_LEFT, SPELL_CHARGES_MAX,
			NUMBER_OF_EQUIPPABLE_BODYPARTS,
			EQUIPPABLE_BODYPARTS_STARTS_HERE
		};
	};


	class Armor: public ItemType
	{
	public:
		Armor(int id):ItemType(id){}
		~Armor(){}
		void DeserializeItemType(std::vector<std::string> tokens) override;
		std::string SerializeItemType() override;

		std::list<BodyPart> coveredBodyParts;
		int damageResistance;
		int totalResistanceUntilBreak;
		int strongAgainstTypes;
		int preventsDamageFromTypes;
		int weakAgainstTypes;
		int carryLimitIncrease;
		bool mustBeWornFirst;
		SpellCharged spellCharges;

		enum ARMOR_TYPE_ENUM
		{
			DAMAGE_RESISTANCE=START_OF_SUB_ITEMTYPE,
			TOTAL_RESISTANCE_UNTIL_BREAK,
			STRONG_AGAINST_TYPES,
			PREVENTS_DAMAGE_FROM_TYPES,
			WEAK_AGAINST_TYPES,
			CARRY_LIMIT_INCREASE,
			MUST_BE_WORN_FIRST,
			SPELL_CHARGES_ID, SPELL_CHARGES_LEFT, SPELL_CHARGES_MAX,
			NUMBER_OF_BODY_PARTS_COVERED,
			BODY_PARTS_COVERED_START_HERE
		};
	};


	class Consummable: public ItemType
	{
	public:
		Consummable(int id):ItemType(id){}
		~Consummable(){}
		void DeserializeItemType(std::vector<std::string> tokens) override;
		std::string SerializeItemType() override;

		std::list< std::pair<int, int> > effects;

		enum CONSUMMABLE_TYPE_ENUM
		{
			NUMBER_OF_CONSUMMABLE_EFFECTS=START_OF_SUB_ITEMTYPE,
			CONSUMMABLE_EFFECTS_START_HERE
		};

	};


	class Scroll: public ItemType
	{
	public:
		Scroll(int id):ItemType(id){}
		~Scroll(){}
		void DeserializeItemType(std::vector<std::string> tokens) override;
		std::string SerializeItemType() override;

		int abilityCast;

		enum SCROLL_TYPE_ENUM
		{
			ABILITY_CAST=START_OF_SUB_ITEMTYPE
		};

	};

	
	class Book: public ItemType
	{
	public:
		Book(int id):ItemType(id){}
		~Book(){}
		void DeserializeItemType(std::vector<std::string> tokens) override;
		std::string SerializeItemType() override;

		int abilityTaught;
		int intelligenceRequiredToReadSuccessfully;

		enum BOOK_TYPE_ENUM
		{
			ABILITY_TAUGHT=START_OF_SUB_ITEMTYPE,
			INTEL_REQUIRED
		};
	};

};

#endif