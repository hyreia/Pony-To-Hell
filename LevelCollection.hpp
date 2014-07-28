#ifndef LEVEL_COLLECTION_HPP
#define LEVEL_COLLECTION_HPP

namespace DBL
{
	/* Container of levels with helper functions */
	struct LevelCollection
	{
		/* Loads levels called creative names like '0.lvl', '1.lvl' until it can't find anymore*/
		void LoadLevelsFromFile();
		void SaveLevelsToFile();


		std::map<int, Tileset> levels;
		inline bool IsLevelAtIndex(int index)
		{
			return levels.count(index) == 1;
		}
	};
};

#endif