#ifndef MENU_HPP
#define MENU_HPP

namespace DBL
{
	class Screen
	{
	public:
		Screen(){}
		virtual ~Screen(){}

		virtual void HandleInput(ALLEGRO_EVENT &ev) = 0;
		virtual void Output() = 0;
		virtual void Process() = 0;

	};
};

#endif