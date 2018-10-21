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


		std::size_t size() const
		{
			if (m_chunks.empty())
			{
				return 0;
			}
			else if (m_first_chunk_not_full == 0)
			{
				return m_chunks.front()->size();
			}
			else if (m_first_chunk_not_full == m_chunks.size())
			{
				return m_chunks.size() * Capacity_per_chunk;
			}
			else
			{
				return m_first_chunk_not_full * Capacity_per_chunk + m_chunks[m_first_chunk_not_full]->size();
			}
		}

		void reserve(std::size_t const new_capacity)
		{
			std::size_t const number_of_chunks = (new_capacity + Capacity_per_chunk - 1) / Capacity_per_chunk;

			m_chunks.reserve(number_of_chunks);

			while (m_chunks.size() < number_of_chunks)
			{
				m_chunks.emplace_back(
					std::make_unique<Memory_chunk<Capacity_per_chunk, Entity, Components...>>()
				);
			}
		}

		std::size_t capacity() const
		{
			return m_chunks.size() * Capacity_per_chunk;
		}

		void shrink_to_fit()
		{
		}


		Element_moved erase(Index index)
		{
			return {};
		}

		Index push_back(Entity entity, Components... components)
		{
			if (size() == capacity())
			{
				reserve(capacity() + Capacity_per_chunk);
			}

			auto& chunk_to_push = *m_chunks[m_first_chunk_not_full];

			chunk_to_push.push_back(entity, components...);
			
			if (chunk_to_push.size() == Capacity_per_chunk)
			{
				++m_first_chunk_not_full;
			}

			return { size() - 1 };
		}

		std::tuple<Entity, Components...> pop_back()
		{
			return {};
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


	private:

		std::size_t m_first_chunk_not_full{ 0 };
		std::vector<std::unique_ptr<Memory_chunk<Capacity_per_chunk, Entity, Components...>>> m_chunks;

	};
}

#endif
