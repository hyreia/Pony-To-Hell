#ifndef ACTOR_STATUS_EFFECTS_HPP
#define ACTOR_STATUS_EFFECTS_HPP

namespace DBL
{
	enum STATUS_EFFECT_TYPES
	{
		HUNGRY,
		THIRSTY,
		POISONED,
		BLINDED,
		TOTAL_STATUS_EFFECT_TYPES
	};

	struct StatusEffect
	{
		static const int EFFECT_DOESNT_EXPIRE = -1;

		int type, value, framesLeft;

		StatusEffect(int _type, int _value = 0, int _framesLeft = 0):
		type(_type),
		value(_value),
		framesLeft(_framesLeft)
		{}

	};

};

#endif