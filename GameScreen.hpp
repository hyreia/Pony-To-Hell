#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

namespace DBL
{
	class GameScreen: public Screen
	{
	public:
		GameScreen();
		virtual ~GameScreen();

		void HandleInput(ALLEGRO_EVENT &ev) override;
		void Output() override;
		void Process() override;

	private:
		bool isMouseHeld, isHoldingStill;
	};
};

#endif