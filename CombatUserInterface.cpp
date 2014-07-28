#include "main.hpp"

using namespace DBL;

CombatUserInterface::CombatUserInterface():
rightClickBoxSelected(0)
{

}
CombatUserInterface::~CombatUserInterface(){}

void CombatUserInterface::Initialize()
{
	rightClickBoxSelected = 0;
	positionOfLeftClickBox.x = SIZE_OF_BOXES;
	positionOfLeftClickBox.y = SCREEN_HEIGHT() - 150;

	positionOfRightClickBoxBar.x = SIZE_OF_BOXES*4; //one space away from left click box
	positionOfRightClickBoxBar.y = SCREEN_HEIGHT() - 150;
}

//void CombatUserInterface::LoadStateFromFile(){}
//void CombatUserInterface::SaveStateToFile(){}

bool CombatUserInterface::IsPositionWithintLeftClickBox(int x, int y)
{
	if(x > positionOfLeftClickBox.x && x < positionOfLeftClickBox.x+SIZE_OF_BOXES &&
		y > positionOfLeftClickBox.y && x < positionOfLeftClickBox.y+SIZE_OF_BOXES)
	{
		return true;
	}
	else return false;
}

int CombatUserInterface::IsPositionWithinARightClickBox(int x, int y)
{
	int ret = -1;
	if(x > positionOfLeftClickBox.x && x < positionOfLeftClickBox.x+SIZE_OF_BOXES*NUM_OF_RIGHT_CLICK_BOXES &&
		y > positionOfLeftClickBox.y && x < positionOfLeftClickBox.y+SIZE_OF_BOXES)
	{
		x -= positionOfLeftClickBox.x;
		x /= SIZE_OF_BOXES;
		ret = x;
	}

	return ret;
}

void CombatUserInterface::IncrementRightClickBox()
{
	rightClickBoxSelected++;
	if(rightClickBoxSelected >= NUM_OF_RIGHT_CLICK_BOXES) rightClickBoxSelected = 0;

}

void CombatUserInterface::DecrementRightClickBox()
{
	rightClickBoxSelected--;
	if(rightClickBoxSelected < 0) rightClickBoxSelected += NUM_OF_RIGHT_CLICK_BOXES;
}

void CombatUserInterface::SetRightClickBoxSelected(int box)
{
	rightClickBoxSelected = box;
}

UIBox CombatUserInterface::GetLeftClick()
{
	return leftBox;
}

UIBox CombatUserInterface::GetRightClick()
{
	return rightBoxes[rightClickBoxSelected];
}

int CombatUserInterface::GetRightClickBoxSelected()
{
	return rightClickBoxSelected;
}

void CombatUserInterface::SetLeftClick(int ability, int item)
{
	leftBox.abilityId = ability;
	leftBox.itemId = item;
}

void CombatUserInterface::SetRightClick(int box, int ability, int item)
{
	if(box > 0 && box < NUM_OF_RIGHT_CLICK_BOXES)
	{
		rightBoxes[box].abilityId = ability;
		rightBoxes[box].itemId = item;
	}
}

XYPair<int> CombatUserInterface::GetStartingPositionOfLeftClickBox()
{
	return positionOfLeftClickBox;
}

XYPair<int> CombatUserInterface::GetStartingPositionOfRightClickBoxBar()
{
	return positionOfRightClickBoxBar;
}

void CombatUserInterface::Draw()
{
	//const double LINE_THICKNESS = 5.0;
	const double SEPERATION = 10.0;
	const double DIST = SIZE_OF_BOXES+SEPERATION;
	//Draw UIBox posession

	game->graphics.DrawBorder(positionOfLeftClickBox.x, positionOfLeftClickBox.y, 
		SIZE_OF_BOXES+positionOfLeftClickBox.x, SIZE_OF_BOXES+positionOfLeftClickBox.y, Color::White);

	//Draw UIBox posessions
	for(int box = 0; box < NUM_OF_RIGHT_CLICK_BOXES; box++)
	{
		game->graphics.DrawBorder(
			//x1 y1
			(box*DIST)+positionOfRightClickBoxBar.x, positionOfRightClickBoxBar.y, 
			(box*DIST)+SIZE_OF_BOXES+positionOfRightClickBoxBar.x, SIZE_OF_BOXES+positionOfRightClickBoxBar.y, 
			rightClickBoxSelected != box? Color::Grey: Color::White);
	}
}