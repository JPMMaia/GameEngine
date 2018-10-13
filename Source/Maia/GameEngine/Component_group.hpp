#ifndef MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED
#define MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED

#include <array>
#include <memory>
#include <numeric>
#include <vector>

#include <gsl/span>

#include <Maia/GameEngine/Entity.hpp>

namespace Maia::GameEngine
{
	class Component_group
	{
	public:
		
		template <class T>
		T get_component_data(Entity entity) const
		{
			return {};
		}

		template <class T>
		void set_component_data(Entity entity, T&& data)
		{
		}

	private:

		struct Memory_chunk
		{
			std::array<std::byte, 0x4000> data;
			std::unique_ptr<Memory_chunk> next;
		};

		std::size_t get_base_component_offset(gsl::span<std::uint16_t> const component_sizes_in_bytes, std::size_t const component_index)
		{
			return std::accumulate(
				component_sizes_in_bytes.begin(),
				component_sizes_in_bytes.begin() + component_index,
				std::size_t{ 0 }
			);	
		}

		std::size_t get_entry_offset(std::size_t const base_component_offset, std::size_t const size_of_component, std::size_t const entry_index) const
		{
			return base_component_offset + entry_index * size_of_component;
		}

		std::size_t num_entries_per_chunk() const
		{
			// TODO
			return {};
		}

		std::size_t get_chunk_index(std::size_t const entry_index) const
		{
			return entry_index % (num_entries_per_chunk() - 1);
		}

		Memory_chunk& get_chunk(std::size_t const chunk_index)
		{
			Memory_chunk* current_chunk = m_first_chunk.get();

			for (std::size_t current_chunk_index = 0; current_chunk_index < chunk_index; ++current_chunk_index)
			{
				current_chunk = current_chunk->next.get();
			}

			return *current_chunk;
		}

		template <class T>
		void set_data(Memory_chunk& chunk, std::size_t const offset, T&& data)
		{
			std::memcpy(&chunk.data[offset], &data, sizeof(T));
		}
		
		std::vector<std::uint16_t> m_component_sizes;
		std::unique_ptr<Memory_chunk> m_first_chunk;

		friend class Entity_manager;
	};
}

#endif
