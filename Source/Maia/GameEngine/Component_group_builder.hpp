#ifndef MAIA_GAMEENGINE_COMPONENTGROUPBUILDER_H_INCLUDED
#define MAIA_GAMEENGINE_COMPONENTGROUPBUILDER_H_INCLUDED

#include <array>
#include <cstdint>

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
			m_component_group_hash += std::hash(ComponentType::ID());
			m_components_sizes[m_component_count++] = sizeof(ComponentType);
		}

		Component_group build() const;

	private:

		std::uint16_t m_component_group_hash;
		std::array<std::uint16_t, 16> m_components_sizes;
		std::size_t m_component_count;

	};
}

#endif
