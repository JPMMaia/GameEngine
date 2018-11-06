#ifndef MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED
#define MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED

#include <algorithm>
#include <cassert>
#include <numeric>

#include <gsl/span>

#include <Maia/GameEngine/Component.hpp>
#include <Maia/GameEngine/Entity.hpp>

namespace Maia::GameEngine
{
	class Components_chunk
	{
	public:

		template <typename Component>
		using Reference = std::remove_const_t<std::remove_reference_t<Component>>&;

		template <typename Component>
		using Const_reference = std::remove_const_t<std::remove_reference_t<Component>> const&;


		void resize(std::size_t count, std::byte value)
		{
			m_data.resize(count, value);
		}


		template <typename Component>
		Const_reference<Component> get_component_data(std::size_t component_index) const
		{
			return reinterpret_cast<Const_reference<Component>>(m_data[component_index]);
		}

		template <typename Component>
		Reference<Component> get_component_data(std::size_t component_index)
		{
			return reinterpret_cast<Reference<Component>>(m_data[component_index]);
		}

		template <typename Component>
		void set_component_data(std::size_t component_index, Component&& component)
		{
			reinterpret_cast<Reference<Component>>(m_data[component_index]) = std::forward<Component>(component);
		}


		template <typename Component>
		gsl::span<Component> components(std::size_t offset, std::ptrdiff_t count)
		{
			return { reinterpret_cast<Component*>(m_data.data() + offset), count };
		}

		template <typename Component>
		gsl::span<Component const> components(std::size_t offset, std::ptrdiff_t count) const
		{
			return { reinterpret_cast<Component const*>(m_data.data() + offset), count };
		}


	private:

		std::vector<std::byte> m_data;

	};

	struct Component_group_index
	{
		std::size_t value;
	};

	struct Component_group_element_moved
	{
		Entity entity;
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
				Components_chunk& chunk = m_chunks.emplace_back();
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

			decrement_size();

			return { get_component_data<Entity>(index) };
		}

		template <typename... Component>
		Index push_back(Component... component)
		{
			if (size() == capacity())
			{
				reserve(capacity() + m_capacity_per_chunk);
			}

			std::size_t const chunk_index = m_first_chunk_not_full;
			Components_chunk& chunk_to_push = m_chunks[chunk_index];
			
			Component_group_index component_group_index{ size() };
			std::size_t const entity_index = calculate_entity_index(component_group_index);

			set_components_data(chunk_to_push, entity_index, std::forward<Component>(component)...);

			increment_size();

			return { size() - 1 };
		}

		void pop_back()
		{
			decrement_size();
		}


		template <typename... Component>
		std::tuple<Component...> back() const
		{
			return get_components_data<Component...>({ m_size - 1 });
		}


		template <typename Component>
		Component get_component_data(Index index) const
		{
			Components_chunk const& chunk = get_entity_chunk(index);
			
			std::size_t const entity_index = calculate_entity_index(index);
			std::size_t const component_offset = get_component_offset<Component>();
			std::size_t const component_index = component_offset + entity_index;

			return get_component_data<Component>(chunk, component_index);
		}

		template <typename Component>
		void set_component_data(Index index, Component&& component)
		{
			Components_chunk& chunk = get_entity_chunk(index);
			
			std::size_t const entity_index = calculate_entity_index(index);
			std::size_t const component_offset = get_component_offset<Component>();
			std::size_t const component_index = component_offset + entity_index;

			set_component_data<Component>(chunk, component_index, std::forward<Component>(component));
		}


		template <typename... Component>
		std::tuple<Component...> get_components_data(Index index) const
		{
			Components_chunk const& chunk = get_entity_chunk(index);
			std::size_t const entity_index = calculate_entity_index(index);

			return get_components_data<Component...>(chunk, entity_index);
		}

		template <typename... Component>
		void set_components_data(Index index, Component&&... component)
		{
			// Pre-condition: components in order

			Components_chunk& chunk = get_entity_chunk(index);
			std::size_t const entity_index = calculate_entity_index(index);

			set_components_data(chunk, entity_index, std::forward<Component>(component)...);
		}

		// TODO maybe hide
		gsl::span<Components_chunk const> components_chunks() const
		{
			return m_chunks;
		}

		gsl::span<Components_chunk> components_chunks()
		{
			return m_chunks;
		}
		
