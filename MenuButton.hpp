#ifndef MENU_BUTTON_HPP
#define MENU_BUTTON_HPP

namespace DBL
{
	class MenuButton
	{
	public:
		MenuButton();
		~MenuButton();

		void Initialize(int centerX, int topY, int width, bool willAdjustToFitText, const std::string &text);

		void Draw();
		void DrawHighlighted();

		bool IsPositionWithinButton(int x, int y);

	private:
		std::string text;
		void Draw(bool isHighlighted);
		XYPair<> corner, oppositeCorner;
	};
};

#endif