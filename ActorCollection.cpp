#include "main.hpp"

using namespace DBL;

ActorCollection::ActorCollection(): nextAvailableId(1), PLAYER_ACTOR_ID(1)
{}
ActorCollection::~ActorCollection(){}

int ActorCollection::GetNextAvailableId()
{
	return nextAvailableId++;
}

int ActorCollection::InsertActor(Actor *actor)
{
	//Find vacant slot
	//while(actors.count(nextAvailableId)==1) nextAvailableId++;

	InsertActor(actor->id, actor);

	return actor->id;;
}

void ActorCollection::InsertActor(int id, Actor *actor)
{
	if(actors.count(id)==1) delete actors[id];
	//actor->id = nextAvailableId;
	actors.insert(std::pair<int, Actor*>(id, actor));
}

Actor *ActorCollection::GetActor(int id)
{
	if(actors.count(id) == 1) return actors[id];
	else return NULL;
}

Actor *ActorCollection::GetPlayer()
{
	if(actors.count(PLAYER_ACTOR_ID) == 1) return actors[PLAYER_ACTOR_ID];
	else return NULL;
}

void ActorCollection::EraseActor(int id)
{
	if(actors.count(id) == 1)
	{
		delete actors[id];
	}
}

bool ActorCollection::MoveActorToActiveList(int id)
{
	if(inactiveActors.count(id) == 1)
	{
		Actor *actor = inactiveActors[id];
		inactiveActors.erase(id);
		actors.insert(std::pair<int, Actor*>(id, actor));

		return true;
	}
	else return false;
}

void ActorCollection::MoveActorToInactiveList(int id)
{
	if(actors.count(id) == 1)
	{
		Actor *actor = actors[id];
		actors.erase(id);
		inactiveActors.insert(std::pair<int, Actor*>(id, actor));
	}
}

void ActorCollection::FreeAllActors()
{
	for(auto actor = actors.begin(); actor != actors.end(); actor++)
	{
		delete (*actor).second;
	}
	actors.clear();

	for(auto actor = inactiveActors.begin(); actor != inactiveActors.end(); actor++)
	{
		delete (*actor).second;
	}
	inactiveActors.clear();
}

bool ActorCollection::LoadActorsFromFile()
{
	Actor *pendingActor = NULL;
	int actorId = nextAvailableId;
	//try to get file
	ALLEGRO_FILE *file = al_fopen("actors.act", "r");
	if(file)
	{
	//if we have an open file
		const int BUF_SIZE = 512;
		char buffer[BUF_SIZE];
		while(al_fgets(file, buffer, BUF_SIZE))
		{
			std::string str = buffer;
			if (!str.empty() && str[str.length()-1] == '\n') str.erase(str.length()-1);
			
			//if a line starts with a tab, it's a component to actor being assembled
			if(buffer[0] == ActorComponent::ACT_COMP && pendingActor)
			{
				str = str.substr(1); //removes the ACT_COMP character 
				auto tokenized = split(str, ActorComponent::DELIM);

				if(!tokenized.empty())
				{
					//ACTOR IMAGES
					if( strcmp(tokenized[0].c_str(), ActorImages::actorImages) == 0)
					{
						pendingActor->images = new ActorImages();
						pendingActor->images->Deserialize(tokenized);
					}
					//POSITION
					else if( strcmp(tokenized[0].c_str(), Position::position) == 0)
					{
						pendingActor->position = new Position();
						pendingActor->position->Deserialize(tokenized);
					}
					//AI
					else if( strcmp(tokenized[0].c_str(), Ai::aiType) == 0)
					{
						pendingActor->ai = Ai::NewAiFromDeserialization(tokenized);
					}
					//MOVEMENT
					else if( strcmp(tokenized[0].c_str(), Movement::movement) == 0)
					{
						pendingActor->movement = new Movement();
						pendingActor->movement->Deserialize(tokenized);
					}
					//COMBATANT
					else if( strcmp(tokenized[0].c_str(), Combatant::combatant) == 0)
					{
						pendingActor->combatant = new Combatant();
						pendingActor->combatant->Deserialize(tokenized);
					}
					//VILLAGER
					else if( strcmp(tokenized[0].c_str(), Villager::villager) == 0)
					{
						pendingActor->villager = new Villager();
						pendingActor->villager->Deserialize(tokenized);
					}
					//CONTAINS
					//CARRYABLE
					//BODY
					else if( strcmp(tokenized[0].c_str(), Body::body) == 0)
					{
						pendingActor->body = new Body();
						pendingActor->body->Deserialize(tokenized);
					}
					//NAME
				}
				//use the first and maybe second parts to determine the component type
				//deserialize a component of that type with the tokenized string
				//add it to the actor being assembled
			}
			else if(buffer[0] == '#' || buffer[0] == ';'){ /* is comment; ignore */}
			else //it must be for an actor
			{
				//add any pending actor being assembled to the map, clear pending space
				if(pendingActor)
				{
					InsertActor(actorId, pendingActor);
					if(actorId == PLAYER_ACTOR_ID)
					{
						game->SetGameLevel(pendingActor->position->depth);
					}
					pendingActor = NULL;
				}
				//parse out the id
				auto parseOutId = split(str, '|');
				if(parseOutId.size() > 1)
				{
					actorId = atoi(parseOutId[0].c_str());
					auto tokenized = split(parseOutId[1], ActorComponent::DELIM);
					pendingActor = new Actor(actorId);
					pendingActor->Deserialize(tokenized);
				}
				//else syntax is wrong, either malformed or not actor line, move along
				
			}
		}
	//Once the file has been fully read, close it
	al_fclose(file);
	}
	else
	{
		return false;
	}
	//If we have any pending actors, add them to the map
	if(pendingActor)
	{
		InsertActor(actorId, pendingActor);
		if(actorId == PLAYER_ACTOR_ID)
		{
			game->SetGameLevel(pendingActor->position->depth);
		}
		pendingActor = NULL;
	}

	return true;
}

