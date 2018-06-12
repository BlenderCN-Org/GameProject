#ifndef RENDER_BATCH_HPP
#define RENDER_BATCH_HPP

#include "../Engine/TemporaryStorage.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>
#include <glm/glm.hpp>

class Sky;

class RenderCommand {

public:

	virtual ~RenderCommand() {};

	virtual void execute() = 0;

};

class ActiveFrameBufferCommand : public RenderCommand {

public:

	ActiveFrameBufferCommand(IFrameBuffer* fbo, bool clear);

	virtual ~ActiveFrameBufferCommand() {};

	virtual void execute();

private:
	IFrameBuffer * activeFbo;
	bool cls;
};

class ActiveShaderCommand : public RenderCommand {

public:

	ActiveShaderCommand(IShaderObject* shader);

	virtual ~ActiveShaderCommand() {};

	virtual void execute();

private:
	IShaderObject * shader;
};

class ViewProjectionCommand : public RenderCommand {

public:
	ViewProjectionCommand(IShaderObject* shader, int32_t viewProjectionLocation, glm::mat4* vpMatrix);

	virtual ~ViewProjectionCommand() {};

	virtual void execute();

private:

	glm::mat4* vpMat;
	int32_t vpLoc;
	IShaderObject* boundShader;
};

class SkyCommand : public RenderCommand {

public:
	SkyCommand(Sky* _sky, glm::mat4 _vpMat, glm::vec3 _camPos, glm::vec3 _camDir, glm::mat4* _reflect);

	virtual ~SkyCommand() {};

	virtual void execute();

public:

	Sky* sky;
	glm::mat4 vpMat;
	glm::vec3 camPos;
	glm::vec3 camDir;
	glm::mat4* reflect;
};

struct CameraSettings {

	glm::mat4 vpMat;
	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;

};

class RenderBatch {

public:

	RenderBatch(size_t maxCommands, TemporaryStorage* tempStorage);
	virtual ~RenderBatch() {};

	void addCommand(RenderCommand* command);

	void executeBatch();

	TemporaryStorage* getBatchTempStorage() const;

	void setCameraSettings(CameraSettings camSett);
	CameraSettings getCameraSettings();

private:

	CameraSettings cameraSettings;

	size_t numRenderCommands;
	size_t maxRenderCommands;

	RenderCommand** renderCommands;

	TemporaryStorage* tempStore;


};

#endif