#ifndef MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYMANAGER_H_INCLUDED

#include <cstddef>
#include <cstdint>
#include <vector>

#include <Maia/GameEngine/Entity.hpp>
#include <Maia/GameEngine/Component_group.hpp>

namespace Maia::GameEngine
{
	class Entity_manager
	{
	public:

		using index_type = std::size_t;

		

		Component_group& create_component_group(/*...*/);

		Entity create_entity(Component_group& component_group);
		void destroy_entity(Entity entity);

		template <class T>
		T get_component_data(Entity entity);

		template <class T>
		void set_component_data(Entity entity, T&& data);

		void add_shared_component_data(Entity entity/*, todo */);

	private:

		std::vector<index_type> m_entities_indices;
		std::vector<Component_group> m_component_groups;

	};
}

#endif
