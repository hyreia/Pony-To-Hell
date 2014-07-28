#include "main.hpp"

using namespace DBL;

Items::Items(){}
Items::~Items(){}

void Items::Initialize()
{
	LoadTestCase();
}

void Items::Save()
{}

void Items::Free()
{
	//Free them
	for(auto type = itemTypes.begin(); type != itemTypes.end(); type++)
	{
		delete (*type).second;
	}
	itemTypes.clear();

	for(auto item = items.begin(); item != items.end(); item++)
	{
		delete (*item).second;
	}
	items.clear();
}

const ItemType *Items::GetItemType(int itemType)
{
	if(itemTypes.count(itemType) == 1) return itemTypes[itemType];
	else return NULL;
}

Item *Items::GetItem(int itemId)
{
	if(items.count(itemId) == 1) return items[itemId];
	else return NULL;
}

void Items::LoadTestCase()
{
	auto testSwordType = new Weapon(1);
	auto cupcakeType = new Consummable(2);

	itemTypes.insert(std::pair<int, ItemType*>(1, testSwordType) );
	itemTypes.insert(std::pair<int, ItemType*>(2, cupcakeType));
}