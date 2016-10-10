#include "Core.hpp"

#include <glm\gtc\matrix_transform.hpp>

#include <thread>

char* vs =
"#version 410\n"
"\n"
"layout(location = 0) in vec3 vertexPos;\n"
"layout(location = 1) in vec2 vertexUv;\n"
"\n"
"layout(location = 0) out vec2 fragUv;\n"
"\n"
"uniform mat4 viewProjMatrix;\n"
"uniform mat4 worldMat;\n"
"\n"
"void main()\n"
"{\n"
"	fragUv = vertexUv;\n"
"	gl_Position = viewProjMatrix * vec4((vec4(vertexPos, 1.0f) * worldMat).xyz, 1.0f);\n"
"}\n";

char* fs =
"#version 410\n"
"\n"
"layout(location = 0) in vec2 fragUv;\n"
"\n"
"layout(location = 0) out vec4 fragmentColor;\n"
"\n"
"layout(location = 1) out vec4 red;\n"
"uniform sampler2D textureIN;\n"
"void main()\n"
"{\n"
"	vec4 color = texture(textureIN, vec2(fragUv.x, fragUv.y));\n"
//"	color.r = 1.0;"
//"	color.g = 1.0;"
//"	color.b = 1.0;"
//"	color.a = 1.0;"
"	fragmentColor = color; //vec4(fragUv.x, fragUv.y, 0, 1);\n"
"	red = vec4(1.0, 0.0, 0, 1);\n"
"}\n";

void thread(ITexture* tex, bool* kill )
{
	while ( !kill )
	{
		unsigned char t[16 * 16];

		for ( size_t i = 0; i < 16; i++ )
		{
			for ( size_t j = 0; j < 16; j++ )
			{
				t[(i * 16) + j] = (unsigned char)(((float)(i*j) / (16.0f*16.0f)) * 256.0f);
			}
		}

		tex->setTextureData(16, 16, 1, t);
	}
}

void Core::init()
{
	hadReset = false;
	if ( renderEngineLib.loadLibrary("RenderEngine.dll") )
		printf("Loaded\n");

	CreateRenderEngineProc rProc = (CreateRenderEngineProc)renderEngineLib.getProcAddress("CreateRenderEngine");

	disp.setResolution(1280, 720);
	disp.setVsyncMode(VSyncMode::VSYNC_ON);
	disp.setFramerateLock(FramerateLock::FRAME_LOCK_NONE);
	disp.setFullscreenMode(FullscreenMode::WINDOWED);

	//renderEngine = CreateRenderEngine();
	renderEngine = rProc();
	renderEngine->init();
	renderEngine->updateViewPort(1280, 720);

	disp.setRenderEngine(renderEngine);

	input = Input::getInput();
	console = new Console();

	console->dispSettings = &disp;

	camera = renderEngine->createCamera();

	shader = renderEngine->createShaderObject();
	shader->init();

	shader->setShaderCode(ShaderStages::VERTEX_STAGE, vs);
	shader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs);

	shader->buildShader();

	vpLoc = shader->getShaderUniform("viewProjMatrix");
	mdlLoc = shader->getShaderUniform("worldMat");
	texLoc = shader->getShaderUniform("textureIN");

	camInput.init((glm::mat4*)camera->getViewMatrix());

	*(glm::mat4*)(camera->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(1280), float(720), 0.0001f, 100.0f);

	input->attachConsole(console);

	planeMesh = renderEngine->createMesh();
	triangleMesh = renderEngine->createMesh();
	planeMesh->init();
	triangleMesh->init();

	float f[] = { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f };

	planeMesh->setMeshData(f, sizeof(f), VERT_UV);
	map = planeMesh->map(mapSize);
	triangleMesh->setMeshData(f, sizeof(f) - sizeof(float) * 5, VERT_UV);

	text = renderEngine->createText();
	font = renderEngine->createFont();

	font->init("C:/Windows/Fonts/Arial.ttf", 24);
	text->init();
	text->setFont(font);

	fbo = renderEngine->createFrameBuffer();

	fbo->init();

	fbo->setupFrameBuffer(1280, 720, 1, true, false, true);

	texture = renderEngine->createTexture();
	texture->init(1, false);
	toggle = true;
	unsigned char t[16 * 16];

	for ( size_t i = 0; i < 16; i++ )
	{
		for ( size_t j = 0; j < 16; j++ )
		{
			t[(i*16) + j] = (unsigned char) (((float)(i*j)/(16.0f*16.0f)) * 256.0f);
		}
	}

	texture->setTextureData(16, 16, 1, t);
	
	killThread = false;

	streamingThread = new std::thread(thread, texture, &killThread);

	game = new Game();
	game->init();

	mousePosText = renderEngine->createText();
	mousePosText->init();
	mousePosText->setFont(font);

}

