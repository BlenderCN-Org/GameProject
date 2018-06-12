#ifndef SKY_HPP
#define SKY_HPP

/// Internal Includes
#include "../Loader/MapLoader.hpp"
#include "../../Engine/Graphics/SkyDome.hpp"

/// External Includes
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

	Sky();
	Sky(LoadedData &skyData);
	~Sky();

	void update(float dt);

	void render(glm::mat4& vpMat, glm::vec3& cameraPos, glm::vec3& cameraDir, glm::mat4* reflect = nullptr);

private:

	void setup();

	bool verifyData(LoadedData &data);

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