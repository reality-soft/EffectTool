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
}

void KGCA41B::FX_Emitter::OnUpdate(entt::registry& registry)
{
	FX_BaseEffectActor::OnUpdate(registry);

	auto& emitter = registry.get<Emitter>(entity_id_);

	emitter.timer += TIMER->GetDeltaTime();

	// 파티클 lifetime 계산
	for (auto& particle : emitter.particles)
	{
		// 파티클 타이머 체크 후 수명이 다한 파티클은 enable
		particle.timer += TIMER->GetDeltaTime();
		if (particle.timer > particle.lifetime)
		{
			particle.enable = false;
			continue;
		}

		particle.frame_ratio = particle.timer / particle.lifetime;

		if (particle.frame_ratio < 0.3f)
		{
			particle.color.w = 1.0f / 0.3f * particle.frame_ratio;
		}
		else if (particle.frame_ratio > 0.3f)
		{
			particle.color.w = 1.0f - 1.0f / 0.7f * (particle.frame_ratio - 0.3f);
		}
			
		XMStoreFloat3(&particle.scale, XMVectorAdd(XMLoadFloat3(&particle.scale), TIMER->GetDeltaTime() * XMLoadFloat3(&particle.add_size)));
		XMStoreFloat3(&particle.position,	XMVectorAdd(XMLoadFloat3(&particle.position),	TIMER->GetDeltaTime() * XMLoadFloat3(&particle.add_velocity)));

		// 크기는 현재 x값으로만 계산
		particle.scale.x = particle.scale.x;
		particle.scale.y = particle.scale.x;
		particle.scale.z = particle.scale.x;
		particle.rotation.z += TIMER->GetDeltaTime() * particle.add_rotation;
			
	}

	// 1초마다 파티클 생성
	if (emitter.timer > 1.0f)
	{
		emitter.timer -= 1.0f;

		for (int i = 0; i < emitter.emit_per_second; i++)
		{
			emitter.particles.push_back({});
			auto& particle = emitter.particles[emitter.particles.size() - 1];

			// 파티클 당 버퍼 생성
			particle.CreateBuffer();

			// 세부 설정
			particle.enable = true;

			particle.timer = 0.0f;
			particle.lifetime = randstep(emitter.life_time[0], emitter.life_time[1]);
			particle.frame_ratio = 0.0f;

			particle.color = emitter.color;

			particle.position = { 
				randstep(emitter.initial_position[0].x, emitter.initial_position[1].x),
				randstep(emitter.initial_position[0].y, emitter.initial_position[1].y),
				randstep(emitter.initial_position[0].z, emitter.initial_position[1].z) };
			particle.rotation = {0, 0, randstep(-360.0f, 360.0f)};
			// 크기는 현재 x값으로만 계산
			particle.scale = {
				randstep(emitter.initial_size[0].x, emitter.initial_size[1].x),
				randstep(emitter.initial_size[0].x, emitter.initial_size[1].x),
				randstep(emitter.initial_size[0].z, emitter.initial_size[1].z) };

			particle.add_velocity = {
				randstep(emitter.velocity_per_lifetime[0].x, emitter.velocity_per_lifetime[1].x),
				randstep(emitter.velocity_per_lifetime[0].y, emitter.velocity_per_lifetime[1].y),
				randstep(emitter.velocity_per_lifetime[0].z, emitter.velocity_per_lifetime[1].z) };

			particle.add_size = {
				randstep(emitter.size_per_lifetime[0].x, emitter.size_per_lifetime[1].x),
				randstep(emitter.size_per_lifetime[0].y, emitter.size_per_lifetime[1].y),
				randstep(emitter.size_per_lifetime[0].z, emitter.size_per_lifetime[1].z) };

			particle.add_rotation = randstep(emitter.rotation_per_lifetime[0], emitter.rotation_per_lifetime[1]);
		}

	}
}
