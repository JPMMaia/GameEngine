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
		Eigen::Vector3f value;

		Position() :
			value{ 0.0f, 0.0f, 0.0f }
		{
		}

		Position(Eigen::Vector3f value) :
			value(value)
		{
		}
	};

	struct Rotation
	{
		Eigen::Quaternionf value;

		Rotation() :
			value{ 1.0f, 0.0f, 0.0f, 0.0f }
		{
		}

		Rotation(Eigen::Quaternionf value) :
			value(value)
		{
		}
	};

	struct Transform_dirty
	{
		bool value;
	};

	struct Transform_parent
	{
		Entity root_entity;
		Entity parent_entity;
	};

	struct Transform_matrix
	{
		Eigen::Matrix4f value;

		Transform_matrix() = default;

		Transform_matrix(
			Eigen::Vector3f const& translation,
			Eigen::Quaternionf const& rotation
		)
		{
			Eigen::Matrix3f const rotation_matrix = rotation.matrix();

			value <<
				rotation_matrix(0, 0), rotation_matrix(0, 1), rotation_matrix(0, 2), translation(0),
				rotation_matrix(1, 0), rotation_matrix(1, 1), rotation_matrix(1, 2), translation(1),
				rotation_matrix(2, 0), rotation_matrix(2, 1), rotation_matrix(2, 2), translation(2),
				0.0f, 0.0f, 0.0f, 1.0f;
		}

		Transform_matrix(
			Eigen::Vector3f const& first_column, 
			Eigen::Vector3f const& second_column,
			Eigen::Vector3f const& third_column,
			Eigen::Vector3f const& fourth_column
		)
		{
			value <<
				first_column(0), second_column(0), third_column(0), fourth_column(0),
				first_column(1), second_column(1), third_column(1), fourth_column(1),
				first_column(2), second_column(2), third_column(2), fourth_column(2),
				0.0f, 0.0f, 0.0f, 1.0f;
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
			return std::hash<std::uint32_t>{}(transform_parent.parent_entity.value);
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
