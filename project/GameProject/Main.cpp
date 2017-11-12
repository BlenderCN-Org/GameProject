//#include "Game\Game.hpp"

#include <time.h>
#include <string>

#include "ExceptionHandling.hpp"

#include <Memory\MemoryMan.hpp>

#include <conio.h>

#include "Engine/Engine.hpp"
#include "Engine/Input/Input.hpp"
#include "Engine/Interfaces/IDataParser.hpp"
#include "Engine/Graphics/Gui.hpp"
#include "Engine/Graphics/Gui/Panel.hpp"
#include "Engine/Graphics/Gui/Label.hpp"
#include "Engine/Graphics/Gui/ProgressBar.hpp"
//#include <vld.h>

//void reset(Game*& g) {
//	Sleep(5000);
//	delete g;
//	Sleep(5000);
//	g = new Game();
//	g->init();
//}

/*Game* g = new Game();
g->init();

while (g->isRunning()) {
if (g->updateAndRender(dt) == false) {
reset(g);
}

unsigned int temp = clock();
dt = unsigned int(temp - start) / 1000.0f;
start = temp;
}

delete g;*/

int main(int argc, char* argv[]) {
	InitMemoryManagement();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	printf("Waiting for RenderDoc injection, Press any key to continue...\n");
	_getch();

	initExceptionHandlers();

	float dt = 0.0f;
	unsigned int start = clock();

	CEngine* e = new CEngine();

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	Engine::Graphics::CGui* gui = new Engine::Graphics::CGui();
	Engine::Graphics::Gui::Panel* panel = new Engine::Graphics::Gui::Panel();

	Engine::Graphics::Gui::Panel* panel2 = new Engine::Graphics::Gui::Panel();

	Engine::Graphics::Texture::Texture2D* tex = new Engine::Graphics::Texture::Texture2D();
	Engine::Graphics::Texture::Texture2D* tex2 = new Engine::Graphics::Texture::Texture2D();

	Engine::Graphics::Gui::Label* label = new Engine::Graphics::Gui::Label();

	Engine::Graphics::Gui::ProgressBar* progress = new Engine::Graphics::Gui::ProgressBar();
	Engine::Graphics::Texture::Texture2D* pBack = new Engine::Graphics::Texture::Texture2D();
	Engine::Graphics::Texture::Texture2D* pProg = new Engine::Graphics::Texture::Texture2D();

	tex->singleColor(0.5F, 0.5F, 0.0F, 1.0F);
	tex2->singleColor(0.0F, 0.5F, 0.0F, 1.0F);

	pBack->singleColor(0.0F, 0.0F, 0.0F, 1.0F);
	pProg->singleColor(1.0F, 0.0F, 0.0F, 1.0F);

	panel->setPosition(0, 0);
	panel->setSize(100, 100);
	panel->setAnchorPoint(Engine::Graphics::GuiAnchor::CENTER);
	panel->setVisible(true);
	panel->setTexture(tex);

	panel2->setPosition(0, 0);
	panel2->setSize(200, 10);
	panel2->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	panel2->setVisible(true);
	panel2->setTexture(tex2);

	label->setPosition(0, 0);
	label->setSize(200, 10);
	label->setAnchorPoint(Engine::Graphics::GuiAnchor::CENTER);
	label->setVisible(true);

	progress->setPosition(-50, 0);
	progress->setSize(50, 10);
	progress->setAnchorPoint(Engine::Graphics::GuiAnchor::BOTTOM_LEFT);
	progress->setVisible(true);
	progress->setRange(0U, 1000U);
	progress->setValue(500U);
	progress->setBackgorundTexture(pBack);
	progress->setProgressTexture(pProg);

	gui->addGuiItem(panel);
	panel->addGuiItem(panel2);
	panel->addGuiItem(progress);

	gui->addGuiItem(label);

	uint32_t value = 500U;

	printf("starting loop\n");

	while (e->isRunning()) {

		if (in->wasPressedThisFrame(Engine::Input::KeyBindings[Engine::Input::KEYBIND_ENTER], false)) {
			printf("Enter was pressed this frame\n");
		}
		if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_FORWARD], false)) {
			printf("Forward is begin pressed\n");
		}

		e->update(dt);
		//e->clearBackBuffer();
		e->clearDebug();

		gui->render();

		e->presentFrame();

		unsigned int temp = clock();
		dt = unsigned int(temp - start) / 1000.0f;
		start = temp;

		progress->setValue(value);

		value++;
		if (value > 1000U) {
			value = 0U;
		}

	}

	printf("finished execution\n");

	delete progress;
	delete pBack;
	delete pProg;

	delete label;

	delete tex;
	delete tex2;

	delete panel;
	delete panel2;
	delete gui;

	delete e;
	ReleaseMemoryManagement();

	return 0;
}