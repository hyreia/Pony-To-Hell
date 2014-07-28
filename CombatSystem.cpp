#include "main.hpp"

using namespace DBL;

CombatSystem::CombatSystem(){}
CombatSystem::~CombatSystem(){}

void CombatSystem::QueueCombatEvent(CombatEvent ev)
{
	combatEvents.push(ev);
}

void CombatSystem::ProcessQueuedCombatEvents()
{
	while(!combatEvents.empty())
	{
		ColorfulString combatMessage;

		auto ev = combatEvents.front();
		auto actor = game->world.actorCollection.GetActor(ev.actorId);
		auto targetActorId = game->world.IsActorAtPosition(ev.x, ev.y, actor->position->depth);
		auto target = game->world.actorCollection.GetActor(targetActorId);

		//unarmed attack by actor on another actor
		if(ev.abilityId == 0 && ev.itemId == 0 && targetActorId != 0)
		{
			if(target->combatant && target->combatant->isHostile)
			{
				//pick one at random
				auto naturalAttackTypes = game->world.species.GetMainNaturalAttacks(actor->body->species);
				int attackType = naturalAttackTypes[rng::Rand(naturalAttackTypes.size())];

				int damageType = GetDamageTypeFromUnarmedAttack(attackType);
				//apply defense of targeted body part
				int hitChance = GetBaseHitPercentChance(attackType);

				//if(auto bodypart = target->body->GetBodyPartOfType(actor->combatant->targetBodyPartForAttacks))
				//{
				//	hitChance -= int(100*bodypart->calledShotPenalty);
				//}
				//calculate attack damage

				int damage = CalculateDamageFromAttackFormula(attackType, actor->combatant);
				printf("%d ATTACKED %d with %d for %d damage\n", actor->id, targetActorId, attackType, damage);
			

			

			
			
				//chance of hit or miss of targeted body part

				//if attack is going to hit
					//apply attack	
					//post a message that the attack hit for how much damage and if the attack killed it
				
				//else if attack missed 
					//post a message that the attack missed
			}

		}
		else if(ev.abilityId != 0 && ev.itemId == 0) //using an ability
		{
			printf("%d USING ABILITY %d (AT %d)\n", actor->id, ev.abilityId, targetActorId);
		}
		else if(ev.itemId != 0 && ev.abilityId == 0) //using an item
		{
			printf("%d USING ITEM %d (AT %d)\n", actor->id, ev.itemId, targetActorId);
		}
		//else else bare handed attacking nothing, doNothing()
		


		combatEvents.pop();
	}
}

int CombatSystem::GetDamageTypeFromUnarmedAttack(int unarmedAttackType)
{
	int ret = 0;

	switch(unarmedAttackType)
	{
		case SCRATCH: 
		case SHORT_SWORD:
			ret = SLASHING_DAMAGE; break;
		case PECK:
		case IMPALE:	
		case BITE:
			ret = PIERCING_DAMAGE; break;
		case TACKLE:
		case HEADBUTT:
		case KICK:
		case BUCK:
		case PUNCH:
			ret = BLUDGEON_DAMAGE;
		default:
		ret = UNTYPED_DAMAGE;
	}

	return ret;
}

int CombatSystem::GetBaseHitPercentChance(int attackFormula)
{
	int ret = 100;

	switch(attackFormula)
	{

		case BITE:
		case TACKLE:
			ret = 80;
		case BUCK:
		case PECK:
			ret = 90;

		case HEADBUTT:
		case KICK:

		case SCRATCH: 
		case IMPALE:	
		case PUNCH:
		case SHORT_SWORD:
		default:
		ret = 100;
			break;
	}
	return ret;
}

int CombatSystem::CalculateDamageFromAttackFormula(int attackType, Combatant *combatant)
{
	int result = 0;
	switch(attackType)
	{
		case BITE:
		case PECK:
		case SHORT_SWORD:
			result = rng::Dice(1,6) + combatant->Mod(STRENGTH);
		case TACKLE:
		case BUCK:
		case HEADBUTT:
		case KICK:
		case SCRATCH: 
		case IMPALE:	
		case PUNCH:
			result = rng::Dice(1,4) + combatant->Mod(STRENGTH);
		default:
			break;
	}

	return result;
}