#include "main.hpp"

using namespace DBL;

PauseScreenMenu::PauseScreenMenu()
{}

PauseScreenMenu::~PauseScreenMenu(){}

void PauseScreenMenu::HandleInput(ALLEGRO_EVENT &ev)
{

	switch(ev.type)
	{
		case ALLEGRO_EVENT_KEY_DOWN:
			if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
			{
				game->SetNextScreen(new GameScreen());
			}
			break;
		default:
			break;
	}
}

void PauseScreenMenu::Output()
{
	game->DrawSimulation();

	
	game->graphics.SetPrintAlignment(PRINT_LEFT);
	game->graphics.PrintText(0, Color::White, 0, SCREEN_HEIGHT()-50, " PAUSED");
}

void PauseScreenMenu::Process(){}