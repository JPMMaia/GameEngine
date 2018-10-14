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

	SCENARIO("Create a memory chunk, add, remove and set components", "[MemoryChunk]")
	{
		GIVEN("A memory chunk of Position and Rotation components")
		{
			Memory_chunk<Position, Rotation> chunk;

			THEN("The chunk has an initial size of 0")
			{
				CHECK(chunk.size() == 0);
			}

			WHEN("Pushing back components Position { 2.0f, 0.5f, -1.0f } and Rotation { 0.1f, 0.2f, 0.3f, 1.0f }")
			{
				Position original_position{ 2.0f, 0.5f, -1.0f };
				Rotation original_rotation{ 0.1f, 0.2f, 0.3f, 1.0f };

				auto const index = chunk.push_back(original_position, original_rotation);

				//constexpr auto number_of_component_types = 2;
				//chunk.get_components_data_impl({ 0 }, std::make_index_sequence<number_of_component_types>{});

				THEN("The returned index should be equals to 0")
				{
					CHECK(index.value == 0);
				}

				THEN("The size of the chunk should have been incremented by 1")
				{
					CHECK(chunk.size() == 1);
				}

				AND_WHEN("Getting components of element at index 0")
				{
					std::tuple<Position, Rotation> const components = chunk.get_components_data(index);
					Position const& current_position = std::get<0>(components);
					Rotation const& current_rotation = std::get<1>(components);

					THEN("The position and rotation components should have the same value that was pushed back")
					{
						CHECK(current_position.x == original_position.x);
						CHECK(current_position.y == original_position.y);
						CHECK(current_position.z == original_position.z);

						CHECK(current_rotation.a == original_rotation.a);
						CHECK(current_rotation.b == original_rotation.b);
						CHECK(current_rotation.c == original_rotation.c);
						CHECK(current_rotation.w == original_rotation.w);
					}
				}

				AND_WHEN("Setting element at index 0 given Position {- 1.0f, 2.0f, 3.0f } and Rotation { 0.5f, 0.5f, 0.0f, 0.5f }")
				{
					Position new_position{ -1.0f, 2.0f, 3.0f };
					Rotation new_rotation{ 0.5f, 0.5f, 0.0f, 0.5f };

					chunk.set_components_data(index, new_position, new_rotation);

					AND_WHEN("Getting components of element at index 0")
					{
						std::tuple<Position, Rotation> const components = chunk.get_components_data(index);
						Position const& current_position = std::get<0>(components);
						Rotation const& current_rotation = std::get<1>(components);

						THEN("The position and rotation components should have the same value that was set")
						{
							CHECK(current_position.x == new_position.x);
							CHECK(current_position.y == new_position.y);
							CHECK(current_position.z == new_position.z);

							CHECK(current_rotation.a == new_rotation.a);
							CHECK(current_rotation.b == new_rotation.b);
							CHECK(current_rotation.c == new_rotation.c);
							CHECK(current_rotation.w == new_rotation.w);
						}
					}
				}

				AND_WHEN("Popping back")
				{
					std::tuple<Position, Rotation> const components = chunk.pop_back();
					Position const& current_position = std::get<0>(components);
					Rotation const& current_rotation = std::get<1>(components);

					THEN("The position and rotation components should have the same value that was pushed back")
					{
						CHECK(current_position.x == original_position.x);
						CHECK(current_position.y == original_position.y);
						CHECK(current_position.z == original_position.z);

						CHECK(current_rotation.a == original_rotation.a);
						CHECK(current_rotation.b == original_rotation.b);
						CHECK(current_rotation.c == original_rotation.c);
						CHECK(current_rotation.w == original_rotation.w);
					}
				}
			}
		}
	}
}
