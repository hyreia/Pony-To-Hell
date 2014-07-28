#include "main.hpp"

using namespace DBL;

ActorMovementManager::ActorMovementManager(){}
ActorMovementManager::~ActorMovementManager(){}

void ActorMovementManager::RegisterActorToMove(int actorId, int destX, int destY)
{
	if(game->world.actorCollection.actors.count(actorId) == 1)
	{
		auto actor = game->world.actorCollection.actors.at(actorId);
		if(actor->position && actor->movement)
		{
			if(destX < 0) destX = 0;
			else if(destX >= (int)game->world.levelCollection.levels[actor->position->depth].tiles.size())
			{
				destX = (int)game->world.levelCollection.levels[actor->position->depth].tiles.size()-1;
			}
			else if(destY >= (int)game->world.levelCollection.levels[actor->position->depth].tiles[0].size()-1)
			{
				destY = (int)game->world.levelCollection.levels[actor->position->depth].tiles[0].size()-1;
			}

			//set it to move to this location at its determined move speed
			actor->movement->destination.x = destX;
			actor->movement->destination.y = destY;
			actor->movement->destinationDepth = actor->position->depth;
			if(destX < (int)game->world.levelCollection.levels[actor->position->depth].tiles.size() &&
				destY < (int)game->world.levelCollection.levels[actor->position->depth].tiles[0].size() &&
				destX >= 0 && destY >= 0)
			{
				int material = game->world.levelCollection.levels[actor->position->depth].tiles[destX][destY].tileMaterialIndex;
				if(material == DOWN_STAIRS)
				{
					actor->movement->destinationDepth++;
				}
				if(material == UP_STAIRS)
				{
					actor->movement->destinationDepth--;
				}
			

				//Add it to the moving list if it's not already in it
				if( actorsMoving.count(actorId) == 0)
				{
					actor->movement->isMoving = true;
					actorsMoving.insert(actorId);
				}
			}
			//else actor can't go there
		}
	}
	
}

void ActorMovementManager::RegisterActorToMove(int actorId, int destX, int destY, int destDepth)
{
	/* There is a slight flaw with this right now: you can register to move to different floors but you can't choose a spot
	on a different floor, as the game uses the same variables you would store your destination in, to store the stair position
	
	So you'll arrive at the stairs on your choosen floor and be done. How would this be solved? instead of overwriting your dest
	on the final depth, it would check the destination depth before trying to move you 
	and if your destination depth is different from your current depth, get the stair position itself instead of assuming you have it

	"BUG FIX TODO". This probably isn't going to even be apparent though, and it'll look like the actor "forgot what it was doing"
	when it reached its floor. Assumingly it'll figure it out when it gets there. This is because of the code above
	*/

	if(game->world.actorCollection.actors.count(actorId) == 1)
	{
		auto actor = game->world.actorCollection.actors.at(actorId);
		if(actor->position && actor->movement)
		{
			if(destX < 0) destX = 0;
			else if(destX >= (int)game->world.levelCollection.levels[destDepth].tiles.size())
			{
				destX = (int)game->world.levelCollection.levels[destDepth].tiles.size()-1;
			}

			if(destY < 0) destY = 0;
			else if(destY >= (int)game->world.levelCollection.levels[destDepth].tiles[0].size()-1)
			{
				destY = (int)game->world.levelCollection.levels[destDepth].tiles[0].size()-1;
			}

			if(destDepth == actor->position->depth)
			{
				RegisterActorToMove(actorId, destX, destY);
			}
			else if(destDepth > actor->position->depth) //we need to go deeper
			{
				//to the down stairs
				if(game->world.levelCollection.levels[actor->position->depth].PosOfDownStairs)
				{
					actor->movement->destination = (*game->world.levelCollection.levels[actor->position->depth].PosOfDownStairs);
					//Add it to the moving list if it's not already in it
					if( actorsMoving.count(actorId) == 0)
					{
						actor->movement->isMoving = true;
						actorsMoving.insert(actorId);
					}					
				}
				//else we can't though, do nothing

			}
			else if(destDepth < actor->position->depth) //we need to go up
			{
				//to the down stairs
				if(game->world.levelCollection.levels[actor->position->depth].PosOfUpStairs)
				{
					actor->movement->destination = (*game->world.levelCollection.levels[actor->position->depth].PosOfUpStairs);
					//Add it to the moving list if it's not already in it
					if( actorsMoving.count(actorId) == 0)
					{
						actor->movement->isMoving = true;
						actorsMoving.insert(actorId);
					}					
				}
				//else we can't though, do nothing
			}




		}
	}
}

