#ifndef BODY_HPP
#define BODY_HPP

namespace DBL
{
	enum SEX
	{
		NO_SEX,
		FEMALE,
		MALE
	};

	enum NAMED_SPECIES
	{
		UNKNOWN,
		EARTH_PONY,
		UNICORN_PONY,
		PEGASUS_PONY,
		GRIFFON,
		KOBOLD
	};


	class Body: public ActorComponent
	{
		public:
			virtual ~Body(){}
			//static Body *NewBodyFromDeserialization(std::vector<std::string> tokens);

			void Deserialize(std::vector<std::string> tokens) override;
			std::string Serialize() override;

			static const char *body;

			int species; //used for saying what something is and for serializing its body plan
			int sex; //only for pronoun usage
			std::list<BodyPart> bodyParts;

			//TODO: for equipment
			std::list<int> equipment;

			enum BODY_SERIALIZE_ENUM
			{
				SPECIES=1,
				SEX,
				BODY_PART_INTEGRITY
			};

			bool HasAttackOrAbility(int attackOrAbilityIndex);
			BodyPart *GetBodyPartOfType(int bodyPartType, int orientation = 0);

			Body();
			Body(int species, int sex);
			
			std::list<StatusEffect> statusEffects;

			//static inline std::list<BodyPart> GetPonyBodyPlan();
			//static inline std::list<BodyPart> GetGriffonBodyPlan();
			//static inline std::list<BodyPart> GetKoboldBodyPlan();

	private:
		//void Body::CreateBodyPlanFromSpecies();
	};
/*
	class EarthPonyBody: public Body
	{
	public:
		static const char *subType;
		EarthPonyBody(int gender);
		virtual ~EarthPonyBody();

		//Didn't want to have a deep class inheritance so we're doing this
		static std::list<BodyPart> GetHorseBody(); //used by other classes as well that are horse+X

		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;

		enum HORSE_PARTS_ENUM
		{
			HORSE_HEAD=1,
			HORSE_LEYE,
			HORSE_REYE,
			HORSE_MUZZLE,
			HORSE_LLEG,
			HORSE_RLEG,
			HORSE_LBACKLEG,
			HORSE_RBACKLEG,
			TOTAL_HORSE_BODYPARTS_PLUS_ONE
		};
	};

	class UnicornPonyBody: public Body
	{
	public:
		static const char *subType;
		UnicornPonyBody(int gender);
		virtual ~UnicornPonyBody();
		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;
	};

	class PegasusPonyBody: public Body
	{
	public:
		static const char *subType;
		PegasusPonyBody(int gender);
		virtual ~PegasusPonyBody();
		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;
	};

	class GriffonBody: public Body
	{
	public:
		static const char *subType;
		GriffonBody(int gender);
		virtual ~GriffonBody();
		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;
	};

	class KoboldBody: public Body
	{
	public:
		static const char *subType;
		KoboldBody(int gender);
		virtual ~KoboldBody();
		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;
	};
	*/
};

#endif