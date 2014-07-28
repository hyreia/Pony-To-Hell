#ifndef PAUSE_SCREEN_MENU_HPP
#define PAUSE_SCREEN_MENU_HPP

namespace DBL
{
	class PauseScreenMenu: public Screen
	{
	public:
		PauseScreenMenu();
		~PauseScreenMenu();
		void HandleInput(ALLEGRO_EVENT &ev) override;
		void Output() override;
		void Process() override;

	};
};

#endif