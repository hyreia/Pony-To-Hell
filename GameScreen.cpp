#include "main.hpp"

using namespace DBL;

GameScreen::GameScreen():
isMouseHeld(false),
isHoldingStill(false)
{
	
	/*
	ColorfulString message;
	message.Add(ColorStringPair(Color::White, "It really sucks that the show won't try to make "));
	message.Add(ColorStringPair(Color::Yellow, "Flash Sentry"));
	message.Add(ColorStringPair(Color::Grey, " (completely unrelated to EG Flash, obviously)"));
	message.Add(ColorStringPair(Color::White, " and "));
	message.Add(ColorStringPair(Color::Purple, "Twilight"));
	message.Add(ColorStringPair(Color::White, " a thing on FIM. I mean, there's some potential there for a cute romance, or at least have "));	
	message.Add(ColorStringPair(Color::Yellow, "Flash"));
	message.Add(ColorStringPair(Color::White, " have a small crush on "));
	message.Add(ColorStringPair(Color::Purple, "Twilight"));
	message.Add(ColorStringPair(Color::White, "."));

	game->world.messageQueue.AddMessage(ColorfulString(Color::Red, "This is a test message. Notice the red font. and cool lettering."));
	game->world.messageQueue.AddMessage(message);

	ColorStringPair::SetCommonColor(Color::White);
	ColorfulString msg2;
	msg2.Add(ColorStringPair("Spooky Skeleton punched you for 12 damage!"));
	game->world.messageQueue.AddMessage(msg2);
	game->world.messageQueue.AddMessage(msg2);
	game->world.messageQueue.AddMessage(msg2);
	
	ColorfulString snark;
	snark.Add(ColorStringPair("Anon, everyone knows "));
	snark.Add(ColorStringPair(Color::Red, "THAT"));
	snark.Add(ColorStringPair(" doesn't count!"));
	game->world.messageQueue.AddMessage(snark);
	//*/
	
}

GameScreen::~GameScreen(){}

void GameScreen::HandleInput(ALLEGRO_EVENT &ev)
{
	switch(ev.type)
	{
		case ALLEGRO_EVENT_KEY_DOWN:
			//if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE) game->SetNextScreen(new PauseScreenMenu());
			if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE) game->SetNextScreen(new LevelTransitionScreen());
			else if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) game->SetNextScreen(new QuitPromptScreen());
			else if(ev.keyboard.keycode == ALLEGRO_KEY_LSHIFT){	isHoldingStill = true; }
			break;
		case ALLEGRO_EVENT_KEY_UP:
			if(ev.keyboard.keycode == ALLEGRO_KEY_LSHIFT)
			{
				isHoldingStill = false;
			}
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			if(ev.mouse.button == 1 || ev.mouse.button == 2)
			{
				auto clickPosition = game->graphics.GetTileCoordinateUnderScreenPosition(ev.mouse.x, ev.mouse.y);
				bool isLeftClick = false;
				if(ev.mouse.button == 1){ isLeftClick = true; isMouseHeld = true; }
				auto playerId = game->world.actorCollection.PLAYER_ACTOR_ID;
				auto player = game->world.actorCollection.GetActor(playerId);
				if(PlayerControlledAi *ai = dynamic_cast<PlayerControlledAi *>(player->ai))
				{
					ai->RespondToMouseClick(player, clickPosition.x, clickPosition.y, isLeftClick);
				}
				
			}
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			if(ev.mouse.dz > 0) game->world.combatUserInterface.DecrementRightClickBox();
			else if(ev.mouse.dz < 0) game->world.combatUserInterface.IncrementRightClickBox();
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			if(ev.mouse.button == 1) isMouseHeld = false;
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			game->SetNextScreen(new QuitPromptScreen);
			break;
		default:
			break;
	}


}

void GameScreen::Output()
{
	game->DrawSimulation();
	game->world.messageQueue.Draw();
	game->world.combatUserInterface.Draw();
	
}

void GameScreen::Process()
{
	if(isMouseHeld && !isHoldingStill)
	{

		ALLEGRO_MOUSE_STATE mouse;
		al_get_mouse_state(&mouse);
		auto clickPosition = game->graphics.GetTileCoordinateUnderScreenPosition(mouse.x, mouse.y);
		int playerId = game->world.actorCollection.PLAYER_ACTOR_ID;
		auto player = game->world.actorCollection.GetPlayer();
		if(player)
		{
			game->movingActors.RegisterActorToMove(player->id, clickPosition.x, clickPosition.y,
				game->GetGameLevel());
			if(PlayerControlledAi *ai = dynamic_cast<PlayerControlledAi *>(player->ai))
			{
				ai->RespondToMouseClick(player, clickPosition.x, clickPosition.y);
			}
		}
	}
	if(isHoldingStill)
	{
		auto player = game->world.actorCollection.GetPlayer();
		if(player->movement->isMoving)
		{
			game->movingActors.RegisterActorToMoveTowardsActor(player->id, player->id);
		}
	}

	game->ProcessSimulation();

}