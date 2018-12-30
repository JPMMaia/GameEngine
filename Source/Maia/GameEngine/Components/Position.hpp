#ifndef MAIA_GAMEENGINE_POSITION_H_INCLUDED
#define MAIA_GAMEENGINE_POSITION_H_INCLUDED

#include <Eigen/Core>

#include <Maia/GameEngine/Entity.hpp>

namespace Maia::GameEngine::Components
{
	struct Position
	{
		Eigen::Vector3f value{ 0.0f, 0.0f, 0.0f };

		static Component_ID ID()
		{
			return { 1 };
		}
	};
}

#endif