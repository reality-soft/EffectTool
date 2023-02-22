#pragma once
#include "FX_BaseEffectActor.h"

namespace KGCA41B {
	class FX_Emitter : public FX_BaseEffectActor
	{
	public:
		virtual void OnInit(entt::registry& registry, AABBShape collision_box, Emitter emitter);
		virtual void OnUpdate(entt::registry& registry);
	};
}