void ActorCollection::MakeActorsAtDepthActive(int depth)
{
	for(auto actorIter = game->world.actorCollection.inactiveActors.begin();
		actorIter != game->world.actorCollection.inactiveActors.end(); /* */ )
	{
		auto actor = actorIter->second;

		//if actor has a position
		if(actor->position)
		{
			//if on the next level
			if(actor->position->depth == depth)
			{
				//add to active list, remove from inactive list
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
}

void ActorCollection::MakeAllActorsActive()
{
	for(auto actorIter = game->world.actorCollection.inactiveActors.begin();
		actorIter != game->world.actorCollection.inactiveActors.end(); /* */ )
	{
		auto actor = actorIter->second;

		//add to active list, remove from inactive list
		game->world.actorCollection.actors.insert(std::pair<int, Actor*>(actor->id,
			actor));
		actorIter = game->world.actorCollection.inactiveActors.erase(actorIter);

	}
}

void ActorCollection::MakeAllActorsInactive()
{
	for(auto actorIter = game->world.actorCollection.actors.begin();
		actorIter != game->world.actorCollection.actors.end(); /* */ )
	{
		auto actor = actorIter->second;

		//add to active list, remove from inactive list
		game->world.actorCollection.inactiveActors.insert(std::pair<int, Actor*>(actor->id,
			actor));
		actorIter = game->world.actorCollection.actors.erase(actorIter);

	}
}

#pragma warning(push)
#pragma warning(disable: 4996) //vsprintf is unsafe and depricated but vsprintf_s is nonstandard
void ActorCollection::SaveActorsToFile()
{
	//Open actors file in text mode
	ALLEGRO_FILE *file = al_fopen("actors.act", "w");

	//Iterate through each actor
	for(auto actor = actors.begin(); actor != actors.end(); actor++)
	{
		char id_buffer[13];
		sprintf(id_buffer, "%d", (*actor).first);
		//Get serialize string for Actor class
		std::string actorLine = (*actor).second->Serialize();
		//Insert the actor ID and actorId separator
		actorLine.insert(0, "|");
		actorLine.insert(0, id_buffer);
		actorLine.append("\n");
		//Write it to the file
		al_fwrite(file, actorLine.c_str(), strlen(actorLine.c_str()));

		//Then serialize each component of theirs
		//IMAGES
		if(actor->second->images)
		{
			//Writing each of those components to the file as well
			std::string str = actor->second->images->Serialize();
			str.append("\n");
			al_fwrite(file, str.c_str(), strlen(str.c_str()));
		}
		//POSITION
		if(actor->second->position)
		{
			//Writing each of those components to the file as well
			std::string str = actor->second->position->Serialize();
			str.append("\n");
			al_fwrite(file, str.c_str(), strlen(str.c_str()));
		}
		//AI
		if(actor->second->ai)
		{
			std::string str = actor->second->ai->Serialize();
			str.append("\n");
			al_fwrite(file, str.c_str(), strlen(str.c_str()));
		}
		//MOVEMENT
		if(actor->second->movement)
		{
			//Writing each of those components to the file as well
			std::string str = actor->second->movement->Serialize();
			str.append("\n");
			al_fwrite(file, str.c_str(), strlen(str.c_str()));
		}
		//COMBATANT
		if(actor->second->combatant)
		{
			std::string str = actor->second->combatant->Serialize();
			str.append("\n");
			al_fwrite(file, str.c_str(), strlen(str.c_str()));
		}
		//VILLAGER
		if(actor->second->villager)
		{
			std::string str = actor->second->villager->Serialize();
			str.append("\n");
			al_fwrite(file, str.c_str(), strlen(str.c_str()));
		}
		//CONTAINS
		//CARRYABLE
		//BODY
		if(actor->second->body)
		{
			std::string str = actor->second->body->Serialize();
			str.append("\n");
			al_fwrite(file, str.c_str(), strlen(str.c_str()));
		}
		//NAME
		
			
	}
	
	al_fclose(file);

}
#pragma warning(pop)

std::list<Actor*> ActorCollection::GetActorsInDrawOrder()
{
	std::list<Actor*> ret;
	std::list<Actor*> actorsOnGround;

	//Add active actors to a list if they have a position (and thus a place to be drawn)
	//Also if they are on the ground put them on ground list
	//If they are in the air, add them to end of ret list, if they are standing,
	//add them to the front of the ret list
	//Add the actorsOnGround list to the front of the ret list before returning
	//this puts them in the proper draw order when iterating from front to back

	for(auto actorIter = actors.begin(); actorIter != actors.end(); actorIter++)
	{
		if(actorIter->second->position)
		{
			auto actor = (actorIter->second);
			if(actor->position->elevation == ON_GROUND)
			{
				actorsOnGround.push_back(actor);
			}
			else if(actor->position->elevation == STANDING)
			{
				ret.push_front(actor);
			}
			else if(actor->position->elevation == IN_AIR)
			{
				ret.push_back(actor);
			}
		}

	}

	ret.insert(ret.begin(), actorsOnGround.begin(), actorsOnGround.end());

	return ret;
}