#include "main.hpp"

using namespace DBL;

void ItemType::Deserialize(std::vector<std::string> tokens)
{
	typeId = atoi(tokens[TYPE_ID].c_str());
	itemUsage = atoi(tokens[ITEM_USAGE].c_str());
	name = tokens[ITEM_NAME];
	description = tokens[ITEM_DESC];
	itemTileImage.index = atoi(tokens[ITEM_TILE_IMAGE_INDEX].c_str());
	itemTileImage.tint.r = atoi(tokens[ITEM_TILE_IMAGE_R].c_str());
	itemTileImage.tint.g = atoi(tokens[ITEM_TILE_IMAGE_G].c_str());
	itemTileImage.tint.b = atoi(tokens[ITEM_TILE_IMAGE_B].c_str());
	weight = atoi(tokens[ITEM_WEIGHT].c_str());
	size = atoi(tokens[ITEM_SIZE].c_str());
	monetaryValue = atoi(tokens[ITEM_SIZE].c_str());

	DeserializeItemType(tokens);
}
std::string ItemType::Serialize()
{
	std::ostringstream serial;

	serial << typeId << DELIM <<
		itemUsage << DELIM <<
		description.c_str() << DELIM <<
		itemTileImage.index << DELIM <<
		(int)itemTileImage.tint.r << DELIM <<
		(int)itemTileImage.tint.g << DELIM <<
		(int)itemTileImage.tint.b << DELIM <<
		weight << DELIM <<
		size << DELIM <<
		monetaryValue << DELIM << SerializeItemType();

	return serial.str();
}

void Weapon::DeserializeItemType(std::vector<std::string> tokens)
{

}
std::string Weapon::SerializeItemType()
{ 
	std::string ret; 
	
	return ret; 
}

void Armor::DeserializeItemType(std::vector<std::string> tokens){}
std::string Armor::SerializeItemType()
{ 
	std::string ret; 
	
	return ret; 
}

void Consummable::DeserializeItemType(std::vector<std::string> tokens){}
std::string Consummable::SerializeItemType()
{ 
	std::string ret; 
	
	return ret; 
}

void Scroll::DeserializeItemType(std::vector<std::string> tokens){}
std::string Scroll::SerializeItemType()
{ 
	std::string ret; 
	
	return ret; 
}

void Book::DeserializeItemType(std::vector<std::string> tokens){}
std::string Book::SerializeItemType()
{ 
	std::string ret; 
	
	return ret; 
}