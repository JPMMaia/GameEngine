#ifndef MAIA_GAMEENGINE_MEMORYCHUNK_H_INCLUDED
#define MAIA_GAMEENGINE_MEMORYCHUNK_H_INCLUDED

#include <array>
#include <cassert>
#include <cstddef>

namespace Maia::GameEngine
{
	struct Memory_chunk_index
	{
		std::size_t value;
	};
	
	template <std::size_t Capacity>
	class Memory_chunk
	{
	public:

		using Index = Memory_chunk_index;

		template <typename Data_type>
		Data_type get_data(Index index) const
		{
			return reinterpret_cast<Data_type const&>(m_storage[index.value]);
		}

		template <typename Data_type>
		void set_data(Index index, Data_type&& data)
		{
			reinterpret_cast<Data_type const&>(m_storage[index.value]) = std::forward<Data_type>(data);
		}

		static constexpr std::size_t capacity()
		{
			return Capacity;
		}

	private:

		std::array<std::byte, Capacity> m_storage;

	};
}

#endif
