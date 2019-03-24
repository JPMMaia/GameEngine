#ifndef MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED

#include <any>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
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


	struct Space
	{
		std::size_t value;
	};

	inline bool operator==(Space const& lhs, Space const& rhs)
	{
		return lhs.value == rhs.value;
	}
	
	inline bool operator!=(Space const& lhs, Space const& rhs)
	{
		return !(lhs == rhs);
	}


	class Entity_manager
	{
	public:

		template <typename T>
		using Remove_cvr_t = std::remove_cv_t<std::remove_reference_t<T>>;

		Entity_type_id create_entity_type(
			std::size_t capacity_per_chunk,
			gsl::span<Component_info const> component_infos,
			Space const space
		);

		template <typename... Components>
		Entity_type<Components...> create_entity_type(std::size_t capacity_per_chunk)
		{
			m_component_groups.emplace_back(
				make_component_group<Components..., Entity>(capacity_per_chunk));
			
			m_component_types_groups.emplace_back(
				make_component_types_group<Components..., Entity>()
			);

			Entity_type<Components...> const entity_type{ m_entity_type_ids.size() };
			m_entity_type_ids.push_back(entity_type.id);

			return entity_type;
		}

		
		template <typename... Components>
		Entity create_entity(Entity_type<Remove_cvr_t<Components>...> entity_type, Components&&... components)
		{
			assert(m_entity_type_indices.size() < std::numeric_limits<Entity::Integral_type>::max());

			const auto entity_type_index = [&]() -> Entity_type_index
			{
				const auto entity_type_id_location = std::find(m_entity_type_ids.begin(), m_entity_type_ids.end(), entity_type.id);

				return { static_cast<std::size_t>(std::distance(m_entity_type_ids.begin(), entity_type_id_location)) };
			}();

			if (!m_deleted_entities.empty())
			{
				Entity const entity = m_deleted_entities.back();
				m_deleted_entities.pop_back();
				m_entities_existence[entity.value] = true;

				m_entity_type_indices[entity.value] = entity_type_index;

				Component_group& component_group = m_component_groups[entity_type_index.value];
				Component_group_index component_group_index = component_group.push_back(std::forward<Components>(components)..., entity);
				m_component_group_indices[entity.value] = component_group_index;

				return entity;
			}
			else
			{
				Entity entity
				{
					static_cast<Entity::Integral_type>(m_entity_type_indices.size())
				};
				m_entities_existence.push_back(true);

				m_entity_type_indices.push_back(entity_type_index);

				Component_group& component_group = m_component_groups[entity_type_index.value];
				Component_group_index component_group_index = component_group.push_back(std::forward<Components>(components)..., entity);
				m_component_group_indices.push_back(component_group_index);

				return entity;
			}
		}

		Entity create_entity(Entity_type_id entity_type_id)
		{
			assert(m_entity_type_indices.size() < std::numeric_limits<Entity::Integral_type>::max());

			const Entity_type_index entity_type_index = [&]() -> Entity_type_index
			{
				const auto entity_type_id_location = std::find(m_entity_type_ids.begin(), m_entity_type_ids.end(), entity_type_id);

				return { static_cast<std::size_t>(std::distance(m_entity_type_ids.begin(), entity_type_id_location)) };
			}();

			if (!m_deleted_entities.empty())
			{
				Entity const entity = m_deleted_entities.back();
				m_deleted_entities.pop_back();
				m_entities_existence[entity.value] = true;

				m_entity_type_indices[entity.value] = entity_type_index;

				Component_group& component_group = m_component_groups[entity_type_index.value];
				Component_group_index component_group_index = component_group.push_back(entity);
				m_component_group_indices[entity.value] = component_group_index;

				return entity;
			}
			else
			{
				Entity entity
				{
					static_cast<Entity::Integral_type>(m_entity_type_indices.size())
				};
				m_entities_existence.push_back(true);

				m_entity_type_indices.push_back(entity_type_index);

				Component_group& component_group = m_component_groups[entity_type_index.value];
				Component_group_index component_group_index = component_group.push_back(entity);
				m_component_group_indices.push_back(component_group_index);

				return entity;
			}
		}

		template <typename... Components>
		Entity create_entity(Entity_type<Components...> entity_type)
		{
			return create_entity(entity_type, Components{}...);
		}

		template <typename... Components>
		std::vector<Entity> create_entities(std::size_t count, Entity_type<Remove_cvr_t<Components>...> entity_type, Components&&... components)
		{
			assert(m_entity_type_indices.size() + count <= std::numeric_limits<Entity::Integral_type>::max());

			const Entity_type_index entity_type_index = [&]() -> Entity_type_index
			{
				const auto entity_type_id_location = std::find(m_entity_type_ids.begin(), m_entity_type_ids.end(), entity_type.id);

				return { static_cast<std::size_t>(std::distance(m_entity_type_ids.begin(), entity_type_id_location)) };
			}();

			Component_group& component_group = m_component_groups[entity_type_index.value];

			std::vector<Entity> entities;
			entities.reserve(count);

			for (std::size_t i = 0; i < count; ++i)
			{
				if (!m_deleted_entities.empty())
				{
					Entity const entity = m_deleted_entities.back();
					m_deleted_entities.pop_back();
					m_entities_existence[entity.value] = true;

					m_entity_type_indices[entity.value] = entity_type_index;

					Component_group_index component_group_index = component_group.push_back(std::forward<Components>(components)..., entity);
					m_component_group_indices[entity.value] = component_group_index;

					entities.push_back(entity);
				}
				else
				{
					Entity entity
					{
						static_cast<Entity::Integral_type>(m_entity_type_indices.size())
					};
					m_entity_type_indices.push_back(entity_type_index);

					Component_group_index component_group_index = component_group.push_back(std::forward<Components>(components)..., entity);
					m_component_group_indices.push_back(component_group_index);

					m_entities_existence.push_back(true);

					entities.push_back(entity);
				}
			}

			return entities;
		}

		template <std::size_t Count, typename... Components>
		std::array<Entity, Count> create_entities(Entity_type<Remove_cvr_t<Components>...> entity_type, Components&&... components)
		{
			// TODO refactor

			assert(m_entity_type_indices.size() + Count <= std::numeric_limits<Entity::Integral_type>::max());

			const auto entity_type_index = [&]() -> Entity_type_index
			{
				const auto entity_type_id_location = std::find(m_entity_type_ids.begin(), m_entity_type_ids.end(), entity_type.id);

				return { static_cast<std::size_t>(std::distance(m_entity_type_ids.begin(), entity_type_id_location)) };
			}();

			Component_group& component_group = m_component_groups[entity_type_index.value];

			std::array<Entity, Count> entities;

			for (std::size_t i = 0; i < entities.size(); ++i)
			{
				if (!m_deleted_entities.empty())
				{
					Entity const entity = m_deleted_entities.back();
					m_deleted_entities.pop_back();
					m_entities_existence[entity.value] = true;

					m_entity_type_indices[entity.value] = entity_type_index;

					Component_group_index component_group_index = component_group.push_back(std::forward<Components>(components)..., entity);
					m_component_group_indices[entity.value] = component_group_index;

					entities[i] = entity;
				}
				else
				{
					Entity entity
					{
						static_cast<Entity::Integral_type>(m_entity_type_indices.size())
					};
					m_entity_type_indices.push_back(entity_type_index);

					Component_group_index component_group_index = component_group.push_back(std::forward<Components>(components)..., entity);
					m_component_group_indices.push_back(component_group_index);

					m_entities_existence.push_back(true);

					entities[i] = entity;
				}
			}

			return entities;
		}

		void destroy_entity(Entity entity)
		{
			m_entities_existence[entity.value] = false;
			m_deleted_entities.push_back(entity);

			Entity_type_index const entity_type_index = m_entity_type_indices[entity.value];
			Component_group_index const component_group_index = m_component_group_indices[entity.value];

			if (auto const element_moved = m_component_groups[entity_type_index.value].erase(component_group_index))
			{
				m_component_group_indices[element_moved->entity.value] = component_group_index;
			}
		}

		bool exists(Entity entity) const
		{
			return m_entities_existence[entity.value];
		}


		template <typename Component>
		bool has_component(Entity entity) const
		{
			Entity_type_index const entity_type_index = m_entity_type_indices[entity.value];

			return m_component_types_groups[entity_type_index.value].contains<Component>();
		}


		template <typename Component>
		Component get_component_data(Entity entity) const
		{
			assert(m_component_types_groups[m_entity_type_indices[entity.value].value].contains<Component>());

			Entity_type_index entity_type_index = m_entity_type_indices[entity.value];
			Component_group const& component_group = m_component_groups[entity_type_index.value];

			Component_group_index component_group_index = m_component_group_indices[entity.value];
			return component_group.get_component_data<Component>(component_group_index);
		}

		template <typename Component>
		void set_component_data(Entity entity, Component&& data)
		{
			assert(m_component_types_groups[m_entity_type_indices[entity.value].value].contains<Component>());

			Entity_type_index entity_type_index = m_entity_type_indices[entity.value];
			Component_group& component_group = m_component_groups[entity_type_index.value];
			
			Component_group_index component_group_index = m_component_group_indices[entity.value];
			component_group.set_component_data<Component>(component_group_index, std::forward<Component>(data));
		}


		template <typename... Components>
		std::tuple<Components...> get_components_data(Entity entity) const
		{
			Entity_type_index entity_type_index = m_entity_type_indices[entity.value];
			Component_group const& component_group = m_component_groups[entity_type_index.value];

			Component_group_index component_group_index = m_component_group_indices[entity.value];
			return component_group.get_components_data<Components...>(component_group_index);
		}

		template <typename... Components>
		void set_components_data(Entity entity, Components&&... data)
		{
			Entity_type_index entity_type_index = m_entity_type_indices[entity.value];
			Component_group& component_group = m_component_groups[entity_type_index.value];

			Component_group_index component_group_index = m_component_group_indices[entity.value];
			component_group.set_components_data<Components...>(component_group_index, std::forward<Components>(data)...);
		}

		Component_group const& get_component_group(Entity_type_id entity_type_id) const
		{
			return m_component_groups.back();
		}
		Component_group& get_component_group(Entity_type_id entity_type_id)
		{
			return m_component_groups.back();
		}


		gsl::span<const Component_types_group> get_component_types_groups() const
		{
			return m_component_types_groups;
		}

		gsl::span<Component_types_group> get_component_types_groups()
		{
			return m_component_types_groups;
		}


		gsl::span<const Component_group> get_component_groups() const
		{
			return m_component_groups;
		}

		gsl::span<Component_group> get_component_groups()
		{
			return m_component_groups;
		}

	private:


		static bool is_valid(Component_group_index component_group_index)
		{
			return component_group_index.value != -1;
		}


		// Indexed by Entity_type_index
		std::vector<Entity_type_id> m_entity_type_ids;
		std::vector<Space> m_component_types_spaces;
		std::vector<Component_types_group> m_component_types_groups;
		std::vector<Component_group> m_component_groups;

		// Indexed by Entity.id
		std::vector<Entity_type_index> m_entity_type_indices;
		std::vector<Component_group_index> m_component_group_indices;
		std::vector<bool> m_entities_existence;
		std::vector<Entity> m_deleted_entities;


	};
}

#endif
