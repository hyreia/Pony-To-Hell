#ifndef MAIN_HPP
#define MAIN_HPP

//STL headers
#include <string>
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <sstream>

//Allegro header files
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_native_dialog.h>

//Common utilitarian classes
#include "SplitString.hpp"
#include "XYPair.hpp"
#include "Directions.hpp"
#include "Random.hpp"
//Drawing-related utilitarian classes
#include "Color.hpp"
#include "IndexedImage.hpp"
#include "TextType.hpp"


//Actor headers
#include "Actor.hpp"
#include "ActorImages.hpp"
#include "Movement.hpp"
#include "Position.hpp"
#include "Villager.hpp"
#include "AttackAndAbilityTypes.hpp"
#include "ActorStatusEffects.hpp"
#include "BodyParts.hpp"
#include "Body.hpp"
#include "Ai.hpp"
#include "Item.hpp"
#include "ItemTypes.hpp"
#include "Combatant.hpp"

#include "Items.hpp"
#include "Tiles.hpp"
#include "Graphics.hpp"
#include "MessageQueue.hpp"
#include "CombatEvent.hpp"

#include "ActorCollection.hpp"
#include "LevelCollection.hpp"
#include "Species.hpp"

//Menu related
#include "CombatUserInterface.hpp"
#include "MenuButton.hpp"

//Game related
#include "Screen.hpp"
#include "LevelTransitionScreen.hpp"
#include "PressEnterMenu.hpp"
#include "PauseScreenMenu.hpp"
#include "QuitPromptScreen.hpp"
#include "GameScreen.hpp"
#include "TalkToActorMenu.hpp"

#include "ActorMovementManager.hpp"
#include "CombatSystem.hpp"

#include "World.hpp"
#include "Game.hpp"

#endif