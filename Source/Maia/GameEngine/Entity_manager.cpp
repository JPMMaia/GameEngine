#include "Entity_manager.hpp"

namespace Maia::GameEngine
{
	Entity_type_id Entity_manager::create_entity_type(
		std::size_t const capacity_per_chunk,
		gsl::span<Component_info const> const component_infos,
		Space const space
	)
	{
		Component_types_group const component_types_mask = [&component_infos]() -> Component_types_group
		{
			Component_types_group component_types_mask = {};

			for (Maia::GameEngine::Component_info const& component_info : component_infos)
			{
				component_types_mask.mask.set(component_info.id.value);
			}

			return component_types_mask;
		}();

		std::optional<std::size_t> const match_index = [this, &space, &component_types_mask]() -> std::optional<std::size_t>
		{
			for (std::size_t index = 0; index < m_entity_type_ids.size(); ++index)
			{
				if (m_component_types_spaces[index] == space && m_component_types_groups[index] == component_types_mask)
				{
					return index;
				}
			}

			return {};
		}();


		if (match_index)
		{
			return m_entity_type_ids[*match_index];
		}
		else
		{
			m_component_types_spaces.push_back(space);
			m_component_types_groups.push_back(component_types_mask);

			m_component_groups.emplace_back(component_infos, capacity_per_chunk);

			Entity_type_id const entity_type_id{ m_entity_type_ids.size() };
			m_entity_type_ids.push_back(entity_type_id);

			return entity_type_id;
		}
	}

}