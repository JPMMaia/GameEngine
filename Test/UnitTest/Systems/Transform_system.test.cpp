#include <catch2/catch.hpp>

#include <Maia/GameEngine/Systems/Transform_system.hpp>

namespace Maia::GameEngine::Systems::Test
{
	SCENARIO("Create transforms")
	{
		GIVEN("Position = {} and Rotation = {}")
		{
			Position const position{};
			Rotation const rotation{};

			THEN("The corresponding transform matrix should be equal to the identity matrix")
			{
				Transform_matrix const transform = create_transform(position, rotation);

				CHECK(transform.value.isApprox(Eigen::Matrix4f::Identity(), 0.0f));
			}
		}

		GIVEN("Position = { 1.0f, 2.0f, 3.0f }, Rotation = { .w = 0.0f, .i = 1.0f, .j = 0.0f, .k = 0.0f }")
		{
			Position const position{ { 1.0f, 2.0f, 3.0f } };
			Rotation const rotation{ { 0.0f, 1.0f, 0.0f, 0.0f } };

			THEN("The corresponding transform matrix should be equal to the identity matrix")
			{
				Transform_matrix const transform = create_transform(position, rotation);

				Eigen::Matrix4f expected_matrix;
				expected_matrix <<
					1.0f, 0.0f, 0.0f, 1.0f,
					0.0f, -1.0f, 0.0f, 2.0f,
					0.0f, 0.0f, -1.0f, 3.0f,
					0.0f, 0.0f, 0.0f, 1.0f;

				CHECK(transform.value.isApprox(expected_matrix, 0.0f));
			}
		}

		GIVEN("Position = { 1.0f, 2.0f, 3.0f }, Rotation = { .w = 0.0f, .i = 0.0f, .j = 1.0f, .k = 0.0f }")
		{
			Position const position{ { 1.0f, 2.0f, 3.0f } };
			Rotation const rotation{ { 0.0f, 0.0f, 1.0f, 0.0f } };

			THEN("The corresponding transform matrix should be equal to the identity matrix")
			{
				Transform_matrix const transform = create_transform(position, rotation);

				Eigen::Matrix4f expected_matrix;
				expected_matrix <<
					-1.0f, 0.0f, 0.0f, 1.0f,
					0.0f, 1.0f, 0.0f, 2.0f,
					0.0f, 0.0f, -1.0f, 3.0f,
					0.0f, 0.0f, 0.0f, 1.0f;

				CHECK(transform.value.isApprox(expected_matrix, 0.0f));
			}
		}

		GIVEN("Position = { 1.0f, 2.0f, 3.0f }, Rotation = { .w = 0.0f, .i = 0.0f, .j = 1.0f, .k = 0.0f }")
		{
			Position const position{ { 1.0f, 2.0f, 3.0f } };

			float const angle = std::acos(-1.0f) / 4.0f;
			Rotation const rotation{ { std::cos(angle), 0.0f, 0.0f, std::sin(angle) } };

			THEN("The corresponding transform matrix should be equal to the identity matrix")
			{
				Transform_matrix const transform = create_transform(position, rotation);

				Eigen::Matrix4f expected_matrix;
				expected_matrix <<
					0.0f, -1.0f, 0.0f, 1.0f,
					1.0f, 0.0f, 0.0f, 2.0f,
					0.0f, 0.0f, 1.0f, 3.0f,
					0.0f, 0.0f, 0.0f, 1.0f;

				CHECK(transform.value.isApprox(expected_matrix));
			}
		}
	}

