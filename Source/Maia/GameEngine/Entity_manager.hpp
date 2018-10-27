#ifndef MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED

#include <any>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <Maia/GameEngine/Component_group.hpp>
#include <Maia/GameEngine/Component_types_group.hpp>
#include <Maia/GameEngine/Entity.hpp>
#include <Maia/GameEngine/Entity_type.hpp>

namespace Maia::GameEngine
{
	struct Entity_data
	{
		std::size_t entity_type_index;
		bool is_destroyed;
	};

	class Entity_manager
	{
	public:

		template <typename... Types>
		Entity_type create_entity_type()
		{
			m_component_types_groups.emplace_back(
				make_component_types_group<Types...>()
			);

			Entity_type const entity_type{ m_entity_types.size() };

			m_entity_types.push_back(entity_type);

			return entity_type;
		}

		Entity create_entity(Entity_type entity_type)
		{
			Entity entity
			{
				static_cast<Entity::ID>(m_entities_data.size())
			};

			const auto entity_type_index = [&]() -> std::size_t
			{
				const auto entity_type_location = std::find(m_entity_types.begin(), m_entity_types.end(), entity_type);

				return std::distance(m_entity_types.begin(), entity_type_location);
			}();

			m_entities_data.push_back({ entity_type_index, false });

			return entity;
		}
		void destroy_entity(Entity entity)
		{
			m_entities_data[entity.id].is_destroyed = true;
			
			m_destroyed_entities.push_back(entity);
		}
		
		bool exists(Entity entity) const
		{
			if (entity.id >= m_entities_data.size())
				return false;

			return !m_entities_data[entity.id].is_destroyed;
		}

		template <typename Component>
		void add_component(Entity entity, Component component)
		{
			// Pre conditions:
			//	entity_data.is_destroyed == false
			//	component_types_group.contains<Component>()

			Entity_data const& entity_data = m_entities_data[entity.id];

			Component_types_group component_types_group = m_component_types_groups[entity_data.entity_type_index];
		}

		template <typename Component>
		void remove_component(Entity entity)
		{
		}

		template <typename Component>
		bool has_component() const
		{
			return false;
		}

		template <typename Component>
		Component get_component_data(Entity entity) const
		{
			return {};
		}

		template <typename... Components>
		std::tuple<Components...> get_components_data(Entity entity) const
		{
			return {};
		}

		template <typename Component>
		void set_component_data(Entity entity, Component&& data)
		{
		}

		template <typename... Components>
		void set_components_data(Entity entity, Components&&... data)
		{
		}

	private:

		std::vector<Entity_type> m_entity_types;
		std::vector<Component_types_group> m_component_types_groups;
		std::vector<std::any> m_component_groups;
		std::vector<Entity_data> m_entities_data;
		std::vector<Entity> m_destroyed_entities;
	};
}

#endif
