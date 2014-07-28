#ifndef GAME_HPP
#define GAME_HPP

struct ALLEGRO_EVENT_QUEUE;
struct ALLEGRO_DISPLAY;

//Core class of higher level game mechanics
namespace DBL
{
	class Game
	{
	public:
		Game();
		~Game();

		/* Returns true if everything started okay */
		bool Initialize();
		void Destroy();
		void Quit();

		World world;
		ActorMovementManager movingActors;
		CombatSystem combatSystem;

		//Draws graphics
		Graphics graphics;

		void Run();
		void SkipDrawingNextFrame();
		void BackToGameScreen();

		void SetNextScreen(Screen *screen);

		void ProcessSimulation();
		void DrawSimulation();

		void SetGameLevel(int level);
		int GetGameLevel();
		int64_t GetGameTime();


		static const int FPS;
	private:
		//Core game members
		bool isShuttingDown, hasTimerTicked, isSimulationRunning, isMenuClosing, isSkippingDrawingThisFrame;
		int windowWidth, windowHeight;
		const char *errorMessage;

		//std::list<Actor> actorList;


		Screen *screen;
		Screen *nextScreen;
		int gameLevel;

		//Allegro members
		ALLEGRO_EVENT_QUEUE *event_queue;
		ALLEGRO_TIMER *timer;

		/* Returns false if failed to initialize something */
		bool InitializeAllegro();
		void DestroyAllegro();

		/* Sets game into a certain state to be tested, for development purposes */
		
		void SetupTestCase();
		void DisplayLastError();

		
		void CheckForEvents();
		void ProcessActors();
		void MoveMovingActors();

	};

	extern std::unique_ptr<Game> game;

};

#endif