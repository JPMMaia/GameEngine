#include <catch2/catch.hpp>

#include <Test_components.hpp>

#include <Maia/GameEngine/Memory_chunk.hpp>

namespace Maia::GameEngine::Test
{
	SCENARIO("Create a memory chunk, add, remove and set components", "[Memory_chunk]")
	{
		GIVEN("A memory chunk with 28 bytes of capacity")
		{
			Memory_chunk<28> chunk{};

			THEN("The chunk has capacity of 28")
			{
				CHECK(chunk.capacity() == 28);
			}

			WHEN("Setting component Position { 1.0f, 2.0f, 3.0f } at index 0 and Rotation { 4.0f, 5.0f, 6.0f, 7.0f } at index 12")
			{
				Position original_position{ 1.0f, 2.0f, 3.0f };
				chunk.set_data({ 0 }, original_position);

				Rotation original_rotation{ 4.0f, 5.0f, 6.0f, 7.0f };
				chunk.set_data({ 12 }, original_rotation);

				THEN("The Position at index 0 and Rotation at index 12 must match")
				{
					Position const current_position = chunk.get_data<Position>({ 0 });
					CHECK(original_position == current_position);

					Rotation const current_rotation = chunk.get_data<Rotation>({ 12 });
					CHECK(original_rotation == current_rotation);
				}
			}
		}
	}
}
