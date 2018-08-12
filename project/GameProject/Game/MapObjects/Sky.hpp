#ifndef SKY_HPP
#define SKY_HPP

/// Internal Includes
#include "../../Engine/Graphics/SkyDome.hpp"

/// External Includes
#include <EngineCore/AssetHandling/Loader/MapLoader.hpp>

#include <glm/glm.hpp>
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

struct SkyData {
	float skyColorDay[3];
	float skyColorNight[3];

	float cycleTime;
};

class Sky {

public:

	//Sky();
	Sky(Engine::DataLoader::ILoader** loader, uint32_t skyId);
	virtual ~Sky();

	void reloadCheck();

	void update(float dt);

	void render(glm::mat4& vpMat, glm::vec3& cameraPos, glm::vec3& cameraDir, glm::mat4* reflect = nullptr);

private:

	Engine::DataLoader::ILoader** ppLoader;
	Engine::AssetHandling::EntryData skyData;

	void setup();

	bool verifyData(Engine::AssetHandling::EntryData &data);

	SkyDome *skyDome;

	IShaderObject* skyDomeShader;

	int skydomeVpLocation;
	int skydomeMatLocation;
	int skydomeTimeLoc;
	int skydomeCamPos;
	int skydomeEyeDir;
	int skydomeSunMoon;

	int skyDayColorLoc;
	int skyNightColorLoc;

	float skyTime;
	float skyTimeScaled;
	glm::vec3 sunMoonDir;

	glm::vec3 skyColorDay;
	glm::vec3 skyColorNight;

	float cycleTime;

};

#endif