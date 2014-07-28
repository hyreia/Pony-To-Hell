#ifndef WORLD_HPP
#define WORLD_HPP

namespace DBL
{
	/* Where the game content is stored, so we don't clutter the Game class so much */
	struct World
	{
		void ProcessCombatEvent(CombatEvent *ev);

		bool IsPositionClear(int tileX, int tileY, int depth);

		//returns first actor id or 0 for no actor
		int IsActorAtPosition(int tileX, int tileY, int depth);
		std::set<int> ActorsInParty;

		ActorCollection actorCollection;
		LevelCollection levelCollection;

		Materials materials;
		Species species;
		Items items;

		CombatUserInterface combatUserInterface;
		MessageQueue messageQueue;


	};
};

#endif