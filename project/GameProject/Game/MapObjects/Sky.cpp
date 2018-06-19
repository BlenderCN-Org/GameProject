
/// Internal Includes
#include "Sky.hpp"
#include "../../Engine/Graphics/Graphics.hpp"
#include "../../Engine/Core/System.hpp"

/// External Includes
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

/// Std Includes
#include <fstream>
#include <string>

Sky::Sky() {
	setup();

	skyColorDay = glm::vec3(0.3F, 0.55F, 0.8F);
	skyColorDay = glm::vec3(0.3F, 0.0F, 0.8F);
	skyColorNight = glm::vec3(0.024422F, 0.088654F, 0.147314F);

	cycleTime = 120.0F;
}

Sky::Sky(LoadedData & skyData) {
	setup();

	if (verifyData(skyData)) {

		SkyData* sk = (SkyData*)skyData.data;

		skyColorDay = glm::vec3(sk->skyColorDay[0], sk->skyColorDay[1], sk->skyColorDay[2]);
		skyColorNight = glm::vec3(sk->skyColorNight[0], sk->skyColorNight[1], sk->skyColorNight[2]);

		cycleTime = sk->cycleTime;

	} else {
		skyColorDay = glm::vec3(0.3F, 0.55F, 0.8F);
		skyColorNight = glm::vec3(0.024422F, 0.088654F, 0.147314F);

		cycleTime = 120.0F;
	}
}

Sky::~Sky() {
	delete skyDome;

	skyDomeShader->release();
}

void Sky::update(float dt) {

	skyTime += dt;
	if (skyTime > cycleTime) {
		skyTime -= cycleTime;
	}

	skyTimeScaled = skyTime / cycleTime;

	const float r = 100.0F;
	const float PI = glm::pi<float>();

	float s = (glm::abs(glm::sin(PI * skyTimeScaled * 2.0F + (PI * 0.5F))) * 1.6F) - 0.6F;
	float c = glm::cos(PI * skyTimeScaled * 2.0F + (PI * 0.5F));

	if (skyTimeScaled >= 0.25F && skyTimeScaled <= 0.75F) {
		c = -c;
	}
	if (s > 0.0F) {
		s *= 0.2F;
	}

	sunMoonDir = glm::vec3(-r * 0.4F, r * s, r * c);

	sunMoonDir = normalize(glm::vec3(0) - sunMoonDir);

}

void Sky::render(glm::mat4& vpMat, glm::vec3& cameraPos, glm::vec3& cameraDir, glm::mat4* reflect) {

	skyDomeShader->useShader();

	glm::mat4 m;
	m = glm::translate(m, cameraPos);
	m = glm::transpose(m);


	skyDomeShader->bindData(skydomeVpLocation, UniformDataType::UNI_MATRIX4X4, &vpMat);
	skyDomeShader->bindData(skydomeMatLocation, UniformDataType::UNI_MATRIX4X4, &m);
	skyDomeShader->bindData(skydomeTimeLoc, UniformDataType::UNI_FLOAT, &skyTimeScaled);
	skyDomeShader->bindData(skydomeCamPos, UniformDataType::UNI_FLOAT3, &cameraPos);
	skyDomeShader->bindData(skydomeEyeDir, UniformDataType::UNI_FLOAT3, &cameraDir);

	skyDomeShader->bindData(skyDayColorLoc, UniformDataType::UNI_FLOAT3, &skyColorDay);
	skyDomeShader->bindData(skyNightColorLoc, UniformDataType::UNI_FLOAT3, &skyColorNight);

	if (reflect) {
		sunMoonDir = glm::vec3(glm::vec4(sunMoonDir, 1.0F) * (*reflect));
	}

	skyDomeShader->bindData(skydomeSunMoon, UniformDataType::UNI_FLOAT3, &sunMoonDir);

	gRenderEngine->depthMask(false);

	skyDome->render();

	gRenderEngine->depthMask(true);

}


void Sky::setup() {

	skyDome = new SkyDome();

	skyDomeShader = gRenderEngine->createShaderObject();
	skyDomeShader->init();

	std::string vs = Engine::System::readShader("data/shaders/skydome.vs.glsl");
	std::string fs = Engine::System::readShader("data/shaders/skydome.fs.glsl");

	skyDomeShader->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	skyDomeShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	skyDomeShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

	if (!skyDomeShader->buildShader()) {
		assert(0 && "shader failed to build");
	}

	skydomeVpLocation = skyDomeShader->getShaderUniform("viewProjMatrix");
	skydomeMatLocation = skyDomeShader->getShaderUniform("worldMat");
	skydomeTimeLoc = skyDomeShader->getShaderUniform("time");
	skydomeCamPos = skyDomeShader->getShaderUniform("cameraPos");
	skydomeEyeDir = skyDomeShader->getShaderUniform("eyeDir");
	skydomeSunMoon = skyDomeShader->getShaderUniform("sunMoonDir");

	skyDayColorLoc = skyDomeShader->getShaderUniform("daySky");
	skyNightColorLoc = skyDomeShader->getShaderUniform("nightSky");

}

bool Sky::verifyData(LoadedData &data) {

	bool dataOk = false;

	if (data.tag == SKY_TAG) {

		SkyData* sk = (SkyData*)data.data;

		uint32_t calcSize = sizeof(sk->cycleTime) + sizeof(sk->skyColorDay) + sizeof(sk->skyColorNight);

		// @ TODO add checks for nan and negative values

		if (calcSize == data.size) {
			dataOk = true;
		}
		
	}

	return dataOk;

}