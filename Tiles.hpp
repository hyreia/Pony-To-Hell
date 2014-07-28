#ifndef TILESET_HPP
#define TILESET_HPP

namespace DBL
{
	const int TILE_SIZE = 64;
	const double DIAGONAL_MOVEMENT_COEFF = 0.7071f;

	inline XYPair<double> TileToAbsolutePos(int tileX, int tileY)
	{
		return XYPair<double>(tileX*TILE_SIZE, tileY*TILE_SIZE);
	}
	inline XYPair<double> TileToAbsolutePos(XYPair<> tilePos)
	{
		return TileToAbsolutePos(tilePos.x, tilePos.y);
	}

	inline XYPair<int> AbsoluteToTilePos(double x, double y)
	{
		return XYPair<int>(x/TILE_SIZE, y/TILE_SIZE);
	}

	inline double TileToAbs(int tile){ return tile*TILE_SIZE; }
	inline int AbsToTile(int abs){ return abs/TILE_SIZE; }

	enum TILE_MATERIALS
	{
		GRASS = 1,
		STONE_BLOCK,
		WOODEN_FLOOR,
		DOWN_STAIRS,
		UP_STAIRS,
		DIRT,
	};

	/* Description of a type of tile and its properties */
	struct TileMaterial
	{
		TileMaterial(bool blocksSight, bool blocksMovement, const std::string &desc,
		int _numOfTintVariations = 0):
		isBlockingSight(blocksSight), 
		isBlockingMovement(blocksMovement), 
		description(desc),
		numOfTintVariations(_numOfTintVariations)
		{}

		bool isBlockingSight, isBlockingMovement;
		int numOfTintVariations;

		std::string description;
	};

	/* A collection of tile material properties, which when created are to line up
	with the graphics in the tile assets */
	class Materials
	{
	public:
		Materials(){}
		~Materials(){}

		void Init()
		{
			//Grass
			collection.push_back(TileMaterial(false, false, "grass", 12));
			//Stone Block
			collection.push_back(TileMaterial(true, true, "stone slab", 15));
			//Wooden Floor
			collection.push_back(TileMaterial(false, false, "wooden floor", 3));
			//Down Stairs
			collection.push_back(TileMaterial(false, false, "down stairs", 3));
			//Up Stairs
			collection.push_back(TileMaterial(false, false, "up stairs", 3));
			//dirt
			collection.push_back(TileMaterial(false, false, "dirt", 5));
		}
		
		inline bool IsAMaterialAtIndex(int index){ return index-1 < (int)collection.size()-1; }
		inline TileMaterial& Get(int index){ return collection[index-1]; }

		std::vector<TileMaterial> collection;

	};

	/* What's at a given tile location (the material id and a draw tint variation */
	struct LevelTile
	{
		LevelTile(): tileMaterialIndex(0), tintVariation(0)
		{}

		LevelTile(int _tileMaterialIndex): 
		tileMaterialIndex(_tileMaterialIndex), 
		tintVariation(0)
		{}

		LevelTile(int _tileMaterialIndex, int _tintVariation):
		tileMaterialIndex(_tileMaterialIndex),
		tintVariation(_tintVariation)
		{}


		/* describes what is at that location */
		int tileMaterialIndex;

		//Tint variation to use here when drawing the material
		int tintVariation;
	};




	//Just the id of the tiles in the tileset
	//the id itself is a reference to the tilest that describes
	//what was there
	class  Tileset
	{
	public:
		int64_t lastTimeVisited;

		Tileset():
		  PosOfDownStairs(NULL),
		  PosOfUpStairs(NULL),
		  lastTimeVisited(0)
		{}

		  ~Tileset()
		  {
			if(PosOfUpStairs) delete PosOfUpStairs;
			if(PosOfDownStairs) delete PosOfDownStairs;
		  }

		//2D array of tile types
		std::vector< std::vector<LevelTile> > tiles;

		XYPair<> *PosOfDownStairs, *PosOfUpStairs;
	};
};
#endif

