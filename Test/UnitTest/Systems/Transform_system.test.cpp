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
	}
}
