#ifndef MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED

#include <cstddef>
#include <iosfwd>

namespace Maia::GameEngine
{
	struct Entity_type_id
	{
		std::size_t value;
	};

	inline bool operator==(Entity_type_id lhs, Entity_type_id rhs)
	{
		return lhs.value == rhs.value;
	}
	inline bool operator!=(Entity_type_id lhs, Entity_type_id rhs)
	{
		return !(lhs == rhs);
	}

	std::ostream& operator<<(std::ostream& output_stream, Entity_type_id entity_type_id);
}

#endif
