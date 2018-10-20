#ifndef MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <Maia/GameEngine/Component_group.hpp>
#include <Maia/GameEngine/Entity.hpp>
#include <Maia/GameEngine/Entity_type.hpp>

namespace Maia::GameEngine
{
	class Entity_type_not_found : public std::logic_error
	{
	public:

		Entity_type_not_found() : 
			std::logic_error("Entity type not found!")
		{
		}
	};

	class Entity_not_found : public std::logic_error
	{
	public:

		Entity_not_found() :
			std::logic_error("Entity not found!")
		{
		}
	};

	struct Entity_data
	{
		std::size_t entity_type_index;
	};

	class Entity_manager
	{
	public:

		template <class... Types>
		Entity_type create_entity_type()
		{
			Entity_type entity_type;
			
			auto set_mask = 
			{ 
				(entity_type.m_mask.set(Types::ID()))... 
			};

			const auto entity_type_location = std::find_if(m_entities_types.begin(), m_entities_types.end(),
				[&](Entity_type value) -> bool { return value.m_mask == entity_type.m_mask; });

			if (entity_type_location == m_entities_types.end())
			{
				m_entities_types.push_back(entity_type);
			}

			return entity_type;
		}

		Entity create_entity(Entity_type entity_type)
		{
			/*Entity entity;
			entity.m_id = static_cast<uint32_t>(m_entities_data.size());

			const auto entity_type_index = [&]() -> std::size_t
			{
				const auto entity_type_location = std::find_if(m_entities_types.begin(), m_entities_types.end(),
					[&](Entity_type value) -> bool { return value.m_mask == entity_type.m_mask; });

				if (entity_type_location == m_entities_types.end())
					throw Entity_type_not_found{};

				return std::distance(m_entities_types.begin(), entity_type_location);
			}();
			m_entities_data.push_back(entity_type_index);

			return entity;*/
			return {};
		}
		void destroy_entity(Entity entity)
		{
		}

		template <class T>
		T get_component_data(Entity entity)
		{
			const auto& entity_data = m_entities_data[entity.m_id];

			const auto& entity_type = m_entities_types[entity_data];

			return {};
		}

		template <class T>
		void set_component_data(Entity entity, T&& data)
		{
			//const auto index = m_entities_indices[entity.m_id];
		}

		void add_shared_component_data(Entity entity/*, todo */);

	private:

		std::vector<std::size_t> m_entities_data;
		std::vector<Entity_type> m_entities_types;

	};
}

#endif
