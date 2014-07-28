#ifndef COMBAT_USER_INTERFACE_HPP
#define COMBAT_USER_INTERFACE_HPP

/* Set items or magic to click shortcuts. Left click using the given item 
(weapon suggested), and right click using the item selected on the shortcut bar, changing
right click selection by using the mouse scroll wheel to wheel through.

These are get() in GameScreen typically and set() usually from a menu 

What was in this combat user interface is saved and loaded to keep its state between play sessions
*/

namespace DBL
{
	struct UIBox
	{
		UIBox(): abilityId(0), itemId(0){}

		void SetAbility(int ability)
		{ abilityId = ability; itemId = 0;}

		void SetItem(int item)
		{ itemId = item; abilityId = 0; }

		int abilityId;
		int itemId;
	};

	class CombatUserInterface
	{
	public:
		CombatUserInterface();
		~CombatUserInterface();

		void Initialize();
		void LoadStateFromFile();
		void SaveStateToFile();

		bool IsPositionWithintLeftClickBox(int x, int y);

		/* Returns box position is under, -1 if no box underneath */
		int IsPositionWithinARightClickBox(int x, int y);
		void IncrementRightClickBox(); //mouse wheel down
		void DecrementRightClickBox(); //mouse wheel up
		void SetRightClickBoxSelected(int box); //right click bar selected
		
		UIBox GetLeftClick();
		UIBox GetRightClick();
		int GetRightClickBoxSelected();
		void SetLeftClick(int ability = 0, int item = 0);
		void SetRightClick(int box, int ability = 0, int item = 0);

		XYPair<int> GetStartingPositionOfLeftClickBox();
		XYPair<int> GetStartingPositionOfRightClickBoxBar();

		void Draw();
	private:
		static const char *combat_interface_file;

		static const int SIZE_OF_BOXES = 32;
		static const int NUM_OF_RIGHT_CLICK_BOXES = 9;
		XYPair<int> positionOfLeftClickBox;
		XYPair<int> positionOfRightClickBoxBar;

		
		int rightClickBoxSelected;
		UIBox leftBox;
		UIBox rightBoxes[NUM_OF_RIGHT_CLICK_BOXES];
	};
};

#endif