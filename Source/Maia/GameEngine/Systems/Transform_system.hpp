#ifndef MAIA_GAMEENGINE_TRANSFORMSYSTEM_H_INCLUDED
#define MAIA_GAMEENGINE_TRANSFORMSYSTEM_H_INCLUDED

#include <deque>
#include <functional>
#include <future>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <Maia/GameEngine/Entity.hpp>
#include <Maia/GameEngine/Entity_manager.hpp>

namespace Maia::GameEngine::Systems
{
	struct Position
	{
		Eigen::Vector3f value{ 0.0f, 0.0f, 0.0f };

		static Component_ID ID()
		{
			return { 1 };
		}
	};

	struct Rotation
	{
		Eigen::Quaternionf value{ 1.0f, 0.0f, 0.0f, 0.0f };

		static Component_ID ID()
		{
			return { 2 };
		}
	};

	struct Transform_dirty
	{
		bool value;

		static Component_ID ID()
		{
			return { 3 };
		}
	};

	struct Transform_parent
	{
		Entity entity;

		static Component_ID ID()
		{
			return { 4 };
		}
	};

	inline bool operator==(Transform_parent const& lhs, Transform_parent const& rhs)
	{
		return lhs.entity == rhs.entity;
	}
	inline bool operator!=(Transform_parent const& lhs, Transform_parent const& rhs)
	{
		return !(lhs == rhs);
	}


	struct Transform_root
	{
		Entity entity;

		static Component_ID ID()
		{
			return { 5 };
		}
	};

	struct Transform_matrix
	{
		Eigen::Matrix4f value{ Eigen::Matrix4f::Identity() };

		static Component_ID ID()
		{
			return { 6 };
		}
	};

	using Transforms_tree = std::unordered_multimap<Transform_parent, Entity>;
}

namespace std
{
	// TODO refactor

	template<> struct hash<Maia::GameEngine::Systems::Transform_parent>
	{
		using argument_type = Maia::GameEngine::Systems::Transform_parent;
		using result_type = std::size_t;

		result_type operator()(argument_type const& transform_parent) const noexcept
		{
			return std::hash<Maia::GameEngine::Entity::Integral_type>{}(transform_parent.entity.value);
		}
	};
}

namespace Maia::GameEngine::Systems
{
	Transform_matrix create_transform(Position const& position, Rotation const& rotation);

	Transforms_tree create_transforms_tree(
		Entity_manager const& entity_manager,
		Entity root_transform_entity
	);

	void update_child_transforms(
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
