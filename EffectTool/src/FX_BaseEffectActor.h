#pragma once
#include "DataTypes.h"
#include "Actor.h"

namespace KGCA41B
{
	class FX_BaseEffectActor : public Actor
	{
	public:
		virtual void OnInit(entt::registry& registry, AABBShape collision_box) override;
		virtual void OnUpdate(entt::registry& registry) override;
	};
}


