#ifndef MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED

#include <bitset>

namespace Maia::GameEngine
{
	struct Entity_type
	{
		std::bitset<64> const mask;
	};
}

#endif
