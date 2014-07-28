#include "main.hpp"

using namespace DBL;

		MenuButton::MenuButton(){}

		  void MenuButton::Initialize(int centerX, int topY, int width, bool willAdjustToFitText, const std::string &_text)
		  {
			text = _text;
			text.insert(0, " ");
			text.append(" ");

			if(willAdjustToFitText && width)
			{
				int textWidth = game->graphics.GetTextWidth(0, text);
				if(textWidth> width) width = textWidth;
			}



			corner.x = centerX - width/2;
			oppositeCorner.x = centerX + width/2;
			corner.y = topY;
			oppositeCorner.y = topY + game->graphics.GetTextLineHeight(0);

		}

		MenuButton::~MenuButton()
		{}

		void MenuButton::Draw()
		{
			Draw(false);
		}

		void MenuButton::DrawHighlighted()
		{
			Draw(true);
		}

		bool MenuButton::IsPositionWithinButton(int x, int y)
		{
			if(x > corner.x && y > corner.y &&
				x < oppositeCorner.x && y < oppositeCorner.y)
			{
				return true;
			}
			else return false;
		}

		void MenuButton::Draw(bool isHighlighted)
		{
			game->graphics.SetPrintAlignment(PRINT_LEFT);
			
			int textHalfWidth = game->graphics.GetTextWidth(0, text)/2;

			//game->graphics.DrawColoredBox(corner.x, corner.y,
			//		oppositeCorner.x, oppositeCorner.y, Color::Blue);

			if(isHighlighted)
			{
				game->graphics.PrintText(0, Color::Yellow, corner.x, corner.y, text);
			}
			else
			{
				game->graphics.PrintText(0, Color::Grey, corner.x, corner.y, text);
			}


		}