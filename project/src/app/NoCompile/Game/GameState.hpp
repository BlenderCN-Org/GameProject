#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

enum class GameState : unsigned char {
	eGameStage_MainMenu = 0,
	eGameState_EditorMode = 1,
	eGameState_PlayMode = 2,
	eGameState_loading = 3,

	eGameState_Undefined = 99,
};

#endif