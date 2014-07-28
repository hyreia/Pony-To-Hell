#ifndef COMBAT_EVENT_HPP
#define COMBAT_EVENT_HPP

namespace DBL
{

	struct CombatEvent
	{
		CombatEvent(int actorId, int x, int y, int itemId = 0, int abilityId = 0, int use = 0)
		{
			this->actorId = actorId;
			this->itemId = itemId;
			this->abilityId = abilityId;
			this->x = x; this->y = y;
			this->use = use;
		}

		/* The actor who instantiated the combat event*/
		int actorId;

		/* Optional, gives the index of item being used. If no item is provided actor is assumed to be doing
		 a default unarmed attack for its kind. Use is 0 for primary, 1 is secondary */
		int itemId, abilityId, use;
		/*position event is targeted towards (at area OR actor at this position) may be irrelevant if item
		does not use a position or if location is deemed irrelevant */
		int x, y; 

	};
};

#endif