#ifndef MAIA_GAMEENGINE_ENTITY_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITY_H_INCLUDED

#include <cstdint>
#include <ostream>

namespace Maia::GameEngine
{
	struct Entity
	{
		using ID = std::uint32_t;

		static constexpr ID empty = 0;

		ID const id { empty };
	};

	inline bool operator==(Entity const& lhs, Entity const& rhs)
	{
		return lhs.id == rhs.id;
	}

	inline bool operator!=(Entity const& lhs, Entity const& rhs)
	{
		return !(lhs == rhs);
	}

	inline std::ostream& operator<<(std::ostream& output_stream, Entity const& value)
	{
		output_stream << "{" << value.id << "}";
		return output_stream;
	}
}

#endif
