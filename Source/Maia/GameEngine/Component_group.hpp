#ifndef MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED
#define MAIA_GAMEENGINE_COMPONENTGROUP_H_INCLUDED

#include <array>
#include <memory>

namespace Maia::GameEngine
{
	class Component_group
	{
		struct MemoryChunk
		{
			std::array<std::byte, 0x4000> data;
			std::unique_ptr<MemoryChunk> next;
		};

		std::unique_ptr<MemoryChunk> m_first_chunk;
	};
}

#endif
