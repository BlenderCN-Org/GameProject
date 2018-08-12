#include "../pch.h"

#include <EngineCore/Core/System.hpp>


TEST(Engine, makeUint64_t_test) {

	uint32_t low = 0x00001234LU;
	uint32_t high = 0x00005678LU;

	uint64_t combined = Engine::System::makeUint64_t(high, low);

	if (combined != 0x0000567800001234LLU) {
		FAIL();
	}

}

TEST(Engine, makeUint64_t_test2) {

	uint32_t low  = 0x98765432LU;
	uint32_t high = 0x12345678LU;

	uint64_t combined = Engine::System::makeUint64_t(high, low);

	if (combined != 0x1234567898765432LLU) {
		FAIL();
	}

}