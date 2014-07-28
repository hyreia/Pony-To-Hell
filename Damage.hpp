#ifndef DAMAGE_HPP
#define DAMAGE_HPP

namespace DBL
{
	Damage
	{
		Damage(int damage, int typeFlags):
		damageAmount(damage), damageTypes(typeFlags)
		{}
		int damageAmount;
		int damageTypes;
	};
};


#endif