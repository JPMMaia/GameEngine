#include <cstdint>

#include <catch2/catch.hpp>

#include <Test_components.hpp>

#include <Maia/GameEngine/Entity_manager.hpp>

namespace Maia::GameEngine::Test
{
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
					Entity_type<Position> const entity_type = entity_manager.create_entity_type<Position>(2);

					AND_WHEN("The entity is created with a given position data")
					{
						Entity const entity = entity_manager.create_entity(entity_type, position);

						THEN("The entity manager should report that the entity exists")
						{
							CHECK(entity_manager.exists(entity));
						}

						THEN("The entity manager should report that the entity has a position component")
						{
							CHECK(entity_manager.has_component<Position>(entity));
						}

						THEN("The entity manager should report that the entity doesn't have a rotation component")
						{
							CHECK(!entity_manager.has_component<Rotation>(entity));
						}

						AND_WHEN("The entity's position data is retrieved")
						{
							Position const queried_position = entity_manager.get_component_data<Position>(entity);

							THEN("They must match")
							{
								CHECK(position == queried_position);
							}
						}

						AND_WHEN("The entity is destroyed")
						{
							entity_manager.destroy_entity(entity);

							THEN("The entity manager should report that the entity does not exist")
							{
								CHECK(!entity_manager.exists(entity));
							}
						}
					}
				}
			}
		}
	}

	SCENARIO("Create several entities of different component types, and then iterate through a specific set of components")
	{
		GIVEN("An entity manager")
		{
			Entity_manager entity_manager;

			AND_GIVEN("Three different entity types: Position, Rotation and Position_rotation")
			{
				auto position_entity_type = entity_manager.create_entity_type<Position>(2);
				auto rotation_entity_type = entity_manager.create_entity_type<Rotation>(2);
				auto position_rotation_entity_type = entity_manager.create_entity_type<Position, Rotation>(2);

				WHEN("Three entities of each entity type are created")
				{
					std::array<Entity, 3> position_entities = entity_manager.create_entities<3>(position_entity_type, Position{});
					std::array<Entity, 3> rotation_entities = entity_manager.create_entities<3>(rotation_entity_type, Rotation{});
					std::array<Entity, 3> position_rotation_entities = entity_manager.create_entities<3>(position_rotation_entity_type, Position{}, Rotation{});

					std::array<Position, 6> positions
					{
						Position
						{ 1.0f, 2.0f, 3.0f },
						{ 4.0f, 5.0f, 6.0f },
						{ 7.0f, 8.0f, 9.0f },
						{ 10.0f, 11.0f, 12.0f },
						{ 13.0f, 14.0f, 15.0f },
						{ 16.0f, 17.0f, 18.0f },
					};

					std::unordered_map<Entity, Position> entity_position_map
					{
						{ position_entities[0], positions[0] },
						{ position_entities[1], positions[1] },
						{ position_entities[2], positions[2] },
						{ position_rotation_entities[0], positions[3] },
						{ position_rotation_entities[1], positions[4] },
						{ position_rotation_entities[2], positions[5] },
					};

					for (auto[entity, position] : entity_position_map)
						entity_manager.set_component_data(entity, position);


					std::array<Rotation, 6> rotations
					{
						Rotation
						{ 19.0f, 20.0f, 21.0f, 22.0f },
						{ 23.0f, 24.0f, 25.0f, 26.0f },
						{ 27.0f, 28.0f, 29.0f, 30.0f },
						{ 31.0f, 32.0f, 33.0f, 34.0f },
						{ 35.0f, 36.0f, 37.0f, 38.0f },
						{ 39.0f, 40.0f, 41.0f, 42.0f },
					};

					std::unordered_map<Entity, Rotation> entity_rotation_map
					{
						{ rotation_entities[0], rotations[0] },
						{ rotation_entities[1], rotations[1] },
						{ rotation_entities[2], rotations[2] },
						{ position_rotation_entities[0], rotations[3] },
						{ position_rotation_entities[1], rotations[4] },
						{ position_rotation_entities[2], rotations[5] },
					};

					for (auto[entity, rotation] : entity_rotation_map)
						entity_manager.set_component_data(entity, rotation);


					THEN("It should be possible to iterate through all the Position components")
					{
						gsl::span<Component_types_group> component_types_group = entity_manager.get_component_types_groups();
						gsl::span<Component_group> component_groups = entity_manager.get_component_groups();

						std::size_t count{ 0 };

						for (std::ptrdiff_t component_group_index = 0; component_group_index < component_types_group.size(); ++component_group_index)
						{
							Component_types_group types = component_types_group[component_group_index];

							if (types.contains<Position>())
							{
								Component_group const& component_group = component_groups[component_group_index];

								for (std::size_t chunk_index = 0; chunk_index < component_group.num_chunks(); ++chunk_index)
								{
									gsl::span<const Position> positions = component_group.components<Position>(chunk_index);
									gsl::span<const Entity> entities = component_group.components<Entity>(chunk_index);

									for (std::ptrdiff_t component_index = 0; component_index < entities.size(); ++component_index)
									{
										auto location = entity_position_map.find(entities[component_index]);
										REQUIRE(location != entity_position_map.end());

										Position const& position = location->second;
										CHECK(position == positions[component_index]);

										++count;
									}
								}
							}
						}

						CHECK(count == position_entities.size() + position_rotation_entities.size());
					}

					THEN("It should be possible to iterate through all the Rotation components of entities that do not contain a Position")
					{
						gsl::span<Component_types_group> component_types_group = entity_manager.get_component_types_groups();
						gsl::span<Component_group> component_groups = entity_manager.get_component_groups();

						std::size_t count{ 0 };

						for (std::ptrdiff_t component_group_index = 0; component_group_index < component_types_group.size(); ++component_group_index)
						{
							Component_types_group types = component_types_group[component_group_index];

							if (types.contains<Rotation>() && !types.contains<Position>())
							{
								Component_group const& component_group = component_groups[component_group_index];

								for (std::size_t chunk_index = 0; chunk_index < component_group.num_chunks(); ++chunk_index)
								{
									gsl::span<const Rotation> rotations = component_group.components<Rotation>(chunk_index);
									gsl::span<const Entity> entities = component_group.components<Entity>(chunk_index);

									for (std::ptrdiff_t component_index = 0; component_index < entities.size(); ++component_index)
									{
										auto location = entity_rotation_map.find(entities[component_index]);
										REQUIRE(location != entity_rotation_map.end());

										Rotation const& rotation = location->second;
										CHECK(rotation == rotations[component_index]);

										++count;
									}
								}
							}
						}

						CHECK(count == rotation_entities.size());
					}

					THEN("It should be possible to iterate through all the Position and Rotation components of supported entities")
					{
						gsl::span<Component_types_group> component_types_group = entity_manager.get_component_types_groups();
						gsl::span<Component_group> component_groups = entity_manager.get_component_groups();

						std::size_t count{ 0 };

						for (std::ptrdiff_t component_group_index = 0; component_group_index < component_types_group.size(); ++component_group_index)
						{
							Component_types_group types = component_types_group[component_group_index];

							if (types.contains<Position, Rotation>())
							{
								Component_group const& component_group = component_groups[component_group_index];

								for (std::size_t chunk_index = 0; chunk_index < component_group.num_chunks(); ++chunk_index)
								{
									gsl::span<const Position> positions = component_group.components<Position>(chunk_index);
									gsl::span<const Rotation> rotations = component_group.components<Rotation>(chunk_index);
									gsl::span<const Entity> entities = component_group.components<Entity>(chunk_index);

									for (std::ptrdiff_t component_index = 0; component_index < entities.size(); ++component_index)
									{
										{
											auto location = entity_position_map.find(entities[component_index]);
											REQUIRE(location != entity_position_map.end());

											Position const& position = location->second;
											CHECK(position == positions[component_index]);
										}

										{
											auto location = entity_rotation_map.find(entities[component_index]);
											REQUIRE(location != entity_rotation_map.end());

											Rotation const& rotation = location->second;
											CHECK(rotation == rotations[component_index]);
										}

										++count;
									}
								}
							}
						}

						CHECK(count == position_rotation_entities.size());
					}
				}
			}
		}
	}

	SCENARIO("Create entites, remove and then reuse them")
	{
		GIVEN("An entity manager")
		{
			Entity_manager entity_manager;

			WHEN("Nine entities of Position entity type are created")
			{
				auto position_entity_type = entity_manager.create_entity_type<Position>(2);

				std::array<Entity, 3> position_entities_group_1
				{
					entity_manager.create_entity(position_entity_type, Position{}),
					entity_manager.create_entity(position_entity_type, Position{}),
					entity_manager.create_entity(position_entity_type, Position{})
				};

				std::array<Entity, 3> position_entities_group_2 = entity_manager.create_entities<3>(position_entity_type, Position{});
				std::vector<Entity> position_entities_group_3 = entity_manager.create_entities(3, position_entity_type, Position{});

				std::vector<Entity> all_position_entities;
				all_position_entities.reserve(position_entities_group_1.size() + position_entities_group_2.size() + position_entities_group_3.size());
				all_position_entities.insert(all_position_entities.end(), position_entities_group_1.begin(), position_entities_group_1.end());
				all_position_entities.insert(all_position_entities.end(), position_entities_group_2.begin(), position_entities_group_2.end());
				all_position_entities.insert(all_position_entities.end(), position_entities_group_3.begin(), position_entities_group_3.end());

				THEN("The entity manager should report that entities exist")
				{
					for (Entity const entity : all_position_entities)
					{
						CHECK(entity_manager.exists(entity));
					}
				}

				THEN("The created entities must have a Position component")
				{
					for (Entity const entity : all_position_entities)
					{
						CHECK(entity_manager.has_component<Position>(entity));
					}
				}

				AND_WHEN("Entities are destroyed")
				{
					for (Entity const entity : all_position_entities)
					{
						entity_manager.destroy_entity(entity);
					}

					THEN("The entity manager should report that entities don't exist")
					{
						for (Entity const entity : all_position_entities)
						{
							CHECK(!entity_manager.exists(entity));
						}
					}

					THEN("The component group of positions should be empty")
					{
						gsl::span<Component_types_group> component_types_group = entity_manager.get_component_types_groups();
						gsl::span<Component_group> component_groups = entity_manager.get_component_groups();

						for (std::ptrdiff_t component_group_index = 0; component_group_index < component_types_group.size(); ++component_group_index)
						{
							Component_types_group types = component_types_group[component_group_index];

							if (types.contains<Position>())
							{
								Component_group const& component_group = component_groups[component_group_index];

								CHECK(component_group.size() == 0);
							}
						}
					}

					AND_WHEN("Nine more entities are created, this time with a Rotation component")
					{
						auto rotation_entity_type = entity_manager.create_entity_type<Rotation>(2);

						std::array<Entity, 3> rotation_entities_group_1
						{
							entity_manager.create_entity(rotation_entity_type, Rotation{}),
							entity_manager.create_entity(rotation_entity_type, Rotation{}),
							entity_manager.create_entity(rotation_entity_type, Rotation{})
						};

						std::array<Entity, 3> rotation_entities_group_2 = entity_manager.create_entities<3>(rotation_entity_type, Rotation{});
						std::vector<Entity> rotation_entities_group_3 = entity_manager.create_entities(3, rotation_entity_type, Rotation{});

						std::vector<Entity> all_rotation_entities;
						all_rotation_entities.reserve(rotation_entities_group_1.size() + rotation_entities_group_2.size() + rotation_entities_group_3.size());
						all_rotation_entities.insert(all_rotation_entities.end(), rotation_entities_group_1.begin(), rotation_entities_group_1.end());
						all_rotation_entities.insert(all_rotation_entities.end(), rotation_entities_group_2.begin(), rotation_entities_group_2.end());
						all_rotation_entities.insert(all_rotation_entities.end(), rotation_entities_group_3.begin(), rotation_entities_group_3.end());

						THEN("The created entities should have the same entity values as the previously destroyed ones")
						{
							auto by_entity_value = [](Entity lhs, Entity rhs) -> bool { return lhs.value < rhs.value; };

							std::sort(all_position_entities.begin(), all_position_entities.end(), by_entity_value);
							std::sort(all_rotation_entities.begin(), all_rotation_entities.end(), by_entity_value);

							CHECK(std::equal(all_position_entities.begin(), all_position_entities.end(), all_rotation_entities.begin()));
						}

						THEN("The entity manager should report that the entities exist")
						{
							for (Entity const entity : all_rotation_entities)
							{
								CHECK(entity_manager.exists(entity));
							}
						}

						THEN("The component group of rotations should have size equals 9")
						{
							gsl::span<Component_types_group> component_types_group = entity_manager.get_component_types_groups();
							gsl::span<Component_group> component_groups = entity_manager.get_component_groups();

							for (std::ptrdiff_t component_group_index = 0; component_group_index < component_types_group.size(); ++component_group_index)
							{
								Component_types_group types = component_types_group[component_group_index];

								if (types.contains<Rotation>())
								{
									Component_group const& component_group = component_groups[component_group_index];

									CHECK(component_group.size() == all_rotation_entities.size());
								}
							}
						}

						THEN("The recreated entities must not have a Position component")
						{
							for (Entity const entity : all_rotation_entities)
							{
								CHECK(!entity_manager.has_component<Position>(entity));
							}
						}

						THEN("The recreated entities must have a Rotation component")
						{
							for (Entity const entity : all_rotation_entities)
							{
								CHECK(entity_manager.has_component<Rotation>(entity));
							}
						}
					}
				}
			}
		}
	}

	SCENARIO("Create three entities and set different positions. Then destroy the first and check if the other entities are still valid")
	{
		GIVEN("An entity manager")
		{
			Entity_manager entity_manager;

			WHEN("Three entities of Position entity type are created")
			{
				auto const position_entity_type = entity_manager.create_entity_type<Position>(2);

				std::array<Entity, 3> entities = entity_manager.create_entities<3>(position_entity_type, Position{});

				std::array<Position, 3> const positions
				{
					Position{ 1.0f, 2.0f, 3.0f },
					Position{ 4.0f, 5.0f, 6.0f },
					Position{ 7.0f, 8.0f, 9.0f },
				};

				for (std::size_t entity_index = 0; entity_index < entities.size(); ++entity_index)
				{
					entity_manager.set_component_data(entities[entity_index], positions[entity_index]);
				}

				THEN("The set component data should correspond to the get component data")
				{
					for (std::size_t entity_index = 0; entity_index < entities.size(); ++entity_index)
					{
						Position const position = entity_manager.get_component_data<Position>(entities[entity_index]);
						CHECK(position == positions[entity_index]);
					}
				}

				AND_WHEN("The first entity is destroyed")
				{
					entity_manager.destroy_entity(entities[0]);

					THEN("The entity manager should report that the first entity does not exist")
					{
						CHECK(!entity_manager.exists(entities[0]));
					}

					AND_WHEN("A new entity is created")
					{
						entities[0] = entity_manager.create_entity(position_entity_type, positions[0]);

						THEN("The entities component data should be valid")
						{
							for (std::size_t entity_index = 0; entity_index < entities.size(); ++entity_index)
							{
								Position const position = entity_manager.get_component_data<Position>(entities[entity_index]);
								CHECK(position == positions[entity_index]);
							}
						}
					}
				}
			}
		}
	}

	/*SCENARIO("Create an entity, then add a component type, set values, get values and finally remove it")
	{
		GIVEN("An entity manager")
		{
			Entity_manager entity_manager;

			AND_GIVEN("An empty entity type")
			{
				Entity_type<> const entity_type = entity_manager.create_entity_type(2);

				WHEN("An entity is created")
				{
					Entity const entity = entity_manager.create_entity(entity_type);

					THEN("The entity manager should report that the entity hasn't any Position component associated with it")
					{
						CHECK(entity_manager.has_component<Position>(entity) == false);
					}

					AND_WHEN("A component type is added to that entity")
					{
						Position const original_position{ 1.0f, 2.0f, 3.0f };
						entity_manager.add_component<Position>(entity, original_position);

						THEN("The entity manager should report that the entity has a Position component associated with it")
						{
							CHECK(entity_manager.has_component<Position>(entity));
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
								CHECK(entity_manager.has_component<Position>(entity) == false);
							}
						}
					}
				}
			}
		}
	}*/
}
