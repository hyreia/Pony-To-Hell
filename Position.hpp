#ifndef POSITION_HPP
#define POSITION_HPP

#include "ActorComponent.hpp"

namespace DBL
{
	enum Elevation
	{
		ON_GROUND,
		STANDING,
		IN_AIR
	};



	class Position: public ActorComponent
	{

	public:
		int elevation;
		int depth;	//0 = village, >0 = dungeon depth
		XYPair<> tile;	//tile
		XYPair<double> absolutePos; //absolute postion

		static const char *position;

		Position(int absPosX, int absPosY, int tileSize, int _elevation, int _level):
		elevation(_elevation),
		depth(_level),
		tile(absPosX/tileSize, absPosY/tileSize),
		absolutePos(absPosX, absPosY),
		ActorComponent(position)
		{}


		Position():
		ActorComponent(position),
		elevation(0),
		depth(0)
		{}

		virtual ~Position(){}



		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;

	private:
		enum POSITION_SERIALIZATION
		{
			TILE_POSX=1, 
			TILE_POSY,
			ABS_POSX,
			ABS_POSY,
			ELEVATION,
			LEVEL
		};

	};
};

#endif