#include "main.hpp"

using namespace DBL;

std::unique_ptr<DBL::Game> DBL::game(nullptr);

const int Game::FPS = 60;

Game::Game():
isShuttingDown(false),
windowWidth(960),
windowHeight(600),
//FPS(60),
errorMessage(NULL),
hasTimerTicked(false),
isSimulationRunning(true),
event_queue(NULL),
timer(NULL),
screen(NULL),
nextScreen(NULL),
isMenuClosing(false),
gameLevel(0),
isSkippingDrawingThisFrame(false)
{}

Game::~Game(){}

bool Game::Initialize()
{
	rng::Seed();
	isShuttingDown = !InitializeAllegro();
	

	if(!isShuttingDown) graphics.LoadResources();

	//Setup the game content
	world.materials.Init();
	world.actorCollection.LoadActorsFromFile();
	world.items.Initialize();

	world.actorCollection.MakeAllActorsInactive();
	world.actorCollection.MakeActorsAtDepthActive(gameLevel);
	
	world.levelCollection.LoadLevelsFromFile();
	movingActors.RegisterMovingActorsAfterReloading();
	world.messageQueue.Initialize();
	world.combatUserInterface.Initialize();

	SetupTestCase();

	screen = new PressEnterMenu();
	//screen = new GameScreen();

	return !isShuttingDown;
}

void Game::Destroy()
{
	world.messageQueue.Save();
	
	world.actorCollection.MakeAllActorsActive();
	world.actorCollection.SaveActorsToFile();
	world.levelCollection.SaveLevelsToFile();
	world.items.Save(); world.items.Free();

	graphics.FreeResources();
	DestroyAllegro();
}

void Game::Run()
{

	al_start_timer(timer);

	while(!isShuttingDown)
	{
		//Input
		CheckForEvents();

		if(hasTimerTicked)
		{
			screen->Process();
			screen->Output();
			if(!isSkippingDrawingThisFrame)
				graphics.FlipScreen();
			else isSkippingDrawingThisFrame = false;

			if(nextScreen)
			{
				delete screen;
				screen = nextScreen;
				nextScreen = NULL;
			}
			hasTimerTicked = false;


		}
	}

}

int64_t Game::GetGameTime()
{
	return al_get_timer_count(timer);
}

void Game::SkipDrawingNextFrame()
{
	isSkippingDrawingThisFrame = true;
}

void Game::ProcessSimulation()
{
	ProcessActors();
	MoveMovingActors();
	combatSystem.ProcessQueuedCombatEvents();

	auto player = world.actorCollection.GetPlayer();
	if(player && player->position)
	{
		graphics.CenterDrawRectangleOnPosition(player->position->absolutePos.x,
			player->position->absolutePos.y);
		//graphics.SetDepthToDraw(player->position->depth);
	}
}

void Game::DrawSimulation()
{
	//auto player = world.actorCollection.GetActor(1);
	//if(player)
	//{
	graphics.SetDepthToDraw(gameLevel);
	graphics.DrawTileset(world.levelCollection.levels[gameLevel]);
	//}

	graphics.DrawActors(world.actorCollection.GetActorsInDrawOrder());
}

void Game::ProcessActors()
{
	for(auto actorIter = world.actorCollection.actors.begin(); 
		actorIter != world.actorCollection.actors.end(); actorIter++)
	{
		auto *actor = (*actorIter).second;
		bool isActingThisTurn = false;


		if(actorIter->second->actDelay <= 0)
		{
			isActingThisTurn = true;
		}
		else
		{
			if(actorIter->second->actDelay < 100)
			{
				//The closer to 0 the more likely, the closer to 100 the less likely
				bool isActingThisTurn = !rng::IsProbable(actorIter->second->actDelay);
			}
			//regardless, decrease
			actorIter->second->actDelay -= 100;
		}

		if(isActingThisTurn)
		{
			if(actorIter->second->ai)
			{
				actorIter->second->ai->Act(actorIter->second);

			}
			actorIter->second->ResetActDelay();
		}
	}
}

