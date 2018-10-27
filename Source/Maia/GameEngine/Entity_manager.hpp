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
	struct Entity_type_index
	{
		std::size_t value;
	};

	class Entity_manager
	{
	public:

		using Component_group = Component_group<65536>;

		template <typename... Types>
		Entity_type create_entity_type()
		{
			m_component_groups.emplace_back();

			Entity_type const entity_type{ m_entity_types.size() };
			m_entity_types.push_back(entity_type);

			return entity_type;
		}

		Entity create_entity(Entity_type entity_type)
		{
			Entity entity
			{
				static_cast<Entity::ID>(m_entity_type_indices.size())
			};

			const auto entity_type_index = [&]() -> std::size_t
			{
				const auto entity_type_location = std::find(m_entity_types.begin(), m_entity_types.end(), entity_type);

				return std::distance(m_entity_types.begin(), entity_type_location);
			}();

			m_entity_type_indices.push_back({ entity_type_index });

			return entity;
		}
		void destroy_entity(Entity entity)
		{
			// TODO
		}
		
		bool exists(Entity entity) const
		{
			// TODO
			return {};
		}

		template <typename Component>
		void add_component(Entity entity, Component component)
		{
			// TODO
		}

		template <typename Component>
		void remove_component(Entity entity)
		{
			// TODO
		}

		template <typename Component>
		bool has_component() const
		{
			// TODO
			return false;
		}

		template <typename Component>
		Component get_component_data(Entity entity) const
		{
			Entity_type_index entity_type_index = m_entity_type_indices[entity.id];
			Component_group const& component_group = m_component_groups[entity_type_index.value];

			Component_group_index component_group_index = m_component_group_indices[entity.id];
			return component_group.get_component_data<Component>(component_group_index);
		}

		template <typename... Components>
		std::tuple<Components...> get_components_data(Entity entity) const
		{
			Entity_type_index entity_type_index = m_entity_type_indices[entity.id];
			Component_group const& component_group = m_component_groups[entity_type_index.value];

			Component_group_index component_group_index = m_component_group_indices[entity.id];
			return component_group.get_components_data<Components...>(component_group_index);
		}

		template <typename Component>
		void set_component_data(Entity entity, Component&& data)
		{
			Entity_type_index entity_type_index = m_entity_type_indices[entity.id];
			Component_group& component_group = m_component_groups[entity_type_index.value];

			Component_group_index component_group_index = m_component_group_indices[entity.id];
			component_group.set_component_data<Component>(component_group_index, std::forward<Component>(data));
		}

		template <typename... Components>
		void set_components_data(Entity entity, Components&&... data)
		{
			Entity_type_index entity_type_index = m_entity_type_indices[entity.id];
			Component_group& component_group = m_component_groups[entity_type_index.value];

			Component_group_index component_group_index = m_component_group_indices[entity.id];
			component_group.set_components_data<Components...>(component_group_index, std::forward<Components>(data)...);
		}

	private:

		// Indexed by Entity_type_index
		std::vector<Entity_type> m_entity_types;
		std::vector<Component_group> m_component_groups;

		// Indexed by Entity.id
		std::vector<Entity_type_index> m_entity_type_indices;
		std::vector<Component_group_index> m_component_group_indices;

	};
}

#endif
