#ifndef VILLAGER_HPP
#define VILLAGER_HPP

#include "ActorComponent.hpp"

namespace DBL
{
	//Signifies actor is a villager who talks and has a personality
	//The real villager class would have more to it, but since we're in a time crunch...
	class Villager: public ActorComponent
	{
	public:
		int friendliness, adventureness, talkativeness, familiarity;
		static const char *villager;

		Villager(int _friendliness = 0, int _adventureness = 0, int _talkativeness = 0, int _familiarity = 0):
		ActorComponent(villager),
		friendliness(_friendliness), 
		adventureness(_adventureness), 
		talkativeness(_talkativeness),
		familiarity(_familiarity)
		{}

		//returns -1 if owner has no Ai (and thus no mental state and is not an actual villager)
		int GetVillagerMentalState(Actor *owner);

		//Sets mental state if its a villager, can quietly fail if invalid owner
		//mental state can be set to an invalid value (villager does nothing in this case)
		void SetVillagerMentalState(Actor *owner, int newMentalState);

		virtual ~Villager(){}

		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;

		std::string Greet();
		std::string TalkTo();
		std::string Recruit();
		std::string Flirt();

	private:
		enum VILLAGER_SERIALIZATION
		{
			FRIEND=1,
			ADVENTURE,
			TALK,
			FAMILIAR
		};
	};

};

#endif