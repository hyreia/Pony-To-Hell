#include "main.hpp"

using namespace DBL;

void LevelCollection::LoadLevelsFromFile()
{
	const char extension[] = ".lvl";
	std::ostringstream ss;
	int depth = 0;

	ss << depth << extension;
	
	ALLEGRO_FILE *file = al_fopen(ss.str().c_str(), "r");

	while(file)
	{
		levels.insert(std::pair<int, Tileset>(depth, Tileset()));

		int levelWidth = al_fread32le(file);
		int levelHeight = al_fread32le(file);

		if(al_ferror(file) || al_feof(file))
		{
			break;
		}
		std::vector<LevelTile> tileColumn(levelHeight, LevelTile(1));
		std::vector< std::vector<LevelTile> > tileRow(levelWidth, tileColumn);
		int tilesLoaded = levelWidth*levelHeight;
		int x = 0, y = 0;
		while(tilesLoaded != 0)
		{
			int read = al_fread32le(file);
			if(al_feof(file) || al_ferror(file))
			{
				break;
			}
			if(read > 0) //is a tile material
			{
				tileRow[x][y].tileMaterialIndex = read;
				int variant = game->world.materials.Get(read).numOfTintVariations;
				tileRow[x][y].tintVariation = rng::Rand(variant);
				tilesLoaded--;

				if(read == UP_STAIRS)
				{
					levels[depth].PosOfUpStairs = new XYPair<>(x, y);
				}
				else if(read == DOWN_STAIRS)
				{
					levels[depth].PosOfDownStairs = new XYPair<>(x, y);
				}

				x++;
				if(x >= (int)tileRow.size()){ y++; x = 0; }
				if(y >= (int)tileRow[0].size())
				{
					break;
				}
			}


			if(al_ferror(file)) break;
		}


		al_fclose(file);
		file = NULL;
		ss.str("");

		levels[depth].tiles = tileRow;

		depth++;
		ss << depth << extension;
		file = al_fopen(ss.str().c_str(), "r");
	}


}

void LevelCollection::SaveLevelsToFile()
{
	//Maps are so small there's not really a need for RLE, so just write each tile out
	int depth = 0;
	const char extension[] = ".lvl";
	std::ostringstream ss;

	while( levels.count(depth) == 1)
	{
		ss.str("");
		ss << depth << extension;
		ALLEGRO_FILE *file = al_fopen(ss.str().c_str(), "w");
		if(file)
		{
			//width and height
			al_fwrite32le(file, (int)levels[depth].tiles.size());
			al_fwrite32le(file, (int)levels[depth].tiles[0].size());

			int previousMaterial = 0;
			int numOfTilesRepeated = 0;
			int x = 0, y = 0;
			for(y = 0; y < (int)levels[depth].tiles[0].size(); y++)
			for(x = 0; x < (int)levels[depth].tiles.size(); x++)
			{

				int currentMaterial = levels[depth].tiles[x][y].tileMaterialIndex;
				al_fwrite32le(file, currentMaterial);
				
			}

			al_fclose(file);
		}
		
		depth++;
	}

}