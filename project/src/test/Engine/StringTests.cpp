#include "../pch.h"

#include <EngineCore/Core/String.hpp>


TEST(Engine, StringCompareTest) {

	Engine::Core::String str = "Init";

	const char* cStr = str.cStr();

	if (strcmp("Init", cStr) != 0) {
		FAIL();
	}

}

TEST(Engine, StringContainsText) {

	Engine::Core::String str = "This is a string";

	Engine::Core::String cmpStr = "This is a string";
	if (false == str.contains(cmpStr)) {
		FAIL();
	}

	Engine::Core::String cmpStr2 = "is";
	if (false == str.contains(cmpStr2)) {
		FAIL();
	}

	Engine::Core::String cmpStr3 = "string";
	if (false == str.contains(cmpStr3)) {
		FAIL();
	}

	Engine::Core::String cmpStr4 = "a";
	if (false == str.contains(cmpStr4)) {
		FAIL();
	}
	
	str = "aadauthhelper.dll";
	Engine::Core::String cmpStr5 = "helper";
	if (false == str.contains(cmpStr5)) {
		FAIL();
	}

}

TEST(Engine, StringContainsText_cStr) {

	Engine::Core::String str = "This is a string";

	if (false == str.contains("This is a string")) {
		FAIL();
	}

	if (false == str.contains("is")) {
		FAIL();
	}

	if (false == str.contains("string")) {
		FAIL();
	}

	if (false == str.contains("a")) {
		FAIL();
	}

	str = "aadauthhelper.dll";
	if (false == str.contains("helper")) {
		FAIL();
	}
}