	SCENARIO("Create transform trees")
	{
		GIVEN("An entity manager")
		{
			Entity_manager entity_manager{};

			AND_GIVEN("A Root_transform_entity_type <Position, Rotation, Transform_matrix>")
			{
				auto const root_transform_entity_type =
					entity_manager.create_entity_type<Position, Rotation, Transform_matrix>(2);

				AND_GIVEN("Several root transform entities")
				{
					std::array<Entity, 4> root_transform_entities =
						entity_manager.create_entities<4>(root_transform_entity_type, Position{}, Rotation{}, Transform_matrix{});

					WHEN("Creating transform tree for a given root transform entity")
					{
						Entity const root_transform_entity = root_transform_entities[1];

						Transforms_tree const transform_tree =
							create_transforms_tree(entity_manager, root_transform_entity);

						THEN("The transform tree should have one entity without any children (empty transform_tree)")
						{
							CHECK(transform_tree.empty());
						}
					}

					AND_GIVEN("A Child_transform_entity_type <Position, Rotation, Transform_matrix, Transform_root, Transform_parent>")
					{
						auto const child_transform_entity_type =
							entity_manager.create_entity_type<Position, Rotation, Transform_matrix, Transform_root, Transform_parent>(2);

						AND_GIVEN("Several child transform entities")
						{
							Entity const root_transform_entity = root_transform_entities[2];

							Entity const first_child =
								entity_manager.create_entity(
									child_transform_entity_type,
									Position{}, Rotation{}, Transform_matrix{},
									Transform_root{ root_transform_entity },
									Transform_parent{ root_transform_entity }
							);

							Entity const first_child_child =
								entity_manager.create_entity(
									child_transform_entity_type,
									Position{}, Rotation{}, Transform_matrix{},
									Transform_root{ root_transform_entity },
									Transform_parent{ first_child }
							);

							Entity const second_child =
								entity_manager.create_entity(
									child_transform_entity_type,
									Position{}, Rotation{}, Transform_matrix{},
									Transform_root{ root_transform_entity },
									Transform_parent{ root_transform_entity }
							);

							Entity const second_child_child =
								entity_manager.create_entity(
									child_transform_entity_type,
									Position{}, Rotation{}, Transform_matrix{},
									Transform_root{ root_transform_entity },
									Transform_parent{ second_child }
							);

							WHEN("Creating transform tree for the root transform with childs")
							{
								Transforms_tree const transform_tree =
									create_transforms_tree(entity_manager, root_transform_entity);

								THEN("The transform tree should have the root transform with the two children")
								{
									std::deque<Entity> expected_children
									{
										first_child,
										second_child
									};

									auto const range = transform_tree.equal_range(Transform_parent{ root_transform_entity });
									REQUIRE(std::distance(range.first, range.second) == expected_children.size());

									for (auto i = range.first; i != range.second; ++i)
									{
										auto const location =
											std::find(expected_children.begin(), expected_children.end(), i->second);

										REQUIRE(location != expected_children.end());
										expected_children.erase(location);
									}

									CHECK(expected_children.empty());
								}

								THEN("The transform tree should have the first child and its respective child")
								{
									auto const range = transform_tree.equal_range(Transform_parent{ first_child });
									REQUIRE(std::distance(range.first, range.second) == 1);

									CHECK(range.first->second == first_child_child);
								}

								THEN("The transform tree should have the second child and its respective child")
								{
									auto const range = transform_tree.equal_range(Transform_parent{ second_child });
									REQUIRE(std::distance(range.first, range.second) == 1);

									CHECK(range.first->second == second_child_child);
								}
							}
						}
					}
				}
			}
		}
	}

