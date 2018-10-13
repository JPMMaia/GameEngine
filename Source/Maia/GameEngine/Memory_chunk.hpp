#ifndef MAIA_GAMEENGINE_MEMORYCHUNK_H_INCLUDED
#define MAIA_GAMEENGINE_MEMORYCHUNK_H_INCLUDED

#include <array>
#include <cstdint>
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

	private:

		static constexpr std::size_t number_of_component_types()
		{
			return sizeof...(Components);
		}

		template <typename Current>
		static constexpr std::size_t single_element_size()
		{
			return sizeof(Current);
		}

		template <typename Current, typename ...Rest>
		static constexpr std::size_t single_element_size()
		{
			

			return sizeof(Current) + single_element_size<Rest...>();
		}

		static constexpr std::size_t number_of_elements()
		{
			constexpr std::size_t chunk_size = 65536;
			
			return chunk_size / single_element_size<Components>();
		}

		std::array<std::type_index, sizeof...(Components)> m_components_order;
		Components_arrays_tuple<65536 / single_element_size<Components...>(), Components...> m_components;
	};
}

#endif
