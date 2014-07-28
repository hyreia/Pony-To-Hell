#include "main.hpp"

using namespace DBL;


bool World::IsPositionClear(int tileX, int tileY, int depth)
{
	if(tileX < 0) return false;
	if(tileY < 0) return false;
	if(tileX > (int)levelCollection.levels[depth].tiles.size()-1) return false;
	if(tileY > (int)levelCollection.levels[depth].tiles[0].size()-1) return false;

	XYPair<> tilePos(tileX, tileY);
	//Tiles
	if(!levelCollection.IsLevelAtIndex(depth)) return false;
	int positionMaterial = levelCollection.levels[depth].tiles[tileX][tileY].tileMaterialIndex;
	if(materials.Get(positionMaterial).isBlockingMovement) return false;

	//Actors
	for(auto actorIter = actorCollection.actors.begin(); actorIter != actorCollection.actors.end(); actorIter++)
	{
		auto actor = (actorIter->second);

		if(
			actor->position && 
			actor->position->tile == tilePos && 
			actor->position->depth == depth &&
			actor->body
			)
		{
			return false;
		}
	}

	return true;
}

int World::IsActorAtPosition(int tileX, int tileY, int depth)
{
	int ret = 0;
	for(auto actorIter = actorCollection.actors.begin(); actorIter != actorCollection.actors.end(); actorIter++)
	{
		auto actor = (actorIter->second);

		if(actor->position && 
			actor->position->tile.x == tileX && 
			actor->position->tile.y == tileY && 
			actor->position->depth == depth)
		{
			//if we haven't found one yet, set this one
			if(ret == 0) ret = actorIter->first;
			//if we have, replace it with this one if that one doesn't have a body and this one does
			//(A body signifies a complex being that we might talk to or fight)
			if(ret != 0)
			{
				if(actor->body && !actorCollection.GetActor(ret)->body)
				{ ret = actorIter->first; }
			}
		}
	}

	return ret;


}

void World::ProcessCombatEvent(CombatEvent *ev)
{
	auto actor = game->world.actorCollection.GetActor(ev->actorId);

	auto targetActor = IsActorAtPosition(ev->x, ev->y, actor->position->depth);

	//if there is an item, we're using it's primary use or second use
	if(ev->itemId != 0)
	{

	}
	else //the actor is performing an unarmed attack
	{

	}
}