		// TODO maybe pass an index instead of chunk
		template <typename Component>
		gsl::span<Component> components(Components_chunk& chunk)
		{
			std::size_t const component_offset = get_component_offset<Component>();
			// TODO check if it is the last chunk and pass correct size accordingly
			return chunk.components<Component>(component_offset, size()); // TODO wrong
		}

		template <typename Component>
		gsl::span<Component const> components(Components_chunk const& chunk) const
		{
			std::size_t const component_offset = get_component_offset<Component>();

			return chunk.components<Component>(component_offset, size());
		}

		
	private:

		template <typename Component>
		using Reference = std::remove_const_t<std::remove_reference_t<Component>>&;

		template <typename Component>
		using Const_reference = std::remove_const_t<std::remove_reference_t<Component>> const&;

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


		void increment_size()
		{
			++m_size;

			if (m_size % m_capacity_per_chunk == 0)
			{
				++m_first_chunk_not_full;
			}
		}
		void decrement_size()
		{
			assert(m_size > 0);

			if (m_first_chunk_not_full > 0 && m_size % m_capacity_per_chunk == 1)
			{
				--m_first_chunk_not_full;
			}
			
			--m_size;
		}

		Components_chunk const& get_entity_chunk(Component_group_index component_group_index) const
		{
			return m_chunks[component_group_index.value / m_capacity_per_chunk];
		}
		Components_chunk& get_entity_chunk(Component_group_index component_group_index)
		{
			return m_chunks[component_group_index.value / m_capacity_per_chunk];
		}

		std::size_t calculate_entity_index(Component_group_index component_group_index)  const
		{
			return component_group_index.value % m_capacity_per_chunk;
		}


		template <typename Component>
		std::size_t get_component_offset() const
		{
			Component_ID const component_id = std::remove_reference_t<Component>::ID();

			auto component_offset = std::find_if(m_component_offsets.begin(), m_component_offsets.end(),
				[&](Component_offset const& component_offset) -> bool {  return component_offset.id == component_id; });

			return component_offset->offset;
		}


		template <typename Component>
		Const_reference<Component> get_component_data(Components_chunk const& chunk, std::size_t component_index) const
		{
			return chunk.get_component_data<Component>(component_index);
		}

		template <typename Component>
		Const_reference<Component> get_component_data(Components_chunk const& chunk, std::size_t entity_index, std::size_t component_type_index) const
		{
			std::size_t const component_offset = m_component_offsets[component_type_index].offset;
			std::size_t const component_index = component_offset + entity_index;

			return get_component_data<Component>(chunk, component_index);
		}


		template <typename Component>
		void set_component_data(Components_chunk& chunk, std::size_t component_index, Component&& component)
		{
			chunk.set_component_data<Component>(component_index, std::forward<Component>(component));
		}

		template <typename Component>
		void set_component_data(Components_chunk& chunk, std::size_t entity_index, std::size_t component_type_index, Component&& component)
		{
			std::size_t const component_offset = m_component_offsets[component_type_index].offset;
			std::size_t const component_index = component_offset + entity_index;

			set_component_data<Component>(chunk, component_index, std::forward<Component>(component));
		}
		

		template <typename... Component, std::size_t... Index>
		std::tuple<Component...> get_components_data(Components_chunk const& chunk, std::size_t entity_index, std::index_sequence<Index...>) const
		{
			return std::make_tuple(get_component_data<Component>(chunk, entity_index, Index)...);
		}

		template <typename... Component, typename Indices = std::make_index_sequence<sizeof...(Component)>>
		std::tuple<Component...> get_components_data(Components_chunk const& chunk, std::size_t entity_index) const
		{
			return get_components_data<Component...>(chunk, entity_index, Indices{});
		}


		template <typename... Component, std::size_t... Index>
		void set_components_data(Components_chunk& chunk, std::size_t entity_index, std::index_sequence<Index...>, Component&&... component)
		{
			(set_component_data(chunk, entity_index, Index, std::forward<Component>(component)), ...);
		}

		template <typename... Component, typename Indices = std::make_index_sequence<sizeof...(Component)>>
		void set_components_data(Components_chunk& chunk, std::size_t entity_index, Component&&... component)
		{
			set_components_data(chunk, entity_index, Indices{}, std::forward<Component>(component)...);
		}


		std::size_t m_size;
		std::size_t m_size_of_single_element;
		std::size_t m_capacity_per_chunk;
		std::size_t m_first_chunk_not_full;
		std::vector<Components_chunk> m_chunks;
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
