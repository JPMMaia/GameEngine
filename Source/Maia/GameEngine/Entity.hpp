#ifndef MAIA_GAMEENGINE_ENTITY_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITY_H_INCLUDED

#include <cstdint>

namespace Maia::GameEngine
{
	struct Entity
	{
		using ID = std::uint32_t;

		ID const id;
	};
}

#endif