void Game::MoveMovingActors()
{
	movingActors.MoveActors();	
}

bool Game::InitializeAllegro()
{
	if(!al_init()){ errorMessage = "al_init()"; }
	if(!al_install_keyboard()){ errorMessage = "al_install_keyboard"; }
	if(!al_install_mouse()){ errorMessage = "al_install_mouse"; }
	if(!al_init_image_addon()){ errorMessage = "al_init_image_addon()"; }
	al_init_font_addon(); //returns void
	if(!al_init_ttf_addon()){ errorMessage = "al_init_ttf_addon()"; }
	if(!al_init_primitives_addon()){ errorMessage = "al_init_primitives_addon"; }
	if(!al_install_audio()){ errorMessage = "al_install_audio"; }
	else
	{
		if(!al_init_acodec_addon())
			errorMessage = "Failed to start audio acodec addon.";
		if(!al_reserve_samples(32))
			errorMessage = "Failed to reserve 32 sounds.";
	}


	if(errorMessage) return false;

	//Create allegro components
	event_queue = al_create_event_queue();

	if(!graphics.CreateDisplay(event_queue, 1280, 800))
	{
		errorMessage = "Couldn't create display";
	}
	
	//timer = al_create_timer(1.0/(double)(FPS)); //hardcoded 60 fps
	timer = al_create_timer(1.0/60.0); //hardcoded 60 fps

	if(!event_queue){ errorMessage = "al_create_event_queue"; }
	
	
	//if(!display){ errorMessage = "al_create_display"; }
	
	if(!timer){ errorMessage = "al_create_timer"; }

	if(errorMessage) return false;

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	return true;
}

void Game::SetGameLevel(int level)
{
	gameLevel = level;
}

int Game::GetGameLevel(){ return gameLevel; }


void Game::DestroyAllegro()
{
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
}

