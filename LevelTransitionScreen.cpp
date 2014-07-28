#include "main.hpp"

using namespace DBL;

LevelTransitionScreen::LevelTransitionScreen():
transitionState(FADING_OUT)
{
	auto playerPosition = game->world.actorCollection.GetActor(
				game->world.actorCollection.PLAYER_ACTOR_ID)->position;
	nextLevel = playerPosition->depth;

	//set to temporary other level
	playerPosition->depth = game->GetGameLevel();
	transitionTime = game->GetGameTime();
}

LevelTransitionScreen::~LevelTransitionScreen()
{}

//no input processed
void LevelTransitionScreen::HandleInput(ALLEGRO_EVENT &ev){}

void LevelTransitionScreen::Output()
{
	const int OPAQUE_COLOR = 255;

	if(transitionState == FADING_OUT)
	{

		// x out of 60 frames passed
		unsigned char fadeOutAmount = float(game->GetGameTime() - transitionTime) / float(game->FPS) * OPAQUE_COLOR;
		game->DrawSimulation();

		Color fade(0, 0, 0, fadeOutAmount);
		game->graphics.ClearToColor(fade);
		if(fadeOutAmount == OPAQUE_COLOR)
		{
			transitionState = LOADING;
		} //else do nothing else
	}
	else if(transitionState == LOADING)
	{
		int lastLevel = game->world.actorCollection.GetActor(
				game->world.actorCollection.PLAYER_ACTOR_ID)->position->depth;

		//Load level player is on if necessary
			//TODO: create levels that need creating here
		
		//set all NPCs not following player or players party to inactive
		for(auto actorIter = game->world.actorCollection.actors.begin();
			actorIter != game->world.actorCollection.actors.end(); /* */ )
		{
			auto actor = actorIter->second;

			//if actor has a position
			if(actor->position)
			{
				//if on the previous last level, is not in player's party or player
				if(actor->position->depth == lastLevel)
				{
					bool isNowInactive = true;
					//if it's moving
					if(actor->movement && actor->movement->isMoving)
					{
						//and its target is the player or in the player's party
						int targetId = actor->movement->targetActorId;
						if(game->world.ActorsInParty.count(targetId) || targetId == game->world.actorCollection.PLAYER_ACTOR_ID)
						{
							isNowInactive = false;
						}
						//else it's removed
					}
					//else it's removed
						
					if(isNowInactive)
					{
						//add to inactive list, remove from active list
						game->world.actorCollection.inactiveActors.insert(std::pair<int, Actor*>(actor->id,
							actor));
						actorIter = game->world.actorCollection.actors.erase(actorIter);
					}
				}
				else
				{
					actorIter++;
				}
			} //else we can just leave it always on (weapon equipped by someone?)
		}
		
		//set all NPCs on next level to active
		for(auto actorIter = game->world.actorCollection.inactiveActors.begin();
			actorIter != game->world.actorCollection.inactiveActors.end(); /* */ )
		{
			auto actor = actorIter->second;

			//if actor has a position
			if(actor->position)
			{
				//if on the next level
				if(actor->position->depth == nextLevel)
				{
					//add to active list, remove from omactive list
					game->world.actorCollection.actors.insert(std::pair<int, Actor*>(actor->id,
						actor));
					actorIter = game->world.actorCollection.inactiveActors.erase(actorIter);
				}
				else
				{
					actorIter++;
				}
			}
		}

		//simulate level until last active time catches up with current time or until time elapsed cap
		game->world.levelCollection.levels[lastLevel].lastTimeVisited = game->GetGameTime();
		const int MAX_SIMULATION_SECS = 30;
		const int MAX_SIMULATION_TIME = Game::FPS*MAX_SIMULATION_SECS;
		int timeSinceLastVisit = game->GetGameTime() - game->world.levelCollection.levels[nextLevel].lastTimeVisited;

		int64_t timeSimulated = min(MAX_SIMULATION_TIME, timeSinceLastVisit);
		while(timeSimulated > 0)
		{
			game->ProcessSimulation();
			timeSimulated--;
		}
		
		//set player back on level
		game->world.actorCollection.MoveActorToActiveList(game->world.actorCollection.PLAYER_ACTOR_ID);
		game->world.actorCollection.GetActor(game->world.actorCollection.PLAYER_ACTOR_ID)->position->depth
			= nextLevel;
		for(auto partyMemberIter = game->world.ActorsInParty.begin(); 
			partyMemberIter != game->world.ActorsInParty.end(); partyMemberIter++)
		{
			game->world.actorCollection.MoveActorToActiveList( (*partyMemberIter) );
		}
		transitionTime = game->GetGameTime();
		game->SetGameLevel(nextLevel);
		transitionState = FADING_IN;
	}
	else if(transitionState == FADING_IN)
	{
		// x out of 60 frames passed
		unsigned char fadeInAmount = float(game->GetGameTime() - transitionTime) / float(game->FPS) * OPAQUE_COLOR;
		game->DrawSimulation();

		Color fade(0, 0, 0, OPAQUE_COLOR-fadeInAmount);
		game->graphics.ClearToColor(fade);
		if(fadeInAmount == OPAQUE_COLOR)
		{
			game->SetNextScreen(new GameScreen());
		} //else do nothing else
	}
	

}

void LevelTransitionScreen::Process(){}