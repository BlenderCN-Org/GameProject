#ifndef GAME_HPP
#define GAME_HPP

///Internal Includes
#include "CameraInput.hpp"
#include "Camera.hpp"
#include "CameraPath.hpp"
#include "Player.hpp"
#include "Editor.hpp"
#include "Sky.hpp"

#include "Map.hpp"

#include "RenderBatch.hpp"

#include "MainMenu.hpp"

#include "Loader/MapLoader.hpp"

#include "../Engine/RefObject.hpp"
#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Mesh/Mesh.hpp"
#include "../Engine/Graphics/Mesh/MirrorMesh.hpp"
#include "../Engine/Graphics/Gui/TextureView.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <vector>

enum class GameState {
	MAIN_MENU,
	PLAY,
	EDIT,
	PAUSE,
};

class Game {
public:

	Game(CEngine* _engine);
	virtual ~Game();

	void update(float dt, uint64_t clocks);

	void render();

private:
	
	MapLoader mapLoader;

	CEngine * engine;
	CameraInput camInput;
	Camera camera;

	Map* map;

	GameState currentState;
	GameState lastState;

	void updateFps(float dt);

	void updateMenu(float dt);
	void updatePlay(float dt);
	void updateEdit(float dt);
	void updatePaused(float dt);

	void renderSky();
	void renderScene();
	void renderShadowMap();

	RenderBatch* batchTmp;

	MainMenu* menu;

	Engine::RefObject<Editor>* editor;

	Engine::Graphics::CGui* gameGui;
	Engine::Graphics::Gui::Panel* metrixPanel;
	Engine::Graphics::Gui::Label* infoLabel;
	Engine::Graphics::Texture::Texture2D* panelTexture;

	CameraPath camPath;

	Player* player;

	glm::mat4 vpMat;

	Engine::Graphics::Mesh::CMesh* mesh;
	Engine::Graphics::Mesh::MirrorMesh* mirror;
	Sky* sky;

	float r;
	float a;

	IFrameBuffer* gBuffer;
	IFrameBuffer* shadowMap;

	IShaderObject* shader;
	IShaderObject* gBufferShader;
	IShaderObject* gBufferBlit;

	IShaderObject* shadowShader;

	int vpLocation;
	int matLocation;
	int selectedLoc;

	int vpLocationGBuff;
	int matLocationGBuff;
	int refMatLocationGBuff;
	int selectedLocGBuff;
	int clipPlane;
	int skinArray;
	
	int blitTexDiff;
	int blitTexNorm;
	int blitTexWPos;
	int blitTextShadow;
	int blitDepthMvp;
	int blitEyePos;

	int shadowMVP;

	float dtOneSec;
	int fps;
	int fpsCounter;
	
	std::vector<Renderable*> frameObjects;

};	


#endif
