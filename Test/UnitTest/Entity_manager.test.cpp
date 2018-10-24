#include <cstdint>

#include <catch2/catch.hpp>

#include <Maia/GameEngine/Entity_manager.hpp>

namespace Maia::GameEngine::Test
{
	namespace
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
	}

	SCENARIO("Create an entity constituted by a position and then destroy it")
	{
		GIVEN("An entity manager")
		{
			Entity_manager entity_manager;

			AND_GIVEN("A position")
			{
				Position const position{ -1.0f, 1.0f, 0.5f };

				WHEN("The entity type is created")
				{
					Entity_type const entity_type = entity_manager.create_entity_type<Position>();

					AND_WHEN("The entity is created")
					{
						Entity const entity = entity_manager.create_entity(entity_type);

						AND_WHEN("The entity's position data is set")
						{
							entity_manager.set_component_data(entity, position);

							AND_WHEN("The entity's position data is retrieved")
							{
								Position const queried_position = entity_manager.get_component_data<Position>(entity);

								THEN("They must match")
								{
									CHECK(position == queried_position);
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

	SCENARIO("Create an entity, then add a component type, set values, get values and finally remove it")
	{
		GIVEN("An entity manager")
		{
			Entity_manager entity_manager;

			AND_GIVEN("An empty entity type")
			{
				Entity_type const entity_type = entity_manager.create_entity_type();

				WHEN("An entity is created")
				{
					Entity const entity = entity_manager.create_entity(entity_type);

					THEN("The entity manager should report that the entity hasn't any Position component associated with it")
					{
						CHECK(entity_manager.has_component<Position>() == false);
					}

					AND_WHEN("A component type is added to that entity")
					{
						Position const original_position { 1.0f, 2.0f, 3.0f };
						entity_manager.add_component<Position>(entity, original_position);

						THEN("The entity manager should report that the entity has a Position component associated with it")
						{
							CHECK(entity_manager.has_component<Position>());
						}

						THEN("The component data associated with the entity should have correspond to the given values")
						{
							Position const current_position = entity_manager.get_component_data<Position>(entity);

							CHECK(original_position == current_position);
						}

						AND_WHEN("The position component is removed")
						{
							entity_manager.remove_component<Position>(entity);

							THEN("The entity manager should report that the entity hasn't any Position component associated with it")
							{
								CHECK(entity_manager.has_component<Position>() == false);
							}
						}
					}
				}
			}
		}
	}
}
