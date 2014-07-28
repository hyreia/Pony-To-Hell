#ifndef SPECIES_HPP
#define SPECIES_HPP

namespace DBL
{
	/* A collection of species, listing their body plans, sizes, type of attacks, etc. 
	
	Eventually this would probably be loaded dynamically from a 
	*/
	class Species
	{
	public:
		Species(){}
		~Species(){}
		//void Init(); ?

		std::list<BodyPart> Species::CreateBodyPartsForSpecies(int species);

		/* Weight is in grams, as all weights in the game will be*/
		int GetBaseWeightForSpecies(int species, int sex);

		std::vector<int> GetMainNaturalAttacks(int species);

	private:
		std::list<BodyPart> GetPonyBodyPlan();
		std::list<BodyPart> GetGriffonBodyPlan();
		std::list<BodyPart> GetKoboldBodyPlan();

	};
};

#endif