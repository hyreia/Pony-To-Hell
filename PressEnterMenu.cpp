#include "main.hpp"

using namespace DBL;

PressEnterMenu::PressEnterMenu()
{}

PressEnterMenu::~PressEnterMenu(){}

void PressEnterMenu::HandleInput(ALLEGRO_EVENT &ev)
{
	switch(ev.type)
	{
		case ALLEGRO_EVENT_KEY_DOWN:
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			//if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
			{
				game->BackToGameScreen();
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			{
				game->SetNextScreen(new QuitPromptScreen);
			}
			break;
	}
}

void PressEnterMenu::Output()
{
	game->graphics.ClearToColor(Color::Black);

	
	game->graphics.DrawColoredBox(SCREEN_WIDTH()/2-300, 300, SCREEN_WIDTH()/2+300, 400,
		Color::BrightBlue);
/*	game->graphics.DrawColoredBox(SCREEN_WIDTH()/2, SCREEN_HEIGHT()/2,
		SCREEN_WIDTH()/2 + 10, SCREEN_HEIGHT()/2 + 10,
		Color::White);*/
	game->graphics.SetPrintAlignment(PRINT_CENTER);
	
	game->graphics.PrintText(FANCY48, Color::Red, SCREEN_WIDTH()/2, 100, "Pone To Hell!!!1111");
	game->graphics.PrintTextRect(COMMON24, Color::White,
		SCREEN_WIDTH()/2 - 300, 300, 600, 0, 0, true, "Press Enter, Game is mouse oriented. Talk to the others, take the stairs. Close the game, come back. Space to pause, Escape can quit");
}

void PressEnterMenu::Process(){}