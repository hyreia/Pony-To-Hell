#ifndef ITEMS_HPP
#define ITEMS_HPP

namespace DBL
{
	/* Not only does it store items by index, it also stores item types */
	class Items
	{
	public:
		Items();
		~Items();

		/* Loads item types and items */
		void Initialize();
		void Save();
		void Free();

		const ItemType *GetItemType(int itemType);
		Item *GetItem(int itemId);

		std::map<int, ItemType*> itemTypes;
		std::map<int, Item*> items;
	private:

		void LoadTestCase();

	};
};

#endif