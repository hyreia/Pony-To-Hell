#include "main.hpp"

using namespace DBL;

QuitPromptScreen::QuitPromptScreen(): highlightedOption(0)
{
	int boxWidth = 300;
	yes.Initialize(SCREEN_WIDTH()/2 - boxWidth/4, SCREEN_HEIGHT()/2,
		50, true, "Yes");
	no.Initialize(SCREEN_WIDTH()/2 + boxWidth/4, SCREEN_HEIGHT()/2,
		50, true, "No");
}

QuitPromptScreen::~QuitPromptScreen()
{}

void QuitPromptScreen::HandleInput(ALLEGRO_EVENT &ev)
{

	switch(ev.type)
	{
		case ALLEGRO_EVENT_KEY_DOWN:
			if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				game->SetNextScreen(new GameScreen());
			}
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			{
				if(yes.IsPositionWithinButton(ev.mouse.x, ev.mouse.y))
				{
					highlightedOption = 1;
				}
				else if(no.IsPositionWithinButton(ev.mouse.x, ev.mouse.y))
				{
					highlightedOption = 2;
				}
				else highlightedOption = 0;
			}
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			{
				if(yes.IsPositionWithinButton(ev.mouse.x, ev.mouse.y))
				{
					game->Quit();
				}
				else if(no.IsPositionWithinButton(ev.mouse.x, ev.mouse.y))
				{
					game->SetNextScreen(new GameScreen());
				}
			}
			break;

		default:
			break;
	}
}

void QuitPromptScreen::Output()
{
	game->DrawSimulation();

	int boxWidth=300, boxHeight=100;
	game->graphics.DrawColoredBox(SCREEN_WIDTH()/2 - boxWidth/2,
		SCREEN_HEIGHT()/2 - boxHeight/2, 
		SCREEN_WIDTH()/2 + boxWidth/2,
		SCREEN_HEIGHT()/2 + boxHeight/2, Color::BrightBlue);
	game->graphics.DrawBorder(SCREEN_WIDTH()/2 - boxWidth/2,
		SCREEN_HEIGHT()/2 - boxHeight/2, 
		SCREEN_WIDTH()/2 + boxWidth/2,
		SCREEN_HEIGHT()/2 + boxHeight/2, Color::Blue);
	game->graphics.SetPrintAlignment(PRINT_CENTER);
	game->graphics.PrintText(0, Color::White, SCREEN_WIDTH()/2, SCREEN_HEIGHT()/2-50,
		"Quit?");
	
	if(highlightedOption == 1) yes.DrawHighlighted();
	else yes.Draw();

	if(highlightedOption == 2) no.DrawHighlighted();
	else no.Draw();
}

void QuitPromptScreen::Process(){}