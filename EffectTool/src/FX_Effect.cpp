#include "FX_Effect.h"
#include "Components.h"
#include "TimeMgr.h"

using namespace reality;

void FX_Effect::OnInit(entt::registry& registry, AABBShape collision_box, string effect_id)
{
	FX_BaseEffectActor::OnInit(registry, collision_box);

	C_Effect initial_effect_comp;
	auto& effect_comp = registry.emplace<C_Effect>(entity_id_, initial_effect_comp);
	effect_comp.local = XMMatrixIdentity();

	effect_comp.effect_id = effect_id;

	if (effect_id != "")
	{
		effect_comp.effect = *RESOURCE->UseResource<Effect>(effect_id);
	}
	

	transform_tree_.AddNodeToNode(TYPE_ID(reality::C_Transform), TYPE_ID(reality::C_Effect));
	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}

void FX_Effect::OnUpdate(entt::registry& registry)
{
	FX_BaseEffectActor::OnUpdate(registry);
	
}

void FX_Effect::AddEffect(entt::registry& registry, map<string, Emitter>& emitter_list)
{
	auto& effect_comp = registry.get<C_Effect>(entity_id_);
	effect_comp.effect.emitters = emitter_list;
}

void FX_Effect::ResetEmitter(entt::registry& registry)
{
	auto& effect_comp = registry.get<C_Effect>(entity_id_);
	effect_comp.effect.emitters.clear();
}
