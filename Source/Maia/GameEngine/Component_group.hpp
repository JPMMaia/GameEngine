#ifndef MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED
#define MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED

#include <Maia/GameEngine/Entity.hpp>
#include <Maia/GameEngine/Memory_chunk.hpp>

namespace Maia::GameEngine
{
	struct Component_group_index
	{
		std::size_t value;
	};

	struct Component_group_element_moved
	{
		Entity entity;
	};

	template <std::size_t Capacity_per_chunk, typename... Components>
	class Component_group
	{
	public:

		using Index = Component_group_index;
		using Element_moved = Component_group_element_moved;

		Index push_back(Entity entity, Components... components)
		{
			return {};
		}

		std::tuple<Entity, Components...> pop_back()
		{
			return {};
		}

		Element_moved remove(Index index)
		{
			return {};
		}

		void reserve(std::size_t new_capacity)
		{
		}

		void shrink_to_fit()
		{
		}

		std::tuple<Entity, Components...> get_components_data(Index index) const
		{
			return {};
		}

		void set_components_data(Index index, Entity entity, Components... components)
		{
		}

		template <typename Component> // TODO enable_if Component is an element of Components
		Component get_component_data(Index index) const
		{
			return {};
		}

		template <typename Component> // TODO enable_if Component is an element of Components
		void set_component_data(Index index, Component&& component)
		{
		}

		std::size_t capacity() const
		{
			return {};
		}

		std::size_t size() const
		{
			return {};
		}

	private:

		std::vector<Memory_chunk<Capacity_per_chunk, Entity, Components...>> m_chunks;

	};
}

#endif
