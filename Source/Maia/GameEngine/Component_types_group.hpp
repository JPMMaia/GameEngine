#ifndef MAIA_GAMEENGINE_COMPONENTTYPESGROUP_H_INCLUDED
#define MAIA_GAMEENGINE_COMPONENTTYPESGROUP_H_INCLUDED

#include <bitset>
#include <ostream>

namespace Maia::GameEngine
{
    struct Component_types_group
	{
		using Mask = std::bitset<64>;

		Mask mask;

		template <typename... Component>
		bool contains() const
		{
			Mask include_mask;
			(include_mask.set(Component::ID().value), ...);

			return (mask & include_mask) == include_mask;
		}
	};

	bool operator==(Component_types_group lhs, Component_types_group rhs)
	{
		return lhs.mask == rhs.mask;
	}
	bool operator!=(Component_types_group lhs, Component_types_group rhs)
	{
		return !(lhs == rhs);
	}

	std::ostream& operator<<(std::ostream& output_stream, Component_types_group value)
	{
		output_stream << value.mask;

		return output_stream;
	}

	template <typename... Components>
	Component_types_group make_component_types_group()
	{
		Component_types_group component_types_group;

		(component_types_group.mask.set(Components::ID().value), ...);

		return component_types_group;
	}
}

#endif