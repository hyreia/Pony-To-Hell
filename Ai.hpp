#ifndef AI_HPP
#define AI_HPP

namespace DBL
{
	class Ai: public ActorComponent
	{
	public:
		virtual void Act(Actor *owner){}
		virtual ~Ai(){}
		static Ai *NewAiFromDeserialization(std::vector<std::string> tokens);

		static const char *aiType;
		const char *componentSecondaryType;
	protected:
		Ai(const char *componentSecondaryType);

	};

	class PlayerControlledAi: public Ai
	{
	public:
		static const char *type;
		PlayerControlledAi(): Ai(type), hasActedRecently(false){}
		~PlayerControlledAi(){}
		
		void Act(Actor *owner) override;
		void RespondToMouseClick(Actor *owner, int tileClickedX, int tileClickedY, bool isLeftClick = true);

		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;
	private:
		bool hasActedRecently;

	};

	class WanderAimlesslyAi: public Ai
	{
	public:
		static const char *type;
		WanderAimlesslyAi(): Ai(type){}
		~WanderAimlesslyAi(){}
		void Act(Actor *owner) override;
		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;
	};

	class VillagerAi: public Ai
	{
	public:
		static const char *type;
		VillagerAi(): mentalState(0), Ai(type){}
		~VillagerAi(){}
		void Act(Actor *owner) override;
		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;

		enum MENTAL_STATE
		{
			VILLAGER_TIME_FOR_FUN,
			VILLAGER_LISTENS_QUIETLY,	//don't walk away while I'm talking to you!
			VILLAGER_TIME_FOR_WORK,
			VILLAGER_BEDTIME,
			VILLAGER_TIME_FOR_COMBAT
		};
		int mentalState;

		enum VILLAGER_AI_SERIALIZE
		{
			//0 = "Ai"
			//1 = "VillagerAi"
			MENTAL_STATE = 2
		};

		void DoActivitiesAroundTheVillage();
		void GoToBedAndSleepUntilMorning();
		void PerformYourJob();
		void ConcernYourselfWithCombat();

		//TODO: give them some kind of todo list
		//They will pick from a list of things they do around the village and go simulate
		//doing those ("go to the flower shop", "find a book and read it", "hang out with a friend")
		//A "todo list" will be a list of steps in order to simulate doing that activity well, "pick a random friend",
			//"follow friend", "find a nearby book"
		//Each individual villager may have its own list or preferences based on its personality and "job"
	};



};

#endif