void ActorMovementManager::RegisterActorToMoveTowardsActor(int actorId, int targetActorId)
{
	if(game->world.actorCollection.actors.count(actorId) == 1 && game->world.actorCollection.actors.count(targetActorId))
	{
		auto actor = game->world.actorCollection.actors.at(actorId);
		auto targetActor = game->world.actorCollection.actors.at(targetActorId);
		if(actor->position && actor->movement && targetActor->position)
		{	
			RegisterActorToMove(actorId, targetActor->position->tile.x, targetActor->position->tile.y,
				targetActor->position->depth);
		}
	}
}


void ActorMovementManager::RegisterMovingActorsAfterReloading()
{
	for(auto actorIter = game->world.actorCollection.actors.begin();
		actorIter != game->world.actorCollection.actors.end(); actorIter++)
	{
		if(actorIter->second->movement && actorIter->second->movement->isMoving)
		{
			actorsMoving.insert(actorIter->first);
		}
	}
}

void ActorMovementManager::MoveActors()
{
	

	/* The general idea: 
		*try to make their tile pos match their destination tile position
		*in order to increment tile pos to match destination tile pos,
		*absolute pos needs to match tile pos (by pixel, so centered)
		*we slowly change absolute pos towards tile pos

		*when changing tile pos the tile it's being changed to needs to be clear 
			(clear = no actors there and not blocking movement)
		*to change tile pos diagonally, the adjacent spaces need to be clear as well as destination
		*/

	//For each actor
	//printf("%d\n", actorsMoving.size());

	for(auto actorIdIter = actorsMoving.begin(); actorIdIter != actorsMoving.end(); /* */)
	{
		if( game->world.actorCollection.actors.count( (*actorIdIter) ) == 1)
		{
			auto actor = game->world.actorCollection.actors.at( (*actorIdIter) );
			
			
			//If actor's absolute position the same as their tile location
			if(actor->position->absolutePos == TileToAbsolutePos(actor->position->tile))
			{
				//They're ready to move their tile position closer to their destination tile position

				//If there destination was another actor, set their destination tile to target actor's tile pos
				if(actor->movement->targetActorId != 0) SetActorDestinationPosToTheirTargetActorPosition(actor);


				//Determine tile to move into to move closer to destination
				if(actor->position->tile.x != actor->movement->destination.x ||
					actor->position->tile.y != actor->movement->destination.y)
				{

					auto newPosition = actor->position->tile;
					bool isBlocked = true;

					/* By allowing movement in both x and y axis, we intentionally allow
					diagonal movement, but only if the tiles we pass through to get there are 
					clear at the time we decide to move. if one is blocked, we won't move diagonally but
					only in the clear direction */

					//need to move left
					if(actor->position->tile.x > actor->movement->destination.x)
					{
						//if that tile is clear, move into that tile
						if(game->world.IsPositionClear(actor->position->tile.x-1, actor->position->tile.y,
							actor->position->depth))
						{
							newPosition.x--;
							isBlocked = false;
						}
					}
					//need to move right
					else if(actor->position->tile.x < actor->movement->destination.x)
					{
						//if that tile is clear, move into that tile
						if(game->world.IsPositionClear(actor->position->tile.x+1, actor->position->tile.y,
							actor->position->depth))
						{
							newPosition.x++;
							isBlocked = false;
						}
					}

					//need to move up
					if(actor->position->tile.y > actor->movement->destination.y)
					{
						//if that tile is clear, move into that tile
						if(game->world.IsPositionClear(actor->position->tile.x, actor->position->tile.y-1,
							actor->position->depth))
						{
							newPosition.y--;
							isBlocked = false;
						}
					}
					//need to move down
					if(actor->position->tile.y < actor->movement->destination.y)
					{
						//if that tile is clear, move into that tile
						if(game->world.IsPositionClear(actor->position->tile.x, actor->position->tile.y+1,
							actor->position->depth))
						{
							newPosition.y++;
							isBlocked = false;
						}
					}

					if(newPosition.x != actor->position->tile.x &&
						newPosition.y != actor->position->tile.y &&
						!game->world.IsPositionClear(newPosition.x, newPosition.y, actor->position->depth))
					{
							//new diagonal position isn't valid, pick just one direction to move towards
						int direction = rng::Rand(2);
						if(direction == 0) newPosition.x = actor->position->tile.x;
						if(direction == 1) newPosition.y = actor->position->tile.y;
					}

					actor->position->tile = newPosition;

					if(!isBlocked) actorIdIter++;
					else
					{
						actorIdIter = actorsMoving.erase(actorIdIter);
					}

				}
				else //or they can't move closer, so they're at immediate destination
				{
					//if on the right floor, we're done
					if(actor->movement->destinationDepth == actor->position->depth)
					{
						actorIdIter = actorsMoving.erase(actorIdIter);
					}
					else //we've pathed to stairs (presumably from following an actor)
					{
						bool isPositionClear = false;
						int nextFloor = actor->position->depth;

						//at lower depth, we're on stairs going up
						if(actor->position->depth > actor->movement->destinationDepth)
						{
							nextFloor--;
						}
						//we're closer to surface, we're on stairs going down
						else if(actor->position->depth < actor->movement->destinationDepth)
						{
							nextFloor++;
						}
						
						//
						if(game->world.levelCollection.IsLevelAtIndex(nextFloor))
						{
							auto actorTilePos = actor->position->tile;
							/* The tile material will be clear (it's stairs going the opposite way, right?)
							 So all we need to do is test if there are any actors on that tile that block

							 Alt: if we don't let other actors block floor changing, then monsters can't be blocked by player
							 and neither can villagers by the player, they just end up stacking on one tile
							 while you can't move into a tile being blocked, there are no rules against being in a blocked tile
							 Allow the change regardless
							*/
							actor->position->depth = nextFloor;
							if(actor->id == game->world.actorCollection.PLAYER_ACTOR_ID)
							{
								game->SetNextScreen(new LevelTransitionScreen());
							}
						}
						else //no such level
						{
							actor->movement->destinationDepth = actor->position->depth;
							//next frame, will be at its destination

						}
						
						actorIdIter++;	
					}

				}



			}
			else //actor's absolute position needs to be moved towards their tile pos location
			{
				auto tilePos = TileToAbsolutePos(actor->position->tile);
				actor->position->absolutePos;

				//their maximum move distance
				double moveDistance = actor->movement->baseMoveSpeed;
				double leftOverMovement = actor->movement->carryOverMomentum;
				actor->movement->carryOverMomentum = 0;

				leftOverMovement = 0;

				//Need to correct position diagonally
				if(tilePos.x != actor->position->absolutePos.x &&
					tilePos.y != actor->position->absolutePos.y)
				{
					moveDistance *= DIAGONAL_MOVEMENT_COEFF;
				}


				//if actor's position is right of tile, move left
				if(actor->position->absolutePos.x > tilePos.x)
				{
					//if the distance actor can move isn't enough to get to tilePos, add moveDistance to absPos
					if(actor->position->absolutePos.x - moveDistance > tilePos.x)
					{
						actor->position->absolutePos.x -= moveDistance;
					}
					else //distance is too much, set absPos to tilePos
					{
						actor->position->absolutePos.x = tilePos.x;
						//the loss move distance should be negligible (or responsible for "clunky movement")
						if(leftOverMovement==0) leftOverMovement = actor->position->absolutePos.x - moveDistance - tilePos.x;
					}
					
				}
				//if actor's position is left of tile, move right
				else if(actor->position->absolutePos.x < tilePos.x)
				{
					//if the distance actor can move isn't enough to get to tilePos, add moveDistance to absPos
					if(actor->position->absolutePos.x + moveDistance < tilePos.x)
					{
						actor->position->absolutePos.x += moveDistance;
					}
					else //distance is too much, set absPos to tilePos
					{
						actor->position->absolutePos.x = tilePos.x;
						if(leftOverMovement==0) leftOverMovement = actor->position->absolutePos.x + moveDistance - tilePos.x;
						//the loss move distance should be negligible (or responsible for "clunky movement")
					}
				}

				//if actor's position is down of tile, move up
				if(actor->position->absolutePos.y > tilePos.y)
				{
					//if the distance actor can move isn't enough to get to tilePos, add moveDistance to absPos
					if(actor->position->absolutePos.y - moveDistance > tilePos.y)
					{
						actor->position->absolutePos.y -= moveDistance;
					}
					else //distance is too much, set absPos to tilePos
					{
						actor->position->absolutePos.y = tilePos.y;
						if(leftOverMovement==0) leftOverMovement =  actor->position->absolutePos.y - moveDistance - tilePos.y;
						//the loss move distance should be negligible (or responsible for "clunky movement")
					}
					
				}
				//if actor's position is up of tile, move down
				else if(actor->position->absolutePos.y < tilePos.y)
				{
					//if the distance actor can move isn't enough to get to tilePos, add moveDistance to absPos
					if(actor->position->absolutePos.y + moveDistance < tilePos.y)
					{
						actor->position->absolutePos.y += moveDistance;
					}
					else //distance is too much, set absPos to tilePos
					{
						actor->position->absolutePos.y = tilePos.y;
						if(leftOverMovement==0) leftOverMovement = actor->position->absolutePos.y + moveDistance - tilePos.y;
						//the loss move distance should be negligible (or responsible for "clunky movement")
					}
				}


				actor->movement->carryOverMomentum = leftOverMovement;
				
				actorIdIter++;
			}

		}
		else //can't find actor, doesn't exist or is in active, remove from the list
		{
			actorIdIter = actorsMoving.erase(actorIdIter);
		}
	}
}

