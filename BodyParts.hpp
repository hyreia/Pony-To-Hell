#ifndef BODY_PARTS_HPP
#define BODY_PARTS_HPP

namespace DBL
{
	enum BODY_PART_TYPE
	{
		NO_PART_SPECIFIED = -1,
		HEAD,
		BEAK,
		MOUTH,
		MUZZLE,
		EYE,
		LEG,
		BACKLEG,
		ARM,
		HAND,
		CLAW,
		WING,
		HORN,
		TAIL,
		MAGIC_HORN,
		NUMBER_OF_BODY_PART_TYPES
	};
	extern const char *BODY_PART_NAMES[NUMBER_OF_BODY_PART_TYPES];

	enum BODY_PART_ORIENTATION
	{
		NO_ORIENTATION,
		LEFT_ORIENTATED,
		RIGHT_ORIENTATED
	};


	struct BodyPart
	{
		BodyPart(
			int type, bool canHoldWeapon,
			double calledShotPenalty,
			double healthPercentInArea,
			double stateOfInjury,
			double recoveryRate,
			int orientation = NO_ORIENTATION);

		int type;
		int orientation; //none, left, right


		bool canHoldWeapon;
		//how easy body part is to hit (0=impossible, 1.0=as easy as full body)
		double calledShotPenalty, 

			//fraction of body health bodypart contains (0-1.0)
			HealthPercentInArea,	
			//current state of body part (0-1.0, 0=broken and useless)
			stateOfInjury, 

			//rate in which injury heals
			recoveryRate;
		
		//State of injury of a body part
		static const double 
			FINE_CONDITION,
			SCRATCHED_CONDITION,
			HURT_CONDITION,
			DAMAGED_CONDITION,
			INJURED_CONDITION,
			BROKEN_CONDITION;

		//list of attack types and abilities granted by 
		//functioning body part when it's working
		std::list<int> attacksAndAbilitiesGranted;

		/* If not injured to the point of damaged and grants ability */
		bool HasAttackOrAbility(int attackOrAbilityIndex);


		static BodyPart NewHead		(double integrity  = 1.0);
		static BodyPart NewBeak		(double integrity  = 1.0);
		static BodyPart NewMouth	(double integrity  = 1.0);
		static BodyPart NewMuzzle	(double integrity  = 1.0);
		static BodyPart NewEye		(bool orientation, double integrity  = 1.0);
		static BodyPart NewLeg		(bool orientation, double integrity = 1.0);
		static BodyPart NewBackleg	(bool orientation, double integrity = 1.0);
		static BodyPart NewArm		(bool orientation, double integrity = 1.0);
		static BodyPart NewHand		(bool orientation, double integrity = 1.0);
		static BodyPart NewClaw		(bool orientation, double integrity = 1.0);
		static BodyPart NewWing		(bool orientation, double integrity = 1.0);
		static BodyPart NewHorn		(bool orientation = NO_ORIENTATION, double integrity = 1.0);
		static BodyPart NewMagicHorn(bool orientation = NO_ORIENTATION, double integrity = 1.0);
		static BodyPart NewTail		(double integrity = 1.0);

		//Does not assign orientation, set this after calling it
		static BodyPart NewBodyPartById(int typeIndex, double integrity = 1.0);


	private:
		void AddAbility(int ability){ attacksAndAbilitiesGranted.push_back(ability); }
	};
};

#endif