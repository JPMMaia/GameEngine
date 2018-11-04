#ifndef MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITYTYPE_H_INCLUDED

#include <cstddef>
#include <ostream>

namespace Maia::GameEngine
{
	struct Entity_type_id
	{
		std::size_t value;
	};

	bool operator==(Entity_type_id lhs, Entity_type_id rhs)
	{
		return lhs.value == rhs.value;
	}
	bool operator!=(Entity_type_id lhs, Entity_type_id rhs)
	{
		return !(lhs == rhs);
	}

	std::ostream& operator<<(std::ostream& output_stream, Entity_type_id entity_type_id)
	{
		output_stream << entity_type_id.value;

		return output_stream;
	}


	template <typename... ComponentsT>
	struct Entity_type
	{
		Entity_type_id id;
	};

	template <typename... Components>
	bool operator==(Entity_type<Components...> lhs, Entity_type<Components...> rhs)
	{
		return lhs.id == rhs.id;
	}

	template <typename... Components>
	bool operator!=(Entity_type<Components...> lhs, Entity_type<Components...> rhs)
	{
		return !(lhs == rhs);
	}

	template <typename... Components>
	std::ostream& operator<<(std::ostream& output_stream, Entity_type<Components...> entity_type)
	{
		output_stream << entity_type.id;
		
		return output_stream;
	}
}

#endif