	SCENARIO("Create a transform tree and update transforms")
	{
		GIVEN("An entity manager")
		{
			Entity_manager entity_manager{};

			AND_GIVEN("A root transform entity and 5 child transform entities")
			{
				float const pi = std::acos(-1.0f);

				auto const root_transform_entity_type = entity_manager.create_entity_type<Position, Rotation, Transform_matrix>(1);

				Entity const root_transform_entity = entity_manager.create_entity(
					root_transform_entity_type,
					Position{ { 1.0f, 2.0f, 3.0f } },
					Rotation{ { std::cos(pi / 4.0f), 0.0f, std::sin(pi / 4.0f), 0.0f } },
					Transform_matrix{}
				);


				auto const child_transform_entity_type = entity_manager.create_entity_type<Position, Rotation, Transform_matrix, Transform_root, Transform_parent>(5);

				Entity const child_transform_entity_0 = entity_manager.create_entity(
					child_transform_entity_type,
					Position{ { -1.0f, 0.5f, -2.0f } },
					Rotation{ { std::cos(pi / 4.0f), std::sin(pi / 4.0f), 0.0f, 0.0f } },
					Transform_matrix{},
					Transform_root{ root_transform_entity },
					Transform_parent{ root_transform_entity }
				);

				Entity const child_transform_entity_1 = entity_manager.create_entity(
					child_transform_entity_type,
					Position{ { 0.0f, 0.0f, 0.0f } },
					Rotation{ { std::cos(pi / 4.0f), 0.0f, 0.0f, std::sin(pi / 4.0f) } },
					Transform_matrix{},
					Transform_root{ root_transform_entity },
					Transform_parent{ child_transform_entity_0 }
				);

				Entity const child_transform_entity_2 = entity_manager.create_entity(
					child_transform_entity_type,
					Position{ { -0.5f, -4.0f, 5.0f } },
					Rotation{ { std::cos(pi / 4.0f), std::sin(pi / 4.0f), 0.0f, 0.0f } },
					Transform_matrix{},
					Transform_root{ root_transform_entity },
					Transform_parent{ child_transform_entity_1 }
				);

				Entity const child_transform_entity_3 = entity_manager.create_entity(
					child_transform_entity_type,
					Position{ { 0.0f, 2.0f, 5.0f } },
					Rotation{ { 1.0f, 0.0f, 0.0f, 0.0f } },
					Transform_matrix{},
					Transform_root{ root_transform_entity },
					Transform_parent{ child_transform_entity_0 }
				);

				Entity const child_transform_entity_4 = entity_manager.create_entity(
					child_transform_entity_type,
					Position{ { 0.0f, 0.0f, -3.0f } },
					Rotation{ { std::cos(pi / 4.0f), 0.0f, 0.0f, std::sin(pi / 4.0f) } },
					Transform_matrix{},
					Transform_root{ root_transform_entity },
					Transform_parent{ root_transform_entity }
				);

				WHEN("The root transform is updated")
				{
					{
						auto[position, rotation] = entity_manager.get_components_data<Position, Rotation>(root_transform_entity);
						Transform_matrix const root_transform_matrix = create_transform(position, rotation);
						entity_manager.set_component_data(root_transform_entity, root_transform_matrix);
					}

					AND_WHEN("The transform tree is created")
					{
						Transforms_tree const transform_tree =
							create_transforms_tree(entity_manager, root_transform_entity);

						AND_WHEN("The child transforms are updated")
						{
							update_child_transforms(entity_manager, transform_tree, root_transform_entity);

							THEN("The root transform is calculated correctly")
							{
								Transform_matrix const transform_matrix = 
									entity_manager.get_component_data<Transform_matrix>(root_transform_entity);

								Eigen::Matrix4f expected_transform_matrix;
								expected_transform_matrix <<
									0.0f, 0.0f, 1.0f, 1.0f,
									0.0f, 1.0f, 0.0f, 2.0f, 
									-1.0f, 0.0f, 0.0f, 3.0f, 
									0.0f, 0.0f, 0.0f, 1.0f;

								CHECK(transform_matrix.value.isApprox(expected_transform_matrix));
							}

							THEN("The child transform 0 is calculated correctly")
							{
								Transform_matrix const transform_matrix =
									entity_manager.get_component_data<Transform_matrix>(root_transform_entity);

								Eigen::Matrix4f expected_transform_matrix;
								expected_transform_matrix <<
									0.0f, 0.0f, 1.0f, 0.0f,
									1.0f, 0.0f, 0.0f, -2.5f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f;

								CHECK(transform_matrix.value.isApprox(expected_transform_matrix));
							}

							THEN("The child transform 1 is calculated correctly")
							{
								Transform_matrix const transform_matrix =
									entity_manager.get_component_data<Transform_matrix>(root_transform_entity);

								Eigen::Matrix4f expected_transform_matrix;
								expected_transform_matrix <<
									0.0f, 0.0f, 1.0f, -0.5f,
									1.0f, 0.0f, 0.0f, -1.0f,
									0.0f, 1.0f, 0.0f, -2.0f,
									0.0f, 0.0f, 0.0f, 1.0f;

								CHECK(transform_matrix.value.isApprox(expected_transform_matrix));
							}

							THEN("The child transform 2 is calculated correctly")
							{
								Transform_matrix const transform_matrix =
									entity_manager.get_component_data<Transform_matrix>(root_transform_entity);

								Eigen::Matrix4f expected_transform_matrix;
								expected_transform_matrix <<
									0.0f, -1.0f, 0.0f, -0.5f,
									0.0f, 0.0f, -1.0f, -4.0f,
									1.0f, 0.0f, 0.0f, 5.0f,
									0.0f, 0.0f, 0.0f, 1.0f;

								CHECK(transform_matrix.value.isApprox(expected_transform_matrix));
							}

							THEN("The child transform 3 is calculated correctly")
							{
								Transform_matrix const transform_matrix =
									entity_manager.get_component_data<Transform_matrix>(root_transform_entity);

								Eigen::Matrix4f expected_transform_matrix;
								expected_transform_matrix <<
									1.0f, 0.0f, 0.0f, -1.0f,
									0.0f, 0.0f, -1.0f, 2.5f,
									0.0f, 1.0f, 0.0f, 3.0f,
									0.0f, 0.0f, 0.0f, 1.0f;

								CHECK(transform_matrix.value.isApprox(expected_transform_matrix));
							}

							THEN("The child transform 4 is calculated correctly")
							{
								Transform_matrix const transform_matrix =
									entity_manager.get_component_data<Transform_matrix>(root_transform_entity);

								Eigen::Matrix4f expected_transform_matrix;
								expected_transform_matrix <<
									0.0f, -1.0f, 0.0f, -2.0f,
									0.0f, 0.0f, 1.0f, 1.0f,
									-1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f;

								CHECK(transform_matrix.value.isApprox(expected_transform_matrix));
							}
						}
					}
				}
			}
		}
	}
}