void Game::SetupTestCase()
{
	graphics.SetWindowTitle("Pony Thirsts for the Blood of her Enemies");


	//Setup actors if we haven't yet
	if(world.actorCollection.actors.empty())
	{
		Actor *playerActor = new Actor(1);
		Actor *otherActor = new Actor(2);
	
		IndexedImage eyes; eyes.index = 4; eyes.tint = Color::White;
		IndexedImage irises; irises.index = 5;
		IndexedImage clothes; clothes.index = ADVENTURER_CLOTHES;
		clothes.tint = Color::White;

		int playerActorId = world.actorCollection.InsertActor(playerActor);
		//world.actorCollection.playerActorId = PLAYER_ACTOR_ID;
		double TILES_PER_SEC = 5.0;
		double moveSpeed = TILES_PER_SEC/FPS*TILE_SIZE;
		playerActor->movement = new Movement(moveSpeed);
		playerActor->position = new Position(3*TILE_SIZE, 5*TILE_SIZE, TILE_SIZE, STANDING, 0);
		playerActor->images = new ActorImages();
		IndexedImage marker; marker.index = UNICORN_TILE; marker.tint = Color::Grey;
		playerActor->images->images.push_back(marker);
		playerActor->images->images.push_back(clothes);
		playerActor->images->images.push_back(eyes);
		irises.tint = Color::Blue;
		playerActor->images->images.push_back(irises);
		playerActor->body = new Body(EARTH_PONY, MALE);
		playerActor->ai = new PlayerControlledAi();
		playerActor->combatant = new Combatant(5, 5, 5, 5, 5, false);
		//marker.index = 2; marker.tint = Color::White;
		//playerActor->images->images.push_back(marker);
		playerActor->ResetActDelay();

		//Setup second actor
		world.actorCollection.InsertActor(otherActor);
		world.actorCollection.actors.insert(std::pair<int, Actor*>(2,
			otherActor));
		otherActor->movement = new Movement(4.0/FPS*TILE_SIZE);
		otherActor->position = new Position(10*TILE_SIZE, 15*TILE_SIZE, TILE_SIZE, STANDING, 0);
		otherActor->villager = new Villager(200, 0, 0, 0);
		otherActor->images = new ActorImages();
		otherActor->body = new Body(EARTH_PONY, FEMALE);
		otherActor->ai = new VillagerAi();
		//otherActor->ai = new WanderAimlesslyAi();
		otherActor->combatant = new Combatant(5, 5, 5, 5, 5, false);
		IndexedImage griffon; griffon.index = PONY_TILE; griffon.tint = Color::Blue;
		otherActor->images->images.push_back(griffon);
		otherActor->images->images.push_back(eyes);
		irises.tint = Color::Yellow;
		otherActor->images->images.push_back(irises);
		otherActor->ResetActDelay();

		Actor *thirdActor = new Actor(3);
		world.actorCollection.InsertActor(thirdActor);
		thirdActor->movement = new Movement(2.0/TILES_PER_SEC/FPS*TILE_SIZE);
		thirdActor->position = new Position(10*TILE_SIZE, 5*TILE_SIZE, TILE_SIZE, STANDING, 0);
		thirdActor->villager = new Villager(0, 200, 100, 0);
		thirdActor->images = new ActorImages();
		thirdActor->ai = new VillagerAi();
		//thirdActor->ai = new WanderAimlesslyAi();
		thirdActor->combatant = new Combatant(5, 5, 5, 5, 5, false);
		marker.index = PEGASUS_TILE; marker.tint = Color::Green;
		thirdActor->images->images.push_back(marker);
		thirdActor->images->images.push_back(eyes);
		irises.tint = Color::Green;
		thirdActor->images->images.push_back(irises);
		thirdActor->ResetActDelay();
		thirdActor->body = new Body(EARTH_PONY, FEMALE);

		Actor *fourthActor = new Actor(4);
		world.actorCollection.InsertActor(fourthActor);
		fourthActor->movement = new Movement(6.0/TILES_PER_SEC/FPS*TILE_SIZE);
		fourthActor->position = new Position(17*TILE_SIZE, 15*TILE_SIZE, TILE_SIZE, STANDING, 0);
		fourthActor->villager = new Villager(100, 100, 500, 0);
		fourthActor->body = new Body(EARTH_PONY, MALE);
		fourthActor->ai = new VillagerAi();
		fourthActor->combatant = new Combatant(5, 5, 5, 5, 5, false);
		fourthActor->images = new ActorImages();
		marker.index = PONY_TILE; marker.tint = Color::Yellow;
		fourthActor->images->images.push_back(marker);
		fourthActor->images->images.push_back(eyes);
		irises.tint = Color::Orange;
		fourthActor->images->images.push_back(irises);
		fourthActor->ResetActDelay();

		Actor *skeleton = new Actor(5);
		world.actorCollection.InsertActor(skeleton);
		skeleton->movement = new Movement(3.0/TILES_PER_SEC/FPS*TILE_SIZE);
		skeleton->position = new Position(15*TILE_SIZE, 10*TILE_SIZE, TILE_SIZE, STANDING, 1);
		skeleton->body = new Body(EARTH_PONY, FEMALE);
		skeleton->ai = new WanderAimlesslyAi();
		skeleton->combatant = new Combatant(5, 2, 2, 1, 1);
		marker.index = HORSE_SKELETON_TILE; marker.tint = Color::Yellow;
		skeleton->images = new ActorImages();
		skeleton->images->images.push_back(marker);
		skeleton->ResetActDelay();

	}


	//Setup a level

	if(!world.levelCollection.IsLevelAtIndex(0))
	{
		world.levelCollection.levels.insert(std::pair<int, Tileset>(0, Tileset()));
		int levelWidth=50, levelHeight=50;
		std::vector<LevelTile> tileColumn(levelHeight, LevelTile(1));
		std::vector< std::vector<LevelTile> > tileRow(levelWidth, tileColumn);
		//tileRow[1][0].tileMaterialIndex = STONE_BLOCK;
		tileRow[1][1].tileMaterialIndex = STONE_BLOCK;
		tileRow[0][0].tileMaterialIndex = STONE_BLOCK;
		tileRow[11][0].tileMaterialIndex = STONE_BLOCK;
		//tileRow[2][0].tileMaterialIndex = STONE_BLOCK;
		//tileRow[10][3].tileMaterialIndex = STONE_BLOCK;
		//tileRow[4][5].tileMaterialIndex = STONE_BLOCK;

		tileRow[5][10].tileMaterialIndex = DOWN_STAIRS;
		world.levelCollection.levels[0].PosOfDownStairs = new XYPair<int>(5, 10);

		tileRow[19][0].tileMaterialIndex = STONE_BLOCK;
		//tileRow[19][1].tileMaterialIndex = STONE_BLOCK;
		//tileRow[18][0].tileMaterialIndex = STONE_BLOCK;

		//Add some variation
		for(int x = 0; x < levelWidth; x++) for(int y = 0; y < levelHeight; y++)
		{
			int tintVariationsAvailable = world.materials.Get(tileRow[x][y].tileMaterialIndex).numOfTintVariations;
			int rand = rng::Rand(tintVariationsAvailable);
			tileRow[x][y].tintVariation = rand;
		}
		world.levelCollection.levels[0].lastTimeVisited = GetGameTime();
		world.levelCollection.levels[0].tiles = tileRow;

	}
	if(!world.levelCollection.IsLevelAtIndex(1))
	{
		world.levelCollection.levels.insert(std::pair<int, Tileset>(1, Tileset()));
		int levelWidth=50, levelHeight=50;
		std::vector<LevelTile> tileColumn(levelHeight, LevelTile(DIRT));
		std::vector< std::vector<LevelTile> > tileRow(levelWidth, tileColumn);
		world.levelCollection.levels[1].tiles = tileRow;

		tileRow[5][10].tileMaterialIndex = UP_STAIRS;
		world.levelCollection.levels[1].PosOfUpStairs = new XYPair<int>(5, 10);

		//Add some variation
		for(int x = 0; x < levelWidth; x++) for(int y = 0; y < levelHeight; y++)
		{
			int tintVariationsAvailable = world.materials.Get(tileRow[x][y].tileMaterialIndex).numOfTintVariations;
			int rand = rng::Rand(tintVariationsAvailable);
			tileRow[x][y].tintVariation = rand;
		}
		world.levelCollection.levels[1].tiles = tileRow;
	}

	//*/



	graphics.SetDrawRectangleSize(SCREEN_WIDTH(), SCREEN_HEIGHT());


	//screen = new PressEnterMenu();

	//Register for actor to move
	//movingActors.RegisterActorToMove(playerActorId, 8, 4);
}

void Game::DisplayLastError()
{
	graphics.ShowErrorMessage(errorMessage);
}

void Game::CheckForEvents()
{
	//timer
	ALLEGRO_EVENT ev;

	Screen *newMenu = NULL;

	al_wait_for_event(event_queue, &ev);
	do
	{
		switch(ev.type)
		{
		case ALLEGRO_EVENT_TIMER:
			hasTimerTicked = true;
			break;
			//TODO: redirect display events to graphics
		default:
			screen->HandleInput(ev);
			break;
		}
		

	} while(al_get_next_event(event_queue, &ev));

	if(newMenu) screen = newMenu;

	//keys
	//mouse
}

void Game::Quit()
{
	isShuttingDown = true;
}

void Game::BackToGameScreen()
{
	SetNextScreen(new GameScreen());
}

void Game::SetNextScreen(Screen *screen)
{
	//if(!nextScreen)
	{
		nextScreen = screen;
	}
}