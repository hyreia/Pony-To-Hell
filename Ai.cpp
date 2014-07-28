#include "main.hpp"

using namespace DBL;

const char *Ai::aiType = "Ai";
const char *PlayerControlledAi::type = "PlayerControlledAi";
const char *WanderAimlesslyAi::type = "WanderAimlesslyAi";
const char *VillagerAi::type = "VillagerAi";

Ai::Ai(const char *componentSecondaryType): ActorComponent(aiType),
componentSecondaryType(componentSecondaryType)
{}

Ai *Ai::NewAiFromDeserialization(std::vector<std::string> tokens)
{
	Ai *ret = NULL;

	if(tokens.size() >= 2)
	{
			 if(tokens[1] == PlayerControlledAi::type)	ret = new PlayerControlledAi();
		else if(tokens[1] == WanderAimlesslyAi::type)	ret = new WanderAimlesslyAi();
		else if(tokens[1] == VillagerAi::type)			ret = new VillagerAi();


		if(ret) ret->Deserialize(tokens);
	}

	return ret;
}



std::string PlayerControlledAi::Serialize()
{
	std::ostringstream serial;
	serial << ACT_COMP << componentType << DELIM << type;

	return serial.str();
}

void PlayerControlledAi::Deserialize(std::vector<std::string> tokens)
{
	//Don't need to load anything for now
}

void PlayerControlledAi::Act(Actor *owner)
{
	hasActedRecently = false;
	//This is where the player would check the input they were given
	//If they were to attack something it would happen here

	
}


void PlayerControlledAi::RespondToMouseClick(Actor *owner, int tileClickedX, int tileClickedY, bool isLeftClick)
{
	if(!hasActedRecently)
	{
		hasActedRecently = true;

		int targetActorId = game->world.IsActorAtPosition(tileClickedX, tileClickedY, owner->position->depth);
			

		auto box = isLeftClick? 
			game->world.combatUserInterface.GetLeftClick(): 
			game->world.combatUserInterface.GetRightClick();


			//there's an actor there
			if(targetActorId)
			{
				auto targetActor = game->world.actorCollection.GetActor(targetActorId);
				if(targetActor->villager && isLeftClick)
				{
					//left clicks on villagers are always to talk
					//are we close enough to talk to the villager?
					//TODO: get rid of the hardcoded 2, check line of sight and give reasonable distance away
					if(owner->position->tile.distanceSquared(targetActor->position->tile) < 2)
					{
						game->SetNextScreen(new TalkToActorMenu(targetActorId));
					}
				}
				else
				{
					//TODO: tell combat system player is attacking unarmed
					CombatEvent ev(owner->id, tileClickedX, tileClickedY, box.itemId, box.abilityId, isLeftClick? 0: 1);
					game->combatSystem.QueueCombatEvent(ev);
					
				}
			}
			else //no actor there but try doing something other than moving?
			{
				CombatEvent ev(owner->id, tileClickedX, tileClickedY, box.itemId, box.abilityId, isLeftClick? 0: 1);
				game->combatSystem.QueueCombatEvent(ev);
			}

	}

	
}
	/*
	if(isHoldingStill) //don't let player move
	{
		if(owner->movement->isMoving)
			owner->movement->destination == owner->position->tile;
	}*/
	/*
	int targetActorId = game->world.IsActorAtPosition(tileClickedX, tileClickedY, owner->position->depth);
	if(targetActorId)
	{
		auto targetActor = game->world.actorCollection.GetActor(targetActorId);
		if(targetActor != 0)
		{
			//If close enough we can interact with them			
			//If target is a villager
			if(targetActor->villager)
			{
				//Close enough to talk to?
				//If villager is in a combatant you'll want a different menu than a talk one
				//TODO: get rid of the hardcoded 3, check line of sight and give reasonable distance away
				 if(owner->position->tile.distanceSquared(targetActor->position->tile) < 2)
				 {
					 game->SetNextScreen(new TalkToActorMenu(targetActorId));
				 }
				 else
				 {
					 //Too far away to talk to
					 game->movingActors.RegisterActorToMove(owner->id, tileClickedX, tileClickedY);
				 }	
			}
			//If target is a hostile monster
				//TODO: determine how far away player can attack and if they have line of sight
			//If target is some non-descript actor (an object on the floor?)
			else
			{
				game->movingActors.RegisterActorToMove(owner->id, tileClickedX, tileClickedY);
			}
		}
	}
	else
	{
		game->movingActors.RegisterActorToMove(owner->id, tileClickedX, tileClickedY);
	}
	*/



