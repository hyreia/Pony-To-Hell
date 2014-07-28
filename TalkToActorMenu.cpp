#include "main.hpp"

using namespace DBL;

TalkToActorMenu::TalkToActorMenu(int actorId):
conversatingActorId(actorId),
conversatingActor(NULL),
highlightedButton(0),
talkMenuState(TALK_MAIN_STATE)
{
	conversatingActor = game->world.actorCollection.GetActor(conversatingActorId);

	if(conversatingActor->villager->GetVillagerMentalState(conversatingActor) == VillagerAi::VILLAGER_TIME_FOR_FUN)
	{
		//Change mental state
		conversatingActor->villager->SetVillagerMentalState(conversatingActor, VillagerAi::VILLAGER_LISTENS_QUIETLY);
		//Setup window for converstion, including its buttons
		//TODO: consider creating this as a completely different screen and using this or something else
		//instead as a stepping stone to that screen so all the logic of the different talk menus
		//are separated
		int lineHeight = game->graphics.GetTextLineHeight(0);
		greet.Initialize(SCREEN_WIDTH()/2	, SCREEN_HEIGHT()/2	 -  50	+ 0 * lineHeight, 50, true, "Greet");
		talk.Initialize(SCREEN_WIDTH()/2	, SCREEN_HEIGHT()/2	 -  50	+ 1 * lineHeight, 50, true, "Talk");
		recruit.Initialize(SCREEN_WIDTH()/2 , SCREEN_HEIGHT()/2  -  50	+ 2 * lineHeight, 50, true, "Recruit");
		flirt.Initialize(SCREEN_WIDTH()/2	, SCREEN_HEIGHT()/2	 -  50	+ 3 * lineHeight, 50, true, "Flirt");
		nothing.Initialize(SCREEN_WIDTH()/2 , SCREEN_HEIGHT()/2  -  50	+ 4 * lineHeight, 50, true, "Nothing");
	}
	else if(conversatingActor->villager->GetVillagerMentalState(conversatingActor) == VillagerAi::VILLAGER_TIME_FOR_WORK)
	{
		//Create and setup merchant menu
	}
	else if(conversatingActor->villager->GetVillagerMentalState(conversatingActor) == VillagerAi::VILLAGER_BEDTIME ||
		conversatingActor->villager->GetVillagerMentalState(conversatingActor) == VillagerAi::VILLAGER_LISTENS_QUIETLY)
	{
		//don't respond, you're either asleep or listening to him already
	}
	else if(conversatingActor->villager->GetVillagerMentalState(conversatingActor) == VillagerAi::VILLAGER_TIME_FOR_COMBAT)
	{
		//Bring up menu used to talk to actor when they are a recruit
	}


}

TalkToActorMenu::~TalkToActorMenu()
{}

void TalkToActorMenu::HandleInput(ALLEGRO_EVENT &ev)
{
	switch(ev.type)
	{
		case ALLEGRO_EVENT_KEY_DOWN:
			if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE ||
				ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
			{
				if(talkMenuState == TALK_MAIN_STATE)
				{
						conversatingActor->villager->SetVillagerMentalState(conversatingActor, VillagerAi::VILLAGER_TIME_FOR_FUN);
							game->SetNextScreen(new GameScreen());
				}
				else
				{
					talkMenuState = TALK_MAIN_STATE;
					message.clear();
				}
			}
			break;
			case ALLEGRO_EVENT_MOUSE_AXES:
			{
				if(greet.IsPositionWithinButton(ev.mouse.x, ev.mouse.y))	  highlightedButton = 1;
				else if(talk.IsPositionWithinButton(ev.mouse.x, ev.mouse.y)) highlightedButton = 2;
				else if(recruit.IsPositionWithinButton(ev.mouse.x, ev.mouse.y)) highlightedButton = 3;
				else if(flirt.IsPositionWithinButton(ev.mouse.x, ev.mouse.y)) highlightedButton = 4;
				else if(nothing.IsPositionWithinButton(ev.mouse.x, ev.mouse.y)) highlightedButton = 5;
			}
			break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			{
				if(talkMenuState == TALK_MAIN_STATE) 
				{
					if(greet.IsPositionWithinButton(ev.mouse.x, ev.mouse.y))		talkMenuState = TALK_GREET_STATE;
					else if(talk.IsPositionWithinButton(ev.mouse.x, ev.mouse.y))	talkMenuState = TALK_CONVERSATION_STATE;
					else if(recruit.IsPositionWithinButton(ev.mouse.x, ev.mouse.y)) talkMenuState = TALK_RECRUIT_STATE;
					else if(flirt.IsPositionWithinButton(ev.mouse.x, ev.mouse.y))	talkMenuState = TALK_FLIRT_STATE;
					else if(nothing.IsPositionWithinButton(ev.mouse.x, ev.mouse.y))
					{
						conversatingActor->villager->SetVillagerMentalState(conversatingActor, VillagerAi::VILLAGER_TIME_FOR_FUN);
							game->SetNextScreen(new GameScreen());
					}
				}
				else
				{
					talkMenuState = TALK_MAIN_STATE;
					message.clear();
				}

			}
			break;
		default:
			break;
	}
}

void TalkToActorMenu::Output()
{
	game->DrawSimulation();
	game->world.messageQueue.Draw();

	if(conversatingActor->name && conversatingActor->villager->familiarity >= 100)
	{
		game->graphics.SetPrintAlignment(PRINT_RIGHT);
		//TODO: print name in top right corner
	}

	game->graphics.DrawColoredBox(SCREEN_WIDTH()/2 - 200,
		SCREEN_HEIGHT()/2 - 50, 
		SCREEN_WIDTH()/2 + 200,
		SCREEN_HEIGHT()/2 + 100, 
		Color(0, 0, 255, 128));

	if(talkMenuState == TALK_MAIN_STATE)
	{
		if(highlightedButton == 1) greet.DrawHighlighted(); else greet.Draw();
		if(highlightedButton == 2) talk.DrawHighlighted(); else talk.Draw();
		if(highlightedButton == 3) recruit.DrawHighlighted(); else recruit.Draw();
		if(highlightedButton == 4) flirt.DrawHighlighted(); else flirt.Draw();
		if(highlightedButton == 5) nothing.DrawHighlighted(); else nothing.Draw();
	}
	else
	{

		//game->graphics.PrintTextRect(0, Color::White, SCREEN_WIDTH()/2 - 175, SCREEN_HEIGHT()/2,
		//	350, 0, 0, true, message);
	}



	/*
	std::string greet;
	auto villager = conversatingActor->villager;

	std::string greeting = PickGreeting();
	game->graphics.PrintTextRect(0, 
		Color::White, SCREEN_WIDTH()/2 - 150,
		SCREEN_HEIGHT()/2,
		200, 0, 0, true, greeting);*/


	//game->graphics.

}

void TalkToActorMenu::Process()
{
	game->ProcessSimulation();

	if(talkMenuState != TALK_MAIN_STATE)
	{
		auto social = conversatingActor->villager;
		if(talkMenuState == TALK_GREET_STATE) message = social->Greet();
		else if(talkMenuState == TALK_CONVERSATION_STATE) message = social->TalkTo();
		else if(talkMenuState == TALK_RECRUIT_STATE) message = social->Recruit();
		else if(talkMenuState == TALK_FLIRT_STATE) message = social->Flirt();

		message.insert(0, "The Pony says \"");
		message.append("\"");

		game->world.messageQueue.AddMessage(ColorfulString(Color::White, message));
		message.clear();
		talkMenuState = TALK_MAIN_STATE;
	}

}