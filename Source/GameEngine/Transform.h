#ifndef MAIA_GAMEENGINE_TRANSFORM_H_INCLUDED
#define MAIA_GAMEENGINE_TRANSFORM_H_INCLUDED

#include <cstddef>
#include <mutex>
#include <shared_mutex>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace Maia::GameEngine
{
	// TODO consider splitting in two different classes to handle multithreading
	class Transform
	{
	public:

		struct Parent
		{
			std::size_t index;
		};

		static std::unique_lock<std::shared_mutex> write_lock();
		static std::shared_lock<std::shared_mutex> read_lock();

		Transform();
		Transform(const Transform&) = delete;
		Transform(Transform&&) = delete;
		~Transform();

		Transform& operator=(const Transform&) = delete;
		Transform& operator=(Transform&&) = delete;

		Parent parent() const;
		void parent(Parent parent);

		Eigen::Vector3f local_translation() const;
		Eigen::Quaternionf local_rotation() const;
		Eigen::Vector3f local_scale() const;
		
		Eigen::Matrix4f parent_transform() const;
		void parent_transform(const Eigen::Vector3f& translation, const Eigen::Quaternionf& rotation, const Eigen::Vector3f& scale);
		
		Eigen::Matrix4f world_transform() const;

	private:

		struct Data
		{
			// TODO add allocators
			std::vector<Parent> parents;
			std::vector<Eigen::Vector3f> translations;
			std::vector<Eigen::Quaternionf> rotations;
			std::vector<Eigen::Vector3f> scales;
			std::vector<Eigen::Matrix4f> world_transforms;
		};

		static Data s_data;
		static std::shared_mutex m_mutex;
		std::size_t m_index;

	};
}

#endif