std::string WanderAimlesslyAi::Serialize()
{
	std::ostringstream serial;
	serial << ACT_COMP << componentType << DELIM << type;
	return serial.str();
}
void WanderAimlesslyAi::Deserialize(std::vector<std::string> tokens)
{
	//Doesn't need any additional information
}
void WanderAimlesslyAi::Act(Actor *owner)
{
	//Occassionally pick an adjacent tile, go there
	if(rng::OneOutOf(3))
	{ 
		bool isClear = false;
		int moveAttempts = TOTAL_DIRECTIONS;
		auto floor = game->world.levelCollection.levels[owner->position->depth].tiles;
		auto pos = owner->position->tile;
		int direction = rng::Rand(8); //mix a random first direction
		do
		{

			switch(direction)
			{
			case DIR_UP: //0
				if(pos.y > 0 && game->world.IsPositionClear(pos.x, pos.y-1, owner->position->depth))
				{
					pos.y--;
					isClear = true;
				}
				break;
			case DIR_DOWN: //4
				if(pos.y < (int)floor[0].size()-1 && game->world.IsPositionClear(pos.x, pos.y+1, owner->position->depth))
				{
					pos.y++;
					isClear = true;
				}
				break;
			case DIR_UPRIGHT: //1
				if(pos.y > 0 && pos.x < (int)floor.size()-1 && game->world.IsPositionClear(pos.x+1, pos.y-1, owner->position->depth))
				{
					pos.x++; pos.y--;
					isClear = true;
				}
				break;
			case DIR_DOWNLEFT: //5
				if(pos.x > 0 && pos.y < (int)floor[0].size()-1 && game->world.IsPositionClear(pos.x-1, pos.y+1, owner->position->depth))
				{
					pos.x--; pos.y++;
					isClear = true;
				}
				break;
			case DIR_RIGHT: //2
				if(pos.x < (int)floor.size()-1 && game->world.IsPositionClear(pos.x+1, pos.y, owner->position->depth))
				{
					pos.x++;
					isClear = true;
				}
				break;
			case DIR_LEFT: //6
				if(pos.x > 0 && game->world.IsPositionClear(pos.x-1, pos.y, owner->position->depth))
				{
					pos.x--;
					isClear = true;
				}
				break;
			case DIR_DOWNRIGHT: //3
				if(pos.x < (int)floor.size()-1 && pos.y < (int)floor[0].size()-1 && game->world.IsPositionClear(pos.x+1, pos.y+1, owner->position->depth))
				{
					pos.x++; pos.y++;
					isClear = true;
				}
				break;
			case DIR_UPLEFT: //7
				if(pos.x > 0 && pos.y > 0 && game->world.IsPositionClear(pos.x-1, pos.y-1, owner->position->depth))
				{
					pos.x--; pos.y--;
					isClear = true;
				}
				break;
			}

			//check them all
			direction++;
			if(direction == TOTAL_DIRECTIONS) direction = DIR_UP;
		} while(!isClear && --moveAttempts > 0);
		
		if(isClear)
		{
			int id = owner->id;
			game->movingActors.RegisterActorToMove(owner->id, pos.x, pos.y, owner->position->depth);
		}
		//else not moving (the actor literally can't go any direction!)
		
	}
	//else do nothing
}



std::string VillagerAi::Serialize()
{
	std::ostringstream serial;
	serial << ACT_COMP << componentType << DELIM << 
		type << DELIM << 
		mentalState;


	return serial.str();
}
void VillagerAi::Deserialize(std::vector<std::string> tokens)
{
	mentalState = atoi(tokens[MENTAL_STATE].c_str());
}
void VillagerAi::Act(Actor *owner)
{
	switch(mentalState)
	{
	case VILLAGER_TIME_FOR_FUN: DoActivitiesAroundTheVillage();
		break;
	case VILLAGER_BEDTIME: GoToBedAndSleepUntilMorning();
		break;
	case VILLAGER_LISTENS_QUIETLY: //do nothing, someone is talking to you; pay attention
		break;
	case VILLAGER_TIME_FOR_WORK: PerformYourJob();
		break;
	case VILLAGER_TIME_FOR_COMBAT: ConcernYourselfWithCombat();
		break;
	}
	/* 
		Depending on the ai's state it wil either perform its villager functions,
		stand there while talking to the player, be combat focused, etc
	*/
}

void VillagerAi::DoActivitiesAroundTheVillage()
{
	//TODO:
	//If in the middle of an activity, finish it
		//by continuing on the current item of todo list
	//If it's bed time, go to bed
	//If it's time for work, go to work
	//If needing something to do, pick from list of things preferable to do
		//add required steps to todo list
}

void VillagerAi::GoToBedAndSleepUntilMorning()
{
	//TODO:
	//If it's time to wake up
		//create a todo list to get out of bed
		//set mental state to time for fun
	//If it's past bed time
		//if in own bed do nothing
		//else If not in bed go to own bed
}

void VillagerAi::PerformYourJob()
{
	//TODO:
	//If it's still time for work
		//stay in work area and wait for player to interact with you
	//If it's no longer time for work
		//it's time to have fun
}

void VillagerAi::ConcernYourselfWithCombat()
{
	//1st, 2nd, 3rd person order:

	//TODO and figure out how TODO
	//Check Self
		//are you hurt?
			//determine options
		//are you in danger?
			//determine options
	//are you near a hostile creature?
		//decide what to do
		//are you way too far from player?
	//or are you near the player?
		//stay near the player
	//Check player
		//is player hurt?
		//is player in danger?
	//How are the monsters?
		//is any of them about to do give up and can you finish them off?
		//are any of them extremely dangerous?
			//should they be avoided or nerfed?

	//Determine most appropriate action and do it
		
}

