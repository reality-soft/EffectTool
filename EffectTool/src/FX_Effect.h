#pragma once
#include "FX_BaseEffectActor.h"

namespace KGCA41B {
	class FX_Effect : public FX_BaseEffectActor
	{
	public:
		virtual void OnInit(entt::registry& registry, AABBShape collision_box, C_Effect effect);
		virtual void OnUpdate(entt::registry& registry);
	public:
		void AddEmitter(entt::registry& registry, shared_ptr<Emitter> emitter);
		void ResetEmitter(entt::registry& registry);
	};
}