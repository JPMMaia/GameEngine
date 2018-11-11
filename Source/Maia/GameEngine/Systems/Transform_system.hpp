#ifndef MAIA_GAMEENGINE_TRANSFORMSYSTEM_H_INCLUDED
#define MAIA_GAMEENGINE_TRANSFORMSYSTEM_H_INCLUDED

#include <deque>

#include <Eigen/Core>

#include <Maia/GameEngine/Entity.hpp>

namespace Maia::GameEngine::Systems
{
	struct Transform_dirty
	{
		bool value;
	};

	struct Transform_parent
	{
		Entity entity;
	};

	struct Transform_world_matrix
	{
		Eigen::Matrix4f value;
	};

	class Transform_system
	{
	public:

		void execute()
		{
			// Go through all entities that have a Position, Rotation and not have a Transform_parent
			{
				// Create a new thread
				{
					// build_root_transform(position, rotation);

					// build_transformation_tree
				}
			}
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
