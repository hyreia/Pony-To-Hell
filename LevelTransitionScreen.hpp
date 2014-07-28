#ifndef LEVEL_TRANSITION_SCREEN_HPP
#define LEVEL_TRANSITION_SCREEN_HPP

/* Fades out the current game level screen in 1 second,
loads level player is on (if necessary),
sets all NPCs not following player or player's party to inactive
make all NPCs on level active
temporarily remove player from level
simulate level from last time it was active to current time or until time elapse cap
add player back to level
set game level to level player is on
fade current game level screen in 1 second
set game state to GameScreen
*/

namespace DBL
{
	class LevelTransitionScreen: public Screen
	{
	public:
		LevelTransitionScreen();
		virtual ~LevelTransitionScreen();

		void HandleInput(ALLEGRO_EVENT &ev) override;
		void Output() override;
		void Process() override;

	private:
		enum LEVEL_TRANSITION_STATE
		{
			FADING_OUT,
			LOADING,
			FADING_IN
		};
		int nextLevel; // so we can temporarily remove player from level
		int transitionState;
		int transitionTime; //used to track elapsed time

	};
};

#endif