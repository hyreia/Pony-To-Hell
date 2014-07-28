#ifndef ACTOR_COMPONENTS_HPP
#define ACTOR_COMPONENTS_HPP

/*
Actors are a composition of different types of components. This is the base
class that all components are based off of.

This describes what all actor components have in common: they can be serialized and
deserialized. Implementation up to the class or subclass. 

Serialization:
The recommended approach to serialization is to create a (slow) iostream
<< the ACT_COMP to show that it's an actor component, and <<
all the information necessary to reproduce it, separating each by the DELIM
The list should start with the name of the ActorComponent struct (stored statically somewhere in
the ActorComponent struct so it can be accessed elsewhere when deserializing)

ex: Serialize returning "+Type+SubType+Variable1+Variable2+Variable3"

Deserialization should be simplified by tokenizing the string by the delimiter (DELIM)
and converting each string into whatever data it needs to be (atoi, atof, etc)

The correct component to feed the tokenized string to must be determined by the type
and optional subtype.

Care needs to be serialize and deserialize any variables of the components in the same order
in both processes
*/

struct ActorComponent
{
	virtual void Deserialize(std::vector<std::string> tokens) = 0;
	virtual std::string Serialize() = 0;

	static const char ACT_COMP = '\t';
	static const char DELIM = ':';

	const char *componentType;
protected:
	ActorComponent(const char *componentType): componentType(componentType)
	{}
	virtual ~ActorComponent(){}

};

#endif