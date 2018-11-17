#include <Maia/GameEngine/Systems/Transform_system.hpp>

namespace Maia::GameEngine::Systems
{
	Transform_matrix create_transform(Position const& position, Rotation const& rotation)
	{
		Eigen::Vector3f const& translation = position.value;
		Eigen::Matrix3f const rotation_matrix = rotation.value.matrix();

		Eigen::Matrix4f matrix;
		matrix <<
			rotation_matrix(0, 0), rotation_matrix(0, 1), rotation_matrix(0, 2), translation(0),
			rotation_matrix(1, 0), rotation_matrix(1, 1), rotation_matrix(1, 2), translation(1),
			rotation_matrix(2, 0), rotation_matrix(2, 1), rotation_matrix(2, 2), translation(2),
			0.0f, 0.0f, 0.0f, 1.0f;

		return { matrix };
	}

	Transforms_tree create_transforms_tree(
		Entity_manager const& entity_manager,
		Entity root_transform_entity
	)
	{
		return {};
	}

	void update_child_transforms(
		Entity_manager& entity_manager,
		Transforms_tree const& transforms_tree,
		Entity root_transform_entity
	)
	{
	}
}
