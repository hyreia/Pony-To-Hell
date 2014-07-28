#ifndef ITEM_HPP
#define ITEM_HPP

namespace DBL
{

	enum ITEM_TYPE
	{
		ITEM_TYPE_NONE,
		ITEM_TYPE_WEAPON,
		ITEM_TYPE_ARMOR,
		ITEM_TYPE_CONSUMMABLE,
		ITEM_TYPE_SCROLL,
		ITEM_TYPE_BOOK
	};

	/* Item class, covers the full extent of what an item can do*/
	class Item: public ActorComponent
	{
	public:
		static const char *item;
		Item(int instance, int type);

		int instanceId;	//id of this individual item instance
		int itemType;	//index of description of this item
		double usesLeft;

		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;
	private:

		enum ITEM_SERIAL_ENUM
		{
			INSTANCE_ID = 1,
			ITEM_TYPE,
			USES_LEFT
		};
	};
};


#endif