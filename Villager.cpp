#include "main.hpp"

using namespace DBL;

const char *Villager::villager = "Villager";

void Villager::Deserialize(std::vector<std::string> tokens)
{
	friendliness = atoi(tokens[FRIEND].c_str());
	adventureness = atoi(tokens[ADVENTURE].c_str());
	talkativeness = atoi(tokens[TALK].c_str());
	familiarity = atoi(tokens[FAMILIAR].c_str());
}

std::string Villager::Serialize()
{
	std::ostringstream serial;

	serial << ACT_COMP <<
		componentType << DELIM <<
		friendliness << DELIM <<
		adventureness << DELIM <<
		talkativeness << DELIM <<
		familiarity;

	return serial.str();
}

std::string Villager::Greet()
{
	std::string response;

	int greetingLevel = friendliness + familiarity + talkativeness*0.5;

	if(greetingLevel < 100)
	{
		response = "Fuck off";
	}
	else if(greetingLevel < 200)
	{
		response = "...";
	}
	else if(greetingLevel < 400)
	{
		response = "Hi";
	}
	else if(greetingLevel < 600)
	{
		response = "Hello!";
	}
	else
	{
		response = "Hey! How's it going?";
	}

	//reaction
	if(familiarity < 0) familiarity = 0;
	else if(familiarity < 100) familiarity += 100;
	else if(familiarity < 500)
	{
		familiarity += 50;
	}
	else if(familiarity > 600){}

	return response;
}

/* They would have more to talk about if they had some hobbies */
std::string Villager::TalkTo()
{
	std::string response;

	int conversationLevel = friendliness + familiarity*1.0 + talkativeness;
	if(familiarity == 0)
	{
		response = "Well, hello to you too, stranger.";
		familiarity += 15;
	}
	else if(conversationLevel < 200)
	{
		response = "Are you new around here?";
		friendliness += 50;
	}
	else if(conversationLevel < 400)
	{
		response = "What do you mean I'm bald?";
		friendliness += 50;
	}
	else if(conversationLevel < 800)
	{
		response = "So are you going to save us?";
		friendliness += 50;
		adventureness += 20;
	}
	else
	{
		response = "So what did you want to talk about?";
	}

	return response;
}

std::string Villager::Recruit()
{
	std::string response;

	int recruitLevel = adventureness*2.0 + familiarity*1.5 + friendliness;

	if(recruitLevel < 500)
	{
		response = "I'd probably just end up hurting you or myself.";
		adventureness -= 50;
	}
	else if(recruitLevel < 1000)
	{
		response = "Eeeh, I'm not much of a fighter.";
		adventureness += 50;
	}
	else if(recruitLevel < 2000)
	{
		response = "I'm a little busy right now. Maybe later?";
		adventureness += 50;;
	}
	else
	{
		response = "I want to, but I can't.";
		adventureness += 50;
	}

	return response;
}

std::string Villager::Flirt()
{
	std::string response;

	int flirtLevel = familiarity*2.0 + adventureness + friendliness*2.0;

	if(flirtLevel < 300)
	{
		response = "Um... do I know you?";
		friendliness -= 50;
		familiarity -= 25;
	}
	else if(flirtLevel < 500)
	{
		response = "Um... thanks?";
		friendliness += 25;
		familiarity += 25;
	}
	else if(flirtLevel < 1000)
	{
		response = "Haha, you're funny.";
		friendliness += 50;
		familiarity += 50;
	}
	else if(flirtLevel < 2000)
	{
		response = "Haha; I like you.";
	}
	else
	{
		familiarity += 100;
		response = "You're quite the charmer.";
	}


	return response;
}

int Villager::GetVillagerMentalState(Actor *owner)
{
	if(VillagerAi *ai = dynamic_cast<VillagerAi *>(owner->ai))
	{
		return ai->mentalState;
	}
	else return -1;
}

void Villager::SetVillagerMentalState(Actor *owner, int newMentalState)
{
	if(VillagerAi *ai = dynamic_cast<VillagerAi *>(owner->ai))
	{
		ai->mentalState = newMentalState;
	}	
}