#include "FX_Effect.h"
#include "Components.h"
#include "TimeMgr.h"

using namespace KGCA41B;

void FX_Effect::OnInit(entt::registry& registry, AABBShape collision_box, C_Effect effect)
{
	FX_BaseEffectActor::OnInit(registry, collision_box);

	effect.local = XMMatrixIdentity();
	auto& effect_comp = registry.emplace<C_Effect>(entity_id_, effect);

	transform_tree_.AddNodeToNode(TYPE_ID(KGCA41B::C_Transform), TYPE_ID(KGCA41B::C_Effect));
	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}

void FX_Effect::OnUpdate(entt::registry& registry)
{
	FX_BaseEffectActor::OnUpdate(registry);
	
}

void FX_Effect::AddEmitter(entt::registry& registry, shared_ptr<Emitter> emitter)
{
	auto& effect = registry.get<C_Effect>(entity_id_);
	effect.emitters.push_back(emitter);
}

void KGCA41B::FX_Effect::ResetEmitter(entt::registry& registry)
{
	auto& effect = registry.get<C_Effect>(entity_id_);
	effect.emitters.clear();
}
