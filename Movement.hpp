#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include "ActorComponent.hpp"

namespace DBL
{

class Movement: public ActorComponent
{
public:
	double baseMoveSpeed;
	double carryOverMomentum; //intentionally not serialized

	XYPair<int> destination;
	int destinationDepth;
	bool isMoving;
	int targetActorId;

	static const char *movement;

	Movement(double baseSpeed):
	  baseMoveSpeed(baseSpeed),
	  carryOverMomentum(0),
	  ActorComponent(movement),
	  isMoving(false),
	  destinationDepth(0),
	  targetActorId(0)
	  {}

	  Movement():
	  baseMoveSpeed(0),
	  carryOverMomentum(0),
	  ActorComponent(movement),
	  isMoving(false),
	  destinationDepth(0),
	  targetActorId(0)
	  {}
	  
	  virtual ~Movement(){}

		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;
	private:
		enum MOVEMENT_SERIALIZATION
		{
			BASE_MOVE_SPEED=1,
			DESTINATION_X,
			DESTINATION_Y,
			DESTINATION_LEVEL,
			TARGET_ACTOR_ID,
			IS_MOVING
		};
};

};

#endif