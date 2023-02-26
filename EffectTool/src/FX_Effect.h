#pragma once
#include "FX_BaseEffectActor.h"

namespace KGCA41B {
	class FX_Effect : public FX_BaseEffectActor
	{
	public:
		virtual void OnInit(entt::registry& registry, AABBShape collision_box, Effect effect);
		virtual void OnUpdate(entt::registry& registry);
	public:
		void AddEmitter(entt::registry& registry, Emitter emitter);
	};
}