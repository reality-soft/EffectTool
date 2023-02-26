#include "FX_Effect.h"
#include "Components.h"
#include "TimeMgr.h"

using namespace KGCA41B;

void FX_Effect::OnInit(entt::registry& registry, AABBShape collision_box, Effect effect)
{
	FX_BaseEffectActor::OnInit(registry, collision_box);

	effect.local = XMMatrixIdentity();
	auto& effect_comp = registry.emplace<Effect>(entity_id_, effect);

	transform_tree_.AddNodeToNode(TYPE_ID(KGCA41B::Transform), TYPE_ID(KGCA41B::Effect));
	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}

void FX_Effect::OnUpdate(entt::registry& registry)
{
	FX_BaseEffectActor::OnUpdate(registry);

	auto& effect = registry.get<Effect>(entity_id_);

	for (auto& emitter : effect.emitters)
	{
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

			XMVECTOR acc = TIMER->GetDeltaTime() * XMLoadFloat3(&particle.accelation);
			particle.add_velocity.x += acc.m128_f32[0];
			particle.add_velocity.y += acc.m128_f32[1];
			particle.add_velocity.z += acc.m128_f32[2];

			XMStoreFloat3(&particle.scale, XMVectorAdd(XMLoadFloat3(&particle.scale), TIMER->GetDeltaTime() * XMLoadFloat3(&particle.add_size)));
			XMStoreFloat3(&particle.position, XMVectorAdd(XMLoadFloat3(&particle.position), TIMER->GetDeltaTime() * XMLoadFloat3(&particle.add_velocity)));

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
				
				{
					// 버텍스 버퍼
					particle.vertex_list.push_back({ { -1.0f, +1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+0.0f, +0.0f} });
					particle.vertex_list.push_back({ { +1.0f, +1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+1.0f, +0.0f} });
					particle.vertex_list.push_back({ { -1.0f, -1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+0.0f, +1.0f} });
					particle.vertex_list.push_back({ { +1.0f, -1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+1.0f, +1.0f} });

					D3D11_BUFFER_DESC bufDesc;

					ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

					bufDesc.ByteWidth = sizeof(Vertex) * particle.vertex_list.size();
					bufDesc.Usage = D3D11_USAGE_DEFAULT;
					bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bufDesc.CPUAccessFlags = 0;
					bufDesc.MiscFlags = 0;
					bufDesc.StructureByteStride = 0;

					D3D11_SUBRESOURCE_DATA subResourse;

					ZeroMemory(&subResourse, sizeof(D3D11_SUBRESOURCE_DATA));

					subResourse.pSysMem = &particle.vertex_list.at(0);
					subResourse.SysMemPitch;
					subResourse.SysMemSlicePitch;

					DX11APP->GetDevice()->CreateBuffer(&bufDesc, &subResourse, &particle.vertex_buffer);

					// 인덱스 버퍼

					particle.index_list.push_back(0);
					particle.index_list.push_back(1);
					particle.index_list.push_back(2);
					particle.index_list.push_back(2);
					particle.index_list.push_back(1);
					particle.index_list.push_back(3);

					ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

					bufDesc.ByteWidth = sizeof(DWORD) * particle.index_list.size();
					bufDesc.Usage = D3D11_USAGE_DEFAULT;
					bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
					bufDesc.CPUAccessFlags = 0;
					bufDesc.MiscFlags = 0;
					bufDesc.StructureByteStride = 0;

					ZeroMemory(&subResourse, sizeof(D3D11_SUBRESOURCE_DATA));

					subResourse.pSysMem = &particle.index_list.at(0);
					subResourse.SysMemPitch;
					subResourse.SysMemSlicePitch;

					DX11APP->GetDevice()->CreateBuffer(&bufDesc, &subResourse, &particle.index_buffer);
				}

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
				particle.rotation = { 0, 0, randstep(emitter.initial_rotation[0], emitter.initial_rotation[1]) };
				// 크기는 현재 x값으로만 계산
				particle.scale = {
					randstep(emitter.initial_size[0].x, emitter.initial_size[1].x),
					randstep(emitter.initial_size[0].x, emitter.initial_size[1].x),
					randstep(emitter.initial_size[0].z, emitter.initial_size[1].z) };

				particle.add_velocity = {
					randstep(emitter.initial_velocity[0].x, emitter.initial_velocity[1].x),
					randstep(emitter.initial_velocity[0].y, emitter.initial_velocity[1].y),
					randstep(emitter.initial_velocity[0].z, emitter.initial_velocity[1].z) };

				particle.add_size = {
					randstep(emitter.size_per_lifetime[0].x, emitter.size_per_lifetime[1].x),
					randstep(emitter.size_per_lifetime[0].y, emitter.size_per_lifetime[1].y),
					randstep(emitter.size_per_lifetime[0].z, emitter.size_per_lifetime[1].z) };

				particle.add_rotation = randstep(emitter.rotation_per_lifetime[0], emitter.rotation_per_lifetime[1]);

				particle.accelation = {
					randstep(emitter.accelation_per_lifetime[0].x, emitter.accelation_per_lifetime[1].x),
					randstep(emitter.accelation_per_lifetime[0].y, emitter.accelation_per_lifetime[1].y),
					randstep(emitter.accelation_per_lifetime[0].z, emitter.accelation_per_lifetime[1].z) };
			}

		}
	}
	
}

void FX_Effect::AddEmitter(entt::registry& registry, Emitter emitter)
{
	auto& effect = registry.get<Effect>(entity_id_);
	effect.emitters.push_back(emitter);
}
