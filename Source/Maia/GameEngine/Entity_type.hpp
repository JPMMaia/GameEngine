#ifndef MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED

#include <cstddef>
#include <ostream>

namespace Maia::GameEngine
{
	struct Entity_type
	{
		std::size_t id;
	};

	bool operator==(Entity_type lhs, Entity_type rhs)
	{
		return lhs.id == rhs.id;
	}
	bool operator!=(Entity_type lhs, Entity_type rhs)
	{
		return !(lhs == rhs);
	}

	std::ostream& operator<<(std::ostream& output_stream, Entity_type entity_type)
	{
		output_stream << entity_type.id;
		
		return output_stream;
	}
}

#endif
