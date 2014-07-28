#ifndef QUIT_PROMPT_SCREEN_HPP
#define QUIT_PROMPT_SCREEN_HPP

namespace DBL
{
	class QuitPromptScreen: public Screen
	{
	public:
		QuitPromptScreen();
		~QuitPromptScreen();
		void HandleInput(ALLEGRO_EVENT &ev) override;
		void Output() override;
		void Process() override;
	private:
		int highlightedOption;
		MenuButton yes, no;
	};
};

#endif