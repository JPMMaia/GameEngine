#ifndef MAIA_GAMEENGINE_COMPONENT_H_INCLUDED
#define MAIA_GAMEENGINE_COMPONENT_H_INCLUDED

#include <cstdint>

namespace Maia::GameEngine
{
	struct Component_ID
	{
		std::uint16_t value;
	};

	inline bool operator==(Component_ID lhs, Component_ID rhs)
	{
		return lhs.value == rhs.value;
	}

	inline bool operator!=(Component_ID lhs, Component_ID rhs)
	{
		return !(lhs == rhs);
	}

	struct Component_size
	{
		std::uint16_t value;
	};

	struct Component_info
	{
		Component_ID id;
		Component_size size;
	};
}

#endif
