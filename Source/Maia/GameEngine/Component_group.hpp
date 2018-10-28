#ifndef MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED
#define MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED

#include <algorithm>
#include <numeric>
#include <iostream>

#include <gsl/span>

#include <Maia/GameEngine/Entity.hpp>

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

	struct Component_ID
	{
		std::uint16_t value;
	};

	inline bool operator==(Component_ID lhs, Component_ID rhs)
	{
		return lhs.value == rhs.value;
	}

	struct Component_size
	{
		std::uint16_t value;
	};

	struct Component_info
	{
		Component_ID id;
		Component_size size;
	};

	class Component_group
	{
	public:

		using Index = Component_group_index;
		using Element_moved = Component_group_element_moved;


		Component_group(gsl::span<Component_info> component_infos, std::size_t capacity_per_chunk) :
			m_size{ 0 },
			m_size_of_single_element{ calculate_size_of_single_element(component_infos) },
			m_capacity_per_chunk{ capacity_per_chunk },
			m_first_chunk_not_full{ 0 },
			m_chunks{},
			m_component_offsets{ create_component_offsets(component_infos, m_capacity_per_chunk) }
		{
		}


		std::size_t size() const
		{
			return m_size;
		}

		void reserve(std::size_t const new_capacity)
		{
			std::size_t const number_of_chunks = new_capacity / m_capacity_per_chunk
				+ (new_capacity % m_capacity_per_chunk > 0 ? 1 : 0);

			m_chunks.reserve(number_of_chunks);

			while (m_chunks.size() < number_of_chunks)
			{
				Memory_chunk& chunk = m_chunks.emplace_back();
				chunk.resize(m_capacity_per_chunk * m_size_of_single_element, std::byte{});
			}
		}

		std::size_t capacity() const
		{
			return m_chunks.size() * m_capacity_per_chunk;
		}

		void shrink_to_fit()
		{
			std::size_t const ideal_number_of_chunks = size() / m_capacity_per_chunk
				+ (size() % m_capacity_per_chunk > 0 ? 1 : 0);

			m_chunks.erase(m_chunks.begin() + ideal_number_of_chunks, m_chunks.end());
			
			m_chunks.shrink_to_fit();
		}

		template <typename... Component>
		Element_moved erase(Index index)
		{
			std::tuple<Component...> const components_data_at_back = back<Component...>();
			std::apply(
				[&](Component... components_data) { set_components_data<Component...>(index, std::forward<Component>(components_data)...); },
				components_data_at_back
			);

			return { std::get<0>(components_data_at_back) }; // TODO assuming that entity is first component
		}

		template <typename... Component>
		Index push_back(Component... component)
		{
			if (size() == capacity())
			{
				reserve(capacity() + m_capacity_per_chunk);
			}

			std::size_t const chunk_index = m_first_chunk_not_full;
			Memory_chunk& chunk_to_push = m_chunks[chunk_index];
			
			std::size_t const entity_index = m_size % m_capacity_per_chunk;
			set_components_data_impl(chunk_to_push, entity_index, std::forward<Component>(component)...);
			++m_size;

			if (chunk_to_push.size() == m_capacity_per_chunk)
			{
				++m_first_chunk_not_full;
			}

			return { m_size - 1 };
		}

		void pop_back()
		{
			assert(size() > 0);

			Memory_chunk const& chunk_to_pop = m_chunks[m_first_chunk_not_full];

			if (m_first_chunk_not_full > 0 && m_size % m_capacity_per_chunk == 1)
			{
				--m_first_chunk_not_full;
			}

			--m_size;
		}

		template <typename... Component>
		std::tuple<Component...> back() const
		{
			return get_components_data<Component...>({ m_size - 1 });
		}

		template <typename Component>
		Component get_component_data(Index index) const
		{
			std::size_t const chunk_index = index.value / m_capacity_per_chunk;
			Memory_chunk const& chunk = m_chunks[chunk_index];

			std::size_t const entity_index = index.value % m_capacity_per_chunk;

			const Component_ID component_id = std::remove_reference_t<Component>::ID();
			auto component_offset = std::find_if(m_component_offsets.begin(), m_component_offsets.end(),
				[&](Component_offset const& component_offset) -> bool {  return component_offset.id == component_id; });

			std::size_t const component_index = component_offset->offset + entity_index;
			return reinterpret_cast<Component const&>(chunk[component_index]);
		}

		template <typename... Component>
		std::tuple<Component...> get_components_data(Index index) const
		{
			std::size_t const chunk_index = index.value / m_capacity_per_chunk;
			Memory_chunk const& chunk = m_chunks[chunk_index];

			std::size_t const entity_index = index.value % m_capacity_per_chunk;
			return get_components_data_impl<Component...>(chunk, entity_index);
		}

		template <typename Component>
		void set_component_data(Index index, Component&& component)
		{
			std::size_t const chunk_index = index.value / m_capacity_per_chunk;
			Memory_chunk& chunk = m_chunks[chunk_index];

			std::size_t const entity_index = index.value % m_capacity_per_chunk;

			const Component_ID component_id = std::remove_reference_t<Component>::ID();
			auto component_offset = std::find_if(m_component_offsets.begin(), m_component_offsets.end(),
				[&](Component_offset const& component_offset) -> bool {  return component_offset.id == component_id; });

			std::size_t const component_index = component_offset->offset + entity_index;
			reinterpret_cast<Component&>(chunk[component_index]) = std::forward<Component>(component);
		}

		template <typename... Component>
		void set_components_data(Index index, Component&&... component)
		{
			// Pre-condition: components in order

			std::size_t const chunk_index = index.value / m_capacity_per_chunk;
			Memory_chunk& chunk = m_chunks[chunk_index];

			std::size_t const entity_index = index.value % m_capacity_per_chunk;
			set_components_data_impl(chunk, entity_index, std::forward<Component>(component)...);
		}


	private:

		using Memory_chunk = std::vector<std::byte>;

		struct Memory_chunk_index
		{
			std::size_t value;
		};

		struct Component_offset
		{
			Component_ID id;
			std::size_t offset;
		};

		static std::size_t calculate_size_of_single_element(gsl::span<Component_info> const component_infos)
		{
			std::size_t size_of_single_element{ 0 };

			for (Component_info const& component_info : component_infos)
			{
				size_of_single_element += component_info.size.value;
			}

			return size_of_single_element;
		}

		static std::vector<Component_offset> create_component_offsets(gsl::span<Component_info> const component_infos, std::size_t const capacity_per_chunk)
		{
			std::vector<Component_offset> component_offsets;
			component_offsets.reserve(component_infos.size());

			std::size_t current_offset{ 0 };

			for (Component_info const& component_info : component_infos)
			{
				component_offsets.push_back({ component_info.id, { current_offset } });

				current_offset += capacity_per_chunk * component_info.size.value;
			}

			return component_offsets;
		}

		template <typename Component>
		Component get_component_data_impl(Memory_chunk const& chunk, std::size_t entity_index, std::size_t component_type_index) const
		{
			std::size_t const component_offset = m_component_offsets[component_type_index].offset;

			std::size_t const component_index = component_offset + entity_index;
			return reinterpret_cast<std::remove_const_t<std::remove_reference_t<Component>> const&>(chunk[component_index]);
		}

		template <typename... Component, std::size_t... Index>
		std::tuple<Component...> get_components_data_impl(Memory_chunk const& chunk, std::size_t entity_index, std::index_sequence<Index...>) const
		{
			return std::make_tuple(get_component_data_impl<Component>(chunk, entity_index, Index)...);
		}

		template <typename... Component, typename Indices = std::make_index_sequence<sizeof...(Component)>>
		std::tuple<Component...> get_components_data_impl(Memory_chunk const& chunk, std::size_t entity_index) const
		{
			return get_components_data_impl<Component...>(chunk, entity_index, Indices{});
		}

		template <typename Component>
		void set_component_data_impl(Memory_chunk& chunk, std::size_t entity_index, std::size_t component_type_index, Component&& component)
		{
			std::size_t const component_offset = m_component_offsets[component_type_index].offset;

			std::size_t const component_index = component_offset + entity_index;

			using Pointer = std::remove_const_t<std::remove_reference_t<Component>>*;
			Pointer pointer = reinterpret_cast<Pointer>(&chunk[component_index]);

			*pointer = std::forward<Component>(component);
			int i = 0;
		}

		template <typename... Component, std::size_t... Index>
		void set_components_data_impl(Memory_chunk& chunk, std::size_t entity_index, std::index_sequence<Index...>, Component&&... component)
		{
			(set_component_data_impl(chunk, entity_index, Index, std::forward<Component>(component)), ...);
		}

		template <typename... Component, typename Indices = std::make_index_sequence<sizeof...(Component)>>
		void set_components_data_impl(Memory_chunk& chunk, std::size_t entity_index, Component&&... component)
		{
			set_components_data_impl(chunk, entity_index, Indices{}, std::forward<Component>(component)...);
		}


		std::size_t m_size;
		std::size_t m_size_of_single_element;
		std::size_t m_capacity_per_chunk;
		std::size_t m_first_chunk_not_full;
		std::vector<Memory_chunk> m_chunks;
		std::vector<Component_offset> m_component_offsets;

	};

	template <typename... Component>
	Component_group make_component_group(std::size_t capacity_per_chunk)
	{
		std::array<Component_info, sizeof...(Component)> component_infos
		{
			Component_info { Component::ID(), { sizeof(Component) } }...
		};
		// TODO assert that every component ID is different

		return { component_infos, capacity_per_chunk };
	}
}

#endif
