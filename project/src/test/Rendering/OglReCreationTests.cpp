#include "../pch.h"

#include <RenderEngine/RenderEngine.hpp>

#if _DEBUG

TEST(CreationTests, opengl) {
	
	_CrtMemState mStateBeg;
	_CrtMemState mStateEnd;
	_CrtMemState mStateDiff;
	
	ZeroMemory(&mStateBeg, sizeof(_CrtMemState));
	ZeroMemory(&mStateEnd, sizeof(_CrtMemState));
	ZeroMemory(&mStateDiff, sizeof(_CrtMemState));
	
	_CrtMemCheckpoint(&mStateBeg);
	
	IRenderEngine* re = CreateRenderEngine();
	
	RenderEngineCreateInfo reci;
	
	reci.stype = SType::sRenderEngineCreateInfo;
	reci.createRenderWindow = false;
	reci.renderEngineType = RenderEngineType::eRenderOpenGL;
	reci.pNext = nullptr;
	
	try {
	
		re->init(reci);
	
	} catch (const std::exception&) {
		FAIL();
	}
	
	re->release();
	
	_CrtMemCheckpoint(&mStateEnd);
	int found = _CrtMemDifference(&mStateDiff, &mStateBeg, &mStateEnd);
	
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	tmpFlag &= ~_CRTDBG_LEAK_CHECK_DF;
	
	_CrtSetDbgFlag(tmpFlag);

	EXPECT_EQ(found, FALSE);
}

TEST(CreationTests, opengl_do_leak_memory) {

	_CrtMemState mStateBeg;
	_CrtMemState mStateEnd;
	_CrtMemState mStateDiff;

	ZeroMemory(&mStateBeg, sizeof(_CrtMemState));
	ZeroMemory(&mStateEnd, sizeof(_CrtMemState));
	ZeroMemory(&mStateDiff, sizeof(_CrtMemState));

	_CrtMemCheckpoint(&mStateBeg);

	IRenderEngine* re = CreateRenderEngine();

	RenderEngineCreateInfo reci;

	reci.stype = SType::sRenderEngineCreateInfo;
	reci.createRenderWindow = false;
	reci.renderEngineType = RenderEngineType::eRenderOpenGL;
	reci.pNext = nullptr;

	try {

		re->init(reci);

	} catch (const std::exception&) {
		FAIL();
	}

	// this is intentional
	//re->release();

	_CrtMemCheckpoint(&mStateEnd);
	int found = _CrtMemDifference(&mStateDiff, &mStateBeg, &mStateEnd);

	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	tmpFlag &= ~_CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(tmpFlag);

	EXPECT_EQ(found, TRUE);
}

#endif