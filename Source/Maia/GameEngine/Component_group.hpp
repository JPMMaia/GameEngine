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

	template <std::size_t Capacity_per_chunk>
	class Component_group
	{
	public:

		using Index = Component_group_index;
		using Element_moved = Component_group_element_moved;


		std::size_t size() const
		{
			/*if (m_chunks.empty())
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
			}*/
			return {};
		}

		void reserve(std::size_t const new_capacity)
		{
			/*std::size_t const number_of_chunks = (new_capacity + Capacity_per_chunk - 1) / Capacity_per_chunk;

			m_chunks.reserve(number_of_chunks);

			while (m_chunks.size() < number_of_chunks)
			{
				m_chunks.emplace_back(
					std::make_unique<Memory_chunk<Capacity_per_chunk, Entity, Components...>>()
				);
			}*/
		}

		std::size_t capacity() const
		{
			//return m_chunks.size() * Capacity_per_chunk;
			return {};
		}

		void shrink_to_fit()
		{
			/*std::size_t const ideal_number_of_chunks = (size() + Capacity_per_chunk - 1) / Capacity_per_chunk;

			while (m_chunks.size() > ideal_number_of_chunks)
			{
				m_chunks.pop_back();
			}
			
			m_chunks.shrink_to_fit();*/
		}


		Element_moved erase(Index index)
		{
			/*std::size_t const chunk_index = index.value / Capacity_per_chunk;
			auto& chunk = *m_chunks[chunk_index];

			Memory_chunk_index const element_index = { index.value - chunk_index };

			auto const components_data_at_back = pop_back();
			std::apply(
				[&](auto... components_data) { chunk.set_components_data(element_index, components_data...); },
				components_data_at_back
			);

			return { std::get<0>(components_data_at_back) };*/
			return {};
		}

		template <typename... Components>
		Index push_back(Entity entity, Components... components)
		{
			/*if (size() == capacity())
			{
				reserve(capacity() + Capacity_per_chunk);
			}

			auto& chunk_to_push = *m_chunks[m_first_chunk_not_full];

			chunk_to_push.push_back(entity, components...);
			
			if (chunk_to_push.size() == Capacity_per_chunk)
			{
				++m_first_chunk_not_full;
			}

			return { size() - 1 };*/
			return {};
		}

		template <typename... Components>
		std::tuple<Components...> pop_back()
		{
			/*auto& chunk_to_pop = *m_chunks[m_first_chunk_not_full];

			if (m_first_chunk_not_full > 0 && chunk_to_pop.size() == 1)
			{
				--m_first_chunk_not_full;
			}

			return chunk_to_pop.pop_back();*/
			return {};
		}

		template <typename Component>
		Component get_component_data(Index index) const
		{
			/*std::size_t const chunk_index = index.value / Capacity_per_chunk;
			auto const& chunk = *m_chunks[chunk_index];

			Memory_chunk_index const element_index = { index.value - chunk_index };

			return chunk.get_data<Component>(element_index);*/

			return {};
		}

		template <typename... Components>
		std::tuple<Components...> get_components_data(Index index) const
		{
			return {};
		}

		template <typename Component>
		void set_component_data(Index index, Component&& component)
		{
			/*std::size_t const chunk_index = index.value / Capacity_per_chunk;
			auto& chunk = *m_chunks[chunk_index];

			Memory_chunk_index const element_index = { index.value - chunk_index };

			chunk.set_data<Component>(element_index, std::forward<Component>(component));*/
		}

		template <typename... Components>
		void set_components_data(Index index, Components&&... components) const
		{
		}

	private:

		std::size_t m_first_chunk_not_full{ 0 };
		std::vector<std::unique_ptr<Memory_chunk<Capacity_per_chunk>>> m_chunks;

	};
}

#endif
