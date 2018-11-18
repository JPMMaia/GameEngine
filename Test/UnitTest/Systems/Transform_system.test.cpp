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
							CHECK(transform_tree.find({ root_transform_entity }) == transform_tree.end());
						}
					}
					
				}
			}
		}
	}
}
