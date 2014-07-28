#ifndef TALK_TO_ACTOR_MENU_HPP
#define TALK_TO_ACTOR_MENU_HPP

namespace DBL
{
	enum TALK_TO_ACTOR_MENU_SCREEN
	{
		TALK_MAIN_STATE,
		TALK_GREET_STATE,
		TALK_CONVERSATION_STATE,
		TALK_RECRUIT_STATE,
		TALK_FLIRT_STATE,
		TLAK_NOTHING_STATE
	};

	class TalkToActorMenu: public Screen
	{
	public:
		TalkToActorMenu(int actorId);
		~TalkToActorMenu();
		void HandleInput(ALLEGRO_EVENT &ev) override;
		void Output() override;
		void Process() override;
	private:
		MenuButton greet, talk, recruit, flirt, nothing;
		int highlightedButton;
		int talkMenuState;
		std::string message;

		int conversatingActorId;
		Actor *conversatingActor;
	};
};


#endif