#pragma once
#include "DataTypes.h"
#include "Actor.h"

namespace reality
{
	class FX_BaseEffectActor : public Actor
	{
	public:
		virtual void OnInit(entt::registry& registry, AABBShape collision_box) override;
		virtual void OnUpdate(entt::registry& registry) override;
	};
}


