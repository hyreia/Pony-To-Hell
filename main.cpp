#include "main.hpp"

using namespace DBL;

int main()
{
	game.reset(new Game());
	if(game->Initialize())
	{
		game->Run();
	}
	game->Destroy();


	return 0;
}