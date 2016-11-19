#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "Gui.hpp"
#include "GuiButton.hpp"

#include "../Core/Input.hpp"

class MainMenu : public Gui
{
public:

	MainMenu();

	void initMeshes(IRenderEngine* renderEngine);
	void releaseMeshes();

	bool isNewGamePressed();
	bool isLoadGamePressed();
	bool isContinueGamePressed();
	bool isEditorPressed();
	bool isOptionsPressed();
	bool isQuitPressed();

	void render();

private:

	GuiButton newGame;
	GuiButton loadGame;
	GuiButton continueGame;
	GuiButton editor;
	GuiButton options;
	GuiButton quit;

	Input* input;

	KeyBind mouseButton;

	IMesh* newGameMesh;
	IMesh* loadGameMesh;
	IMesh* continueGameMesh;
	IMesh* editorMesh;
	IMesh* optionsMesh;
	IMesh* quitMesh;

	struct vert
	{
		float x, y, z, u, v;

	};

};

#endif