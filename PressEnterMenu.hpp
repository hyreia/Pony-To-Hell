#ifndef PRESS_ENTER_MENU_HPP
#define PRESS_ENTER_MENU_HPP

namespace DBL
{
	class PressEnterMenu: public Screen
	{
	public:
		PressEnterMenu();
		~PressEnterMenu();
		void HandleInput(ALLEGRO_EVENT &ev) override;
		void Output() override;
		void Process() override;

	};
};

#endif