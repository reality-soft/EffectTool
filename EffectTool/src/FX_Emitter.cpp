#include "FX_Emitter.h"
#include "Components.h"
#include "TimeMgr.h"

using namespace KGCA41B;

void FX_Emitter::OnInit(entt::registry& registry, AABBShape collision_box, Emitter emitter)
{
	FX_BaseEffectActor::OnInit(registry, collision_box);

	emitter.local = XMMatrixIdentity();
	auto& emitter_comp = registry.emplace<Emitter>(entity_id_, emitter);

	transform_tree_.AddNodeToNode(TYPE_ID(KGCA41B::Transform), TYPE_ID(KGCA41B::Emitter));
	transform_tree_.root_node->OnUpdate(registry, entity_id_);


	// 파티클 생성
	emitter_comp.particles.resize(emitter_comp.particle_count);
	for (auto& particle : emitter_comp.particles)
	{
		particle.CreateBuffer();
		// 세부 설정
		particle.velocity = { 0.0f, 1.0f, 0.0f };
	}
}

void KGCA41B::FX_Emitter::OnUpdate(entt::registry& registry)
{
	FX_BaseEffectActor::OnUpdate(registry);

	auto& emitter = registry.get<Emitter>(entity_id_);

	emitter.timer += TIMER->GetDeltaTime();

	for (auto& particle : emitter.particles)
	{
		particle.timer = emitter.timer;
		if (particle.timer > particle.lifetime)
			particle.enable = false;
	}
}
