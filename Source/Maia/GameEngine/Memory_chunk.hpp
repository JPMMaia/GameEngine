#ifndef MAIA_GAMEENGINE_MEMORYCHUNK_H_INCLUDED
#define MAIA_GAMEENGINE_MEMORYCHUNK_H_INCLUDED

#include <array>
#include <cassert>
#include <cstdint>
#include <tuple>
#include <typeindex>

namespace Maia::GameEngine
{
	template <typename ...Components>
	class Memory_chunk
	{
	public:

		struct Index
		{
			std::uint16_t value;
		};

		Memory_chunk() :
			m_components_order{ (std::type_index(typeid(Components)))... },
			m_components{},
			m_size{ 0 }
		{
		}

		Index push_back(Components... components)
		{
			assert(size() < capacity());

			constexpr auto number_of_component_types = Memory_chunk::number_of_component_types();
			set_components_data_impl({ m_size }, components..., std::make_index_sequence<number_of_component_types>{});

			return { m_size++ };
		}

		std::tuple<Components...> pop_back()
		{
			assert(size() > 0);

			constexpr auto number_of_component_types = Memory_chunk::number_of_component_types();
			return get_components_data_impl({ --m_size }, std::make_index_sequence<number_of_component_types>{});
		}

		std::tuple<Components...> get_components_data(Index index) const
		{
			assert(index.value < size());

			constexpr auto number_of_component_types = Memory_chunk::number_of_component_types();
			return get_components_data_impl(index, std::make_index_sequence<number_of_component_types>{});
		}

		void set_components_data(Index index, Components... components)
		{
			assert(index.value < size());

			constexpr auto number_of_component_types = Memory_chunk::number_of_component_types();
			set_components_data_impl(index, components..., std::make_index_sequence<number_of_component_types>{});
		}

		template <class Component>
		Component get_component_data(Index index) const
		{
			// TODO
			return {};
		}

		template <class Component>
		void set_component_data(Index index, Component component)
		{
			// TODO
		}

		static constexpr std::size_t capacity()
		{
			constexpr std::size_t chunk_size = 65536;

			return chunk_size / single_element_size();
		}

		std::size_t size() const
		{
			return m_size;
		}

	private:

		static constexpr std::size_t number_of_component_types()
		{
			return sizeof...(Components);
		}

		static constexpr std::size_t single_element_size()
		{
			return (sizeof(Components) + ...);
		}

		template <std::size_t... I>
		std::tuple<Components...> get_components_data_impl(Index index, std::index_sequence<I...>) const
		{
			return { std::get<I>(m_components)[index.value]... };
		}

		template <std::size_t... I>
		void set_components_data_impl(Index index, Components... components, std::index_sequence<I...>)
		{
			((std::get<I>(m_components)[index.value] = components), ...);
		}

		std::array<std::type_index, Memory_chunk::number_of_component_types()> m_components_order;
		std::uint16_t m_size;
		std::tuple<std::array<Components, Memory_chunk::capacity()>...> m_components;
	};
}

#endif
