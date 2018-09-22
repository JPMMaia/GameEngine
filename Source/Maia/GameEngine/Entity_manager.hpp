#ifndef MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED

#include <cstddef>
#include <cstdint>
#include <vector>

#include <Maia/GameEngine/Entity.hpp>
#include <Maia/GameEngine/Component_group.hpp>

namespace Maia::GameEngine
{
	class Entity_not_found : public std::logic_error
	{
	};

	class Entity_manager
	{
	public:

		using index_type = std::size_t;


		Entity create_entity(Component_group component_group)
		{
			std::find(m_component_groups.begin(), m_component_groups.end(),
				[](const Component_group& component_group) -> bool { component_group });

			Entity entity;
			entity.m_id = m_entities_indices.size();

			m_entities_indices.push_back(entity.m_id);

			return entity;
		}
		void destroy_entity(Entity entity)
		{
		}

		template <class T>
		T get_component_data(Entity entity)
		{
			return {};
		}

		template <class T>
		void set_component_data(Entity entity, T&& data)
		{
			const auto index = m_entities_indices[entity];

			m_component_groups[index];
		}

		void add_shared_component_data(Entity entity/*, todo */);

	private:

		std::vector<index_type> m_entities_indices;
		std::vector<Component_group> m_component_groups;

	};
}

#endif
