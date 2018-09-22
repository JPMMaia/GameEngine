#ifndef MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED

#include <bitset>

namespace Maia::GameEngine
{
	class Entity_type
	{
		std::bitset<64> m_mask{};

		friend class Entity_manager;
	};
}

#endif 