void ActorMovementManager::SetActorDestinationPosToTheirTargetActorPosition(Actor *actor)
{
	if(game->world.actorCollection.actors.count(actor->movement->targetActorId) == 1)
	{
		int targetActorId = actor->movement->targetActorId;
		auto targetActor = game->world.actorCollection.actors.at(targetActorId);	
		if(targetActor->position) //sanity check
		{
			//move towards target
			if(actor->position->depth == targetActor->position->depth)
			{
				//on the same floor
				actor->movement->destination = targetActor->position->tile;
			}
			else
			{
				//target is at lower depth and there are stairs down
				if(actor->position->depth > targetActor->position->depth &&
					game->world.levelCollection.levels[actor->position->depth].PosOfDownStairs)
				{
					actor->movement->destination = (*game->world.levelCollection.levels[actor->position->depth].PosOfDownStairs);
					actor->movement->destinationDepth = targetActor->position->depth;
				}
				//target is at higher depth and there are stairs up
				else if(actor->position->depth < targetActor->position->depth &&
					game->world.levelCollection.levels[actor->position->depth].PosOfUpStairs)
				{
					actor->movement->destination = (*game->world.levelCollection.levels[actor->position->depth].PosOfUpStairs);
					actor->movement->destinationDepth = targetActor->position->depth;
				}
				else //there are no stairs in that direction, stuck
				{
					//say it's at its destination so its removed from moving actor list 
					actor->movement->destination = actor->position->tile;
				}
			}
		}
		else //this target doesn't exist anywhere (it has no position
		{
			//say it's at its destination so its removed from the moving actor list 
			actor->movement->destination = actor->position->tile;
		}
	}
	else //this target actor doesn't exist or is no longer active
	{
		//say actor is at its destination so its removed from the moving actor list 
		actor->movement->destination = actor->position->tile;
	}
}