#include "MainMenu.hpp"

#include "../Shaders.hpp"

MainMenu::MainMenu() {
	input = Input::getInput();
	mouseButton.code = 0;
	mouseButton.mouse = 1;
	mouseButton.mod = 0;

	newGame = GuiButton(1800, 550, 100, 30, "New");
	loadGame = GuiButton(1800, 600, 100, 30, "Load");
	continueGame = GuiButton(1800, 650, 100, 30, "Continue");
	editor = GuiButton(1800, 700, 100, 30, "Editor");
	options = GuiButton(1800, 750, 100, 30, "Options");
	quit = GuiButton(1800, 800, 100, 30, "Quit");

	mainMenuVisible = true;

}

void MainMenu::initMeshes(IRenderEngine * renderEngine) {
	renderer = renderEngine;
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

	f = renderEngine->createFont();
	f->init("C:/Windows/Fonts/Arial.ttf", 24);

	t = new Text();
	t->init(renderEngine);
	t->setFont(f);

	t->setText("ASDF", 4, 0, 0, 1.0f);

}

void MainMenu::releaseMeshes() {

	t->release();
	f->release();

	newGameMesh->release();
	loadGameMesh->release();
	continueGameMesh->release();
	editorMesh->release();
	optionsMesh->release();
	quitMesh->release();
}

void MainMenu::setVisible(bool visible) {
	mainMenuVisible = visible;
}

bool MainMenu::isVisible() {
	return mainMenuVisible;
}

void MainMenu::update() {

	int x, y;
	input->getMousePos(x, y);

	newGame.isPointInside(x, y);
	loadGame.isPointInside(x, y);
	continueGame.isPointInside(x, y);
	editor.isPointInside(x, y);
	options.isPointInside(x, y);
	quit.isPointInside(x, y);
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

void MainMenu::render(ICamera* camera) {
	if ( mainMenuVisible ) {
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


		renderer->setDepthTest(false);
		renderer->setBlending(true);
		Shaders::getShaders()->textShader.shaderObject->useShader();
		Shaders::getShaders()->textShader.shaderObject->bindData(Shaders::getShaders()->textShader.vp_location, UniformDataType::UNI_MATRIX4X4, camera->getOrthoMatrix());
		Shaders::getShaders()->textShader.shaderObject->bindData(Shaders::getShaders()->textShader.mdl_location, UniformDataType::UNI_MATRIX4X4, &glm::mat4());

		Rect r = newGame.getBounds();
		t->setText("New", 3, r.x, r.y, 1.0f);
		t->render(Shaders::getShaders()->textShader.shaderObject, Shaders::getShaders()->textShader.tex_location);

		r = loadGame.getBounds();
		t->setText("Load", 4, r.x, r.y, 1.0f);
		t->render(Shaders::getShaders()->textShader.shaderObject, Shaders::getShaders()->textShader.tex_location);

		r = continueGame.getBounds();
		t->setText("Continue", 8, r.x, r.y, 1.0f);
		t->render(Shaders::getShaders()->textShader.shaderObject, Shaders::getShaders()->textShader.tex_location);

		r = editor.getBounds();
		t->setText("Editor", 6, r.x, r.y, 1.0f);
		t->render(Shaders::getShaders()->textShader.shaderObject, Shaders::getShaders()->textShader.tex_location);

		r = options.getBounds();
		t->setText("Options", 7, r.x, r.y, 1.0f);
		t->render(Shaders::getShaders()->textShader.shaderObject, Shaders::getShaders()->textShader.tex_location);

		r = quit.getBounds();
		t->setText("Quit", 4, r.x, r.y, 1.0f);
		t->render(Shaders::getShaders()->textShader.shaderObject, Shaders::getShaders()->textShader.tex_location);

	}
}
