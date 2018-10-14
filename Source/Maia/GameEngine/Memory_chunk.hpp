#ifndef MAIA_GAMEENGINE_MEMORYCHUNK_H_INCLUDED
#define MAIA_GAMEENGINE_MEMORYCHUNK_H_INCLUDED

#include <array>
#include <cstdint>
#include <tuple>
#include <typeindex>

namespace Maia::GameEngine
{
	template<std::size_t N, typename... Types>
	class Components_arrays_tuple;

	template<std::size_t N, typename This>
	class Components_arrays_tuple<N, This>
	{
	public:
		
		using component_type = This;
		using array_type = std::array<component_type, N>;

		array_type m_components; // TODO make private
	};

	template <std::size_t N, typename This, typename ...Rest>
	class Components_arrays_tuple<N, This, Rest...> :
		private Components_arrays_tuple<N, Rest...>
	{
	public:

		using component_type = This;
		using array_type = std::array<component_type, N>;

		array_type m_components;  // TODO make private
	};

	template <std::size_t Index, typename Components_arrays_tuple_>
	struct Array_type_of_tuple;

	template <std::size_t Index, std::size_t N>
	struct Array_type_of_tuple<Index, Components_arrays_tuple<N>>
	{
	};

	template <std::size_t N, typename This, typename... Rest>
	struct Array_type_of_tuple<0, Components_arrays_tuple<N, This, Rest...>>
	{
		using tuple_type = Components_arrays_tuple<N, This, Rest...>;
		using array_type = typename tuple_type::array_type;
	};

	template <std::size_t Index, std::size_t N, typename This, typename... Rest>
	struct Array_type_of_tuple<Index, Components_arrays_tuple<N, This, Rest...>> :
		public Array_type_of_tuple<Index - 1, Components_arrays_tuple<N, Rest...>>
	{
	};

	template <std::size_t Index, std::size_t N, typename... Types>
	using Array_type_of_tuple_t = typename Array_type_of_tuple<Index, Components_arrays_tuple<N, Types...>>::array_type;

	template <std::size_t Index, std::size_t N, typename... Types>
	constexpr Array_type_of_tuple_t<Index, N, Types...>& get(Components_arrays_tuple<N, Types...>& tuple)
	{
		typedef typename Array_type_of_tuple<Index, Components_arrays_tuple<N, Types...>>::tuple_type _Ttype;
		return static_cast<_Ttype&>(tuple).m_components;
	}

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
			m_components{}
		{
		}

		Index push_back(Components... components)
		{
			// TODO
			return { 0 };
		}

		std::tuple<Components...> pop_back()
		{
			// TODO
			return {};
		}

		std::tuple<Components...> get_components_data(Index index) const
		{
			// TODO
			return {};
		}

		void set_components_data(Index index, Components... components)
		{
			// TODO
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

		std::array<std::type_index, Memory_chunk::number_of_component_types()> m_components_order;
		std::uint16_t m_size;
		Components_arrays_tuple<Memory_chunk::capacity(), Components...> m_components;
	};
}

#endif
