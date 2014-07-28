#ifndef ACTOR_IMAGES_HPP
#define ACTOR_IMAGES_HPP

#include "ActorComponent.hpp"

namespace DBL
{
	enum ACTOR_IMAGE_TILE_INDEX
	{
		NO_TILE,
		PONY_TILE,
		UNICORN_TILE,
		PEGASUS_TILE,
		PONY_EYE_TILE,
		PONY_PUPIL_TILE,
		NO_TILE_6,
		SWORD_TILE,
		NO_TILE_8,
		HORSE_SKELETON_TILE,
		ADVENTURER_CLOTHES
	};

	class ActorImages: public ActorComponent
	{
	public:
		bool isVisible;
		//In to-draw order
		std::vector<IndexedImage> images;

		static const char *actorImages;
		
		ActorImages():
		isVisible(true),
		ActorComponent(actorImages)
		{}
		virtual ~ActorImages(){}

		void Deserialize(std::vector<std::string> tokens) override;
		std::string Serialize() override;

		
	};
};


#endif