#include "MainMenu.hpp"

MainMenu::MainMenu() {
	input = Input::getInput();
	mouseButton.code = 0;
	mouseButton.mouse = 1;
	mouseButton.mod = 0;

	newGame = GuiButton(1800, 200, 100, 75, "New");
	loadGame = GuiButton(1800, 300, 100, 75, "Load");
	continueGame = GuiButton(1800, 400, 100, 75, "Continue");
	editor = GuiButton(1800, 500, 100, 75, "Editor");
	options = GuiButton(1800, 600, 100, 75, "Options");
	quit = GuiButton(1800, 700, 100, 75, "Quit");

}

void MainMenu::initMeshes(IRenderEngine * renderEngine) {
	newGameMesh = renderEngine->createMesh();
	loadGameMesh = renderEngine->createMesh();
	continueGameMesh = renderEngine->createMesh();
	editorMesh = renderEngine->createMesh();
	optionsMesh = renderEngine->createMesh();
	quitMesh = renderEngine->createMesh();

	newGameMesh->init(MeshPrimitiveType::TRIANGLE_STRIP);
	loadGameMesh->init(MeshPrimitiveType::TRIANGLE_STRIP);
	continueGameMesh->init(MeshPrimitiveType::TRIANGLE_STRIP);
	editorMesh->init(MeshPrimitiveType::TRIANGLE_STRIP);
	optionsMesh->init(MeshPrimitiveType::TRIANGLE_STRIP);
	quitMesh->init(MeshPrimitiveType::TRIANGLE_STRIP);

	Rect r = newGame.getBounds();
	vert v[] = { vert{ r.x + r.width, r.y + r.height, 0.0f, 1.0f, 1.0f },
		vert{ r.x + r.width, r.y, 0.0f, 1.0f, 0.0f },
		vert{ r.x, r.y + r.height, 0.0f, 0.0f, 1.0f },
		vert{ r.x, r.y, 0.0f, 0.0f, 0.0f }, };
	newGameMesh->setMeshData(v, sizeof(v), VERT_UV);

	r = loadGame.getBounds();
	vert v2[] = { vert{ r.x + r.width, r.y + r.height, 0.0f, 1.0f, 1.0f },
		vert{ r.x + r.width, r.y, 0.0f, 1.0f, 0.0f },
		vert{ r.x, r.y + r.height, 0.0f, 0.0f, 1.0f },
		vert{ r.x, r.y, 0.0f, 0.0f, 0.0f }, };
	loadGameMesh->setMeshData(v2, sizeof(v2), VERT_UV);

	r = continueGame.getBounds();
	vert v3[] = { vert{ r.x + r.width, r.y + r.height, 0.0f, 1.0f, 1.0f },
		vert{ r.x + r.width, r.y, 0.0f, 1.0f, 0.0f },
		vert{ r.x, r.y + r.height, 0.0f, 0.0f, 1.0f },
		vert{ r.x, r.y, 0.0f, 0.0f, 0.0f }, };
	continueGameMesh->setMeshData(v3, sizeof(v3), VERT_UV);

	r = editor.getBounds();
	vert v4[] = { vert{ r.x + r.width, r.y + r.height, 0.0f, 1.0f, 1.0f },
		vert{ r.x + r.width, r.y, 0.0f, 1.0f, 0.0f },
		vert{ r.x, r.y + r.height, 0.0f, 0.0f, 1.0f },
		vert{ r.x, r.y, 0.0f, 0.0f, 0.0f }, };
	editorMesh->setMeshData(v4, sizeof(v4), VERT_UV);

	r = options.getBounds();
	vert v5[] = { vert{ r.x + r.width, r.y + r.height, 0.0f, 1.0f, 1.0f },
		vert{ r.x + r.width, r.y, 0.0f, 1.0f, 0.0f },
		vert{ r.x, r.y + r.height, 0.0f, 0.0f, 1.0f },
		vert{ r.x, r.y, 0.0f, 0.0f, 0.0f }, };
	optionsMesh->setMeshData(v5, sizeof(v5), VERT_UV);

	r = quit.getBounds();
	vert v6[] = { vert{ r.x + r.width, r.y + r.height, 0.0f, 1.0f, 1.0f },
		vert{ r.x + r.width, r.y, 0.0f, 1.0f, 0.0f },
		vert{ r.x, r.y + r.height, 0.0f, 0.0f, 1.0f },
		vert{ r.x, r.y, 0.0f, 0.0f, 0.0f }, };
	quitMesh->setMeshData(v6, sizeof(v6), VERT_UV);
}

void MainMenu::releaseMeshes() {
	newGameMesh->release();
	loadGameMesh->release();
	continueGameMesh->release();
	editorMesh->release();
	optionsMesh->release();
	quitMesh->release();
}

bool MainMenu::isNewGamePressed() {
	
	return isPressed(newGame, input, mouseButton);
}

bool MainMenu::isLoadGamePressed() {
	return isPressed(loadGame, input, mouseButton);
}

bool MainMenu::isContinueGamePressed() {
	return isPressed(continueGame, input, mouseButton);
}

bool MainMenu::isEditorPressed() {
	return isPressed(editor, input, mouseButton);
}

bool MainMenu::isOptionsPressed() {
	return isPressed(options, input, mouseButton);
}

bool MainMenu::isQuitPressed() {
	return isPressed(quit, input, mouseButton);
}

void MainMenu::render() {
	newGameMesh->bind();
	newGameMesh->render();
	loadGameMesh->bind();
	loadGameMesh->render();
	continueGameMesh->bind();
	continueGameMesh->render();
	editorMesh->bind();
	editorMesh->render();
	optionsMesh->bind();
	optionsMesh->render();
	quitMesh->bind();
	quitMesh->render();
}
