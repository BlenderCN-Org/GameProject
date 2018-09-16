#ifndef GAME_HPP
#define GAME_HPP

///Internal Includes
#include "Asteroids/AsteroidsMap.hpp"
#include "Camera.hpp"
#include "CameraInput.hpp"
#include "CameraPath.hpp"
#include "Editor.hpp"
#include "MainMenu.hpp"
#include "PhysicsParticle.hpp"
#include "Player.hpp"
#include "RenderBatch.hpp"

#include "Editor/3dViewGizmo.hpp"
//#include "Loader/MapLoader.hpp"
#include "MapObjects/Map.hpp"
#include "MapObjects/Sky.hpp"

#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Mesh/Mesh.hpp"
#include "../Engine/Graphics/Mesh/MirrorMesh.hpp"
#include "../Engine/Graphics/Gui/TextureView.hpp"
#include "../Engine/Graphics/Gui/Cursor.hpp"
#include "../Engine/Utils/FPSCounter.hpp"

/// External Includes
#include <EngineCore/AssetHandling/Loader/MapLoader.hpp>
#include <EngineCore/AssetHandling/Loader/EditLoader.hpp>
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <vector>

enum class GameState {
	MAIN_MENU,
	PLAY,
	PLAY_ASTEROIDS,
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
	CEngine * engine;
	IMap* map;
	MainMenu* menu;

	Engine::Core::FPSCounter fpsCounter;

	GameState currentState;
	GameState lastState;

	Engine::DataLoader::MapLoader* mapLoader;
	Engine::DataLoader::EditLoader* editLoader;
	Engine::DataLoader::ILoader* activeLoader;

	CameraInput camInput;
	Camera camera;

	void updateMenu(float dt);
	void updatePlay(float dt);
	void updateEdit(float dt);
	void updateAsteroids(float dt);
	void updatePaused(float dt);

	void renderSky();
	void renderScene();
	void renderShadowMap();

	void present();

	void renderAsteroids();

	RenderBatch* batchTmp;

	Editor* editor;

	Engine::Graphics::GuiInfo guiInfo;

	Engine::Graphics::Gui::Panel* metrixPanel;
	Engine::Graphics::Gui::Label* infoLabel;
	
	//Player* player;

	glm::mat4 vpMat;
	
	Engine::Graphics::Mesh::CMesh* mesh;
	Engine::Graphics::Mesh::MirrorMesh* mirror;
	//Sky* sky;

	float r;
	float a;

	Engine::Graphics::Mesh::CMesh* aabbTest;

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

	IShaderObject* testShader;

	std::vector<PhysicsParticle*> physParticles;

	std::vector<Renderable*> frameObjects;
	
};


#endif
