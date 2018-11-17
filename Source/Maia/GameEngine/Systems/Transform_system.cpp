#include <Maia/GameEngine/Systems/Transform_system.hpp>

namespace Maia::GameEngine::Systems
{
	Transform_matrix create_transform(Position const& position, Rotation const& rotation)
	{
		return Transform_matrix(position.value, rotation.value);
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
