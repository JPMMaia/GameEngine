#ifndef MAIA_GAMEENGINE_ENTITY_H_INCLUDED
#define MAIA_GAMEENGINE_ENTITY_H_INCLUDED

#include <cstdint>
#include <functional>
#include <ostream>

#include <Maia/GameEngine/Component.hpp>

namespace Maia::GameEngine
{
	struct Entity
	{
		using Integral_type = std::uint32_t;

		Integral_type value {0};


		static Component_ID ID()
		{
			return { 0 };
		}
	};

	inline bool operator==(Entity const& lhs, Entity const& rhs)
	{
		return lhs.value == rhs.value;
	}

	inline bool operator!=(Entity const& lhs, Entity const& rhs)
	{
		return !(lhs == rhs);
	}

	inline std::ostream& operator<<(std::ostream& output_stream, Entity const& value)
	{
		output_stream << "{" << value.value << "}";
		return output_stream;
	}
}

namespace std
{
	template<> struct hash< Maia::GameEngine::Entity>
	{
		using argument_type = Maia::GameEngine::Entity;
		using result_type = std::size_t;
		
		result_type operator()(argument_type const& entity) const noexcept
		{
			return std::hash<Maia::GameEngine::Entity::Integral_type>{}(entity.value);
		}
	};
}

#endif
