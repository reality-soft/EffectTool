#pragma once
#include "FX_BaseEffectActor.h"

namespace KGCA41B {
	class FX_Effect : public FX_BaseEffectActor
	{
	public:
		virtual void OnInit(entt::registry& registry, AABBShape collision_box, string effect_id = "");
		virtual void OnUpdate(entt::registry& registry);
	public:
		void AddEffect(entt::registry& registry, map<string, Emitter>& emitter_list);
		void ResetEmitter(entt::registry& registry);
	};
}