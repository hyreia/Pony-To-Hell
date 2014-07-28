#ifndef MOVE_HPP
#define MOVE_HPP

#include <set>

//The class that actors that want to move register with
namespace DBL
{
	class ActorMovementManager
	{
	public:
		ActorMovementManager();
		~ActorMovementManager();

		/* This tells the actor to move to the destination passed in on the same depth
		 The actor is on, later there will be one to move to a place on any floor */
		void RegisterActorToMove(int actor, int destX, int destY);

		void RegisterActorToMove(int actor, int destX, int destY, int destDepth);

		void RegisterActorToMoveTowardsActor(int actor, int targetActor);
		void RegisterMovingActorsAfterReloading();

		//List of actors which are currently trying to move somewhere
		std::set<int> actorsMoving;
		
		void MoveActors();

	private:
		void SetActorDestinationPosToTheirTargetActorPosition(Actor *actor);
	};
};

#endif