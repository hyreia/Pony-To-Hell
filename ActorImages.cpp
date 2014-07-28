#include "main.hpp"

using namespace DBL;

const char *ActorImages::actorImages = "ActorImages";

void ActorImages::Deserialize(std::vector<std::string> tokens)
{
	int index = 0;
	isVisible = atoi(tokens[++index].c_str());
	int numOfImages = atoi(tokens[++index].c_str());

	while(numOfImages > 0)
	{
		IndexedImage nextImage;
		nextImage.index = atoi(tokens[++index].c_str());
		nextImage.tint.r = atoi(tokens[++index].c_str());
		nextImage.tint.g = atoi(tokens[++index].c_str());
		nextImage.tint.b = atoi(tokens[++index].c_str());
		nextImage.tint.a = atoi(tokens[++index].c_str());
		images.push_back(nextImage);

		numOfImages--;
	}
}

std::string ActorImages::Serialize()
{
	std::ostringstream serial;
	serial << ACT_COMP << 
		componentType << DELIM <<
		isVisible << DELIM <<
		images.size(); //number of images

	for(auto img = images.begin(); img != images.end(); img++)
	{
		//each image and tint
		serial << DELIM <<
			(*img).index << DELIM <<
			(int)(*img).tint.r << DELIM <<
			(int)(*img).tint.g << DELIM <<
			(int)(*img).tint.b << DELIM <<
			(int)(*img).tint.a;
	}
	std::string ret = serial.str();
	return ret;
}