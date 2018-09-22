#include <cstdint>

#include <catch2/catch.hpp>

#include <Maia/GameEngine/Component_group_builder.hpp>
#include <Maia/GameEngine/Entity_manager.hpp>

namespace Maia::GameEngine::Test
{
	struct Position
	{
		static std::uint16_t ID()
		{
			return 0;
		}

		float x, y, z;
	};

	bool operator==(const Position& lhs, const Position& rhs)
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y
			&& lhs.z == rhs.z;
	}

	std::ostream& operator<<(std::ostream& outputStream, const Position& value)
	{
		outputStream << "{ " << value.x << ", " << value.y << ", " << value.z << " }";
		return outputStream;
	}

	SCENARIO("Create an entity constituted by a position and then destroy it")
	{
		GIVEN("A position")
		{
			const Position position{ -1.0f, 1.0f, 0.5f };

			AND_GIVEN("A component group <Position>")
			{
				auto component_group = []()-> Component_group
				{
					Component_group_builder builder;
					builder.add_component<Position>();
					return builder.build();
				}();

				WHEN("The entity is created")
				{
					Entity_manager entity_manager;
					const auto entity = entity_manager.create_entity(std::move(component_group));

					AND_WHEN("The entity's position data is set")
					{
						entity_manager.set_component_data(entity, position);

						AND_WHEN("The entity's position data is retrieved")
						{
							const auto queriedPosition = entity_manager.get_component_data<Position>(entity);

							THEN("They must match")
							{
								CHECK(position == queriedPosition);
							}
						}
					}

					AND_WHEN("The entity is destroyed")
					{
						entity_manager.destroy_entity(entity);

						THEN("An exception is thrown when trying to retrived data from the destroyed entity")
						{
							CHECK_THROWS_AS(
								entity_manager.get_component_data<Position>(entity),
								Entity_not_found
							);
						}
					}
				}
			}
		

			
			


		}
	}
}
