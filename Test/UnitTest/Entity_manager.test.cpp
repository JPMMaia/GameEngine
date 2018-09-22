#include <cstdint>

#include <catch2/catch.hpp>

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
		GIVEN("An entity manager")
		{
			Entity_manager entity_manager;

			AND_GIVEN("A position")
			{
				const Position position{ -1.0f, 1.0f, 0.5f };

				WHEN("The entity type is created")
				{
					const auto entity_type = entity_manager.create_entity_type<Position>();

					AND_WHEN("The entity is created")
					{
						const auto entity = entity_manager.create_entity(entity_type);

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
}
