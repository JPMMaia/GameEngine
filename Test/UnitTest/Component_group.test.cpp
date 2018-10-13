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


		Components_arrays_tuple<10, Position, Rotation> tuple{};
		auto& x = get<0>(tuple);
		x[0] = Position{ 0, 0, 0 };
		auto y = sizeof(tuple);
		auto size = x.size();
	}
}
