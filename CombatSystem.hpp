#ifndef COMBAT_SYSTEM_HPP
#define COMBAT_SYSTEM_HPP

namespace DBL
{
	class CombatSystem
	{
	public:
		CombatSystem();
		~CombatSystem();
		void QueueCombatEvent(CombatEvent ev);
		void ProcessQueuedCombatEvents();

	private:
		std::queue<CombatEvent> combatEvents;

		int GetDamageTypeFromUnarmedAttack(int unarmedAttackType);
		int GetBaseHitPercentChance(int attackFormula);

		int CalculateDamageFromAttackFormula(int attackType, Combatant *combatant);
		
	};
};

#endif