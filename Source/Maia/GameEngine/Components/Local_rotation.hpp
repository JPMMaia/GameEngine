#ifndef MAIA_GAMEENGINE_LOCALROTATION_H_INCLUDED
#define MAIA_GAMEENGINE_LOCALROTATION_H_INCLUDED

#include <Eigen/Geometry>

#include <Maia/GameEngine/Entity.hpp>

namespace Maia::GameEngine::Components
{
	struct Local_rotation
	{
		Eigen::Quaternionf value{ 1.0f, 0.0f, 0.0f, 0.0f };

		static Component_ID ID()
		{
			return { 2 };
		}
	};
}

#endif