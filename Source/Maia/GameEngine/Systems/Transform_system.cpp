#include <Maia/GameEngine/Systems/Transform_system.hpp>

namespace Maia::GameEngine::Systems
{
	Transform_matrix build_transform(Position const& position, Rotation const& rotation)
	{
		return {};
	}

	Transforms_tree build_transforms_tree(
		Entity_manager const& entity_manager,
		Entity root_transform_entity
	)
	{
		return {};
	}

	void build_child_transforms(
		Entity_manager& entity_manager,
		Transforms_tree const& transforms_tree,
		Entity root_transform_entity
	)
	{
	}
}
