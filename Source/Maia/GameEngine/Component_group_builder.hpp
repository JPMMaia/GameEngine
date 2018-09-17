#ifndef MAIA_GAMEENGINE_COMPONENTGROUPBUILDER_H_INCLUDED
#define MAIA_GAMEENGINE_COMPONENTGROUPBUILDER_H_INCLUDED

#include <Maia/GameEngine/Component_group.hpp>

namespace Maia::GameEngine
{
	class Component_group_mask
	{
	public:


	};

	class Component_group_builder
	{
	public:

		template <class ComponentType>
		void add_component()
		{
			// TODO calculate size of component
			// TODO get component type ID and hash to build a Component_group_mask
		}

		Component_group build() const;

	private:

		

	};
}

#endif