void Core::release()
{
	disp.setWindow(nullptr);
	disp.setRenderEngine(nullptr);

	mousePosText->release();

	killThread = true;
	streamingThread->join();
	delete streamingThread;
	delete game;
	planeMesh->unmap();
	delete console;

	fbo->release();

	shader->release();
	camera->release();
	
	planeMesh->release();
	triangleMesh->release();
	
	font->release();
	text->release();
	
	texture->release();

	renderEngine->release();

	renderEngineLib.unloadLibrary();
	delete this;
}

bool Core::hadGraphicsReset() const
{
	if ( console->reset )
	{
		console->reset = false;
		return true;
	}
	return renderEngine->getGraphicsReset();
}

void Core::setFPS(int _fps)
{
	fps = _fps;
}

void Core::update(float dt)
{
	camInput.update(dt);

	game->update(dt);

	if (input->consoleKeyWasPressed())
	{
		input->toggleConsole();
		console->setVisible(input->consoleIsActive());
		
	}

	glm::mat4 vp = *(glm::mat4*)camera->getPerspectiveMatrix() * *(glm::mat4*)camera->getViewMatrix();

	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;

	shader->useShader();
	shader->bindData(vpLoc, UniformDataType::UNI_MATRIX4X4, &vp);
	shader->bindData(mdlLoc, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
		
	KeyBind kb;
	kb.code = 2;
	kb.mod = 0;
	kb.mouse = false;

	KeyBind kb2;
	kb2.code = 3;
	kb2.mod = 0;
	kb2.mouse = false;
	if ( input->isKeyBindPressed(kb) && !input->consoleIsActive() )
	{
		float* data = (float*)map;
		data[0] += 1.0f;
		planeMesh->flush();
	}

	if ( input->isKeyBindPressed(kb2) && !input->consoleIsActive() )
	{
		float* data = (float*)map;
		data[0] -= 1.0f;
		planeMesh->flush();
	}

	if (input->sizeChange)
	{
		int w = 0;
		int h = 0;
		input->getWindowSize(w, h);

		Resolution res = disp.getResolution();
		renderEngine->updateViewPort(res.width, res.height);
		fbo->resize(res.width, res.height);
		fbo->setWindowSize(w, h);
		if ( w != 0 && h != 0 )
		{
			*(glm::mat4*)(camera->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(res.width), float(res.height), 0.0001f, 100.0f);
		}
		//console->print("Resize\n");
	}
	//input->print();
	input->reset();
}

void Core::render()
{
	fbo->bind();
	renderEngine->renderDebugFrame();
	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;
	//obj->render();

	planeMesh->bind();

	texture->setCompressed(toggle);
	texture->bind();
	
	int id = 0;
	shader->bindData(texLoc, UniformDataType::UNI_INT, &id);

	planeMesh->render();
	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;

	size_t used = renderEngine->getMemoryUsage();
	size_t max = renderEngine->getMaxMemory();

	float percent = (float(used) / float(max)) * 100.0f;
	std::string str = "Memory: " + std::to_string(used) + "/" + std::to_string(max) + "(" + std::to_string(percent) + "%)\n\n\nSome more text\nUsing renderbuffers: " + std::to_string(fbo->isUsingRenderBuffer());
	str += "\n FPS: " +std::to_string(fps);
	str += "\n c: " + std::to_string(c);
	//text->render(std::string("a"), 25.0f, 100.0f, 45.0f, glm::vec3(0.5, 0.8f, 0.2f));

	if (input->consoleIsActive())
	{
		text->setText((char*)console->getText().c_str(), console->getText().length(), 25.0f, 25.0f, 1.0f);

		text->render(console->getText(), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		// render console
	}
	
	text->setText((char*)str.c_str(), str.length(), 25.0f, 600.0f, 1.0f);
	text->render(str, 25.0f, 600.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	
	int x, y;
	int mb, sc;
	input->getState(x, y, mb, sc);

	str = "Mouse pos (" + std::to_string(x) + "," + std::to_string(y) + ")";

	mousePosText->setText((char*)str.c_str(), str.length(), 25.0f, 1000.0f, 1.0f);
	mousePosText->render(str, 25.0f, 10.0f, 1.0f, glm::vec3(0.8, 0.43f, 0.0f));

	fbo->resolveToScreen();
	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;

	if ( c > 1000 )
	{
		toggle = !toggle;
		c = 0;
	}
	c++;
}

DisplaySettings * Core::getDisplaySettings()
{
	return &disp;
}
