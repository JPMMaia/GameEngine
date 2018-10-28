#ifndef MAIA_GAMEENGINE_ENTITY_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITY_H_INCLUDED

#include <cstdint>
#include <ostream>

#include <Maia/GameEngine/Component.hpp>

namespace Maia::GameEngine
{
	struct Entity
	{
		std::uint32_t value {0};

		static Component_ID ID()
		{
			return { 0 };
		}
	};

	inline bool operator==(Entity const& lhs, Entity const& rhs)
	{
		return lhs.value == rhs.value;
	}

	inline bool operator!=(Entity const& lhs, Entity const& rhs)
	{
		return !(lhs == rhs);
	}

	inline std::ostream& operator<<(std::ostream& output_stream, Entity const& value)
	{
		output_stream << "{" << value.value << "}";
		return output_stream;
	}
}

#endif
