#ifndef ACTOR_COLLECTION_HPP
#define ACTOR_COLLECTION_HPP

namespace DBL
{
	class ActorCollection
	{
	public:
		ActorCollection();
		~ActorCollection();

		bool LoadActorsFromFile();
		void SaveActorsToFile();

		int GetNextAvailableId();
		int InsertActor(Actor *actor);
		Actor *GetActor(int id);
		Actor *GetPlayer();
		void EraseActor(int id);

		bool MoveActorToActiveList(int id);
		void MoveActorToInactiveList(int id);

		std::list<Actor*> GetActorsInDrawOrder();

		void MakeActorsAtDepthActive(int depth);
		void MakeAllActorsActive();
		void MakeAllActorsInactive();

		void FreeAllActors();


		std::map<int, Actor*> actors;
		std::map<int, Actor*> inactiveActors;

		const int PLAYER_ACTOR_ID;
	private:
		int nextAvailableId;

		void InsertActor(int id, Actor *actor);
	};
};

#endif