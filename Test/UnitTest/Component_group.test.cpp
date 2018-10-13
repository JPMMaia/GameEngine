#include <catch2/catch.hpp>

#include <Maia/GameEngine/Memory_chunk.hpp>

namespace Maia::GameEngine::Test
{
	struct Position
	{
		float x, y, z;
	};
	struct Rotation
	{
		float a, b, c, w;
	};

	SCENARIO("Test")
	{
		Memory_chunk<Position, Rotation> chunk;
	}
}
