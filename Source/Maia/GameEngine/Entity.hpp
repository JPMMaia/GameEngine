#ifndef MAIA_GAMEENGINE_ENTITY_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITY_H_INCLUDED

#include <cstdint>

namespace Maia::GameEngine
{
	class Entity
	{
		friend class EntityManager;

		std::uint32_t m_id;
	};
}

#endif
