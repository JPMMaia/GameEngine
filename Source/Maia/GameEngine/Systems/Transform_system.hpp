#ifndef MAIA_GAMEENGINE_TRANSFORMSYSTEM_H_INCLUDED
#define MAIA_GAMEENGINE_TRANSFORMSYSTEM_H_INCLUDED

#include <deque>
#include <future>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <Maia/GameEngine/Entity.hpp>
#include <Maia/GameEngine/Entity_manager.hpp>

namespace Maia::GameEngine::Systems
{
	struct Position
	{
		Eigen::Vector3f value;
	};

	struct Rotation
	{
		Eigen::Quaternionf value;
	};

	struct Transform_dirty
	{
		bool value;
	};

	struct Transform_parent
	{
		Entity entity;
	};

	struct Transform_matrix
	{
		Eigen::Matrix4f value;
	};

	using Transforms_tree = std::unordered_multimap<Transform_parent, Entity>;


	Transform_matrix build_transform(Position const& position, Rotation const& rotation);
	
	Transforms_tree build_transforms_tree(
		Entity_manager const& entity_manager,
		Entity root_transform_entity
	);

	void build_child_transforms(
		Entity_manager& entity_manager,
		Transforms_tree const& transforms_tree,
		Entity root_transform_entity
	);


	class Transform_system
	{
	public:

		std::future<void> execute_async()
		{
			// Go through all entities that have a Position, Rotation and not have a Transform_parent
			{
				// Create a new thread
				{
					// build_root_transform(position, rotation);

					// build_transformation_tree
				}
			}

			return {};
		}

	private:

		// For each root matrix, create a thread

		// Called for each thread

		void build_root_transform()
		{
			// Write to root transform
		}

		std::unordered_multimap<Transform_parent, Entity> build_transformation_tree(Entity root_transform_entity)
		{
			// Go through all entities that have a Position, Rotation and a Transform_parent
			// Add entity to multimap by parent

			return {};
		}

		void build_non_root_transforms(Entity root_transform_entity)
		{
			// auto children = hash_map[root_transform.entity];

			// For each child in children
			{
				// write to child transform
			}

			// For each child in children
			{
				// build_non_root_transforms(child.entity);
			}
		}
	};
}

#endif
