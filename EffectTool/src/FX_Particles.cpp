#include "FX_Particles.h"
#include "Components.h"
using namespace KGCA41B;

void FX_Particles::OnInit(entt::registry& registry, KGCA41B::AABB<3> collision_box, Particles particles)
{
	FX_BaseEffectActor::OnInit(registry, collision_box);

	entt::type_hash<KGCA41B::Particles> type_hash_particles;
	particles.local = XMMatrixIdentity();
	auto& particles_comp = registry.emplace<Particles>(entity_id_, particles);

	transform_tree_.root_node->children.push_back(make_shared<TransformTreeNode>(type_hash_particles.value()));

	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}

void KGCA41B::FX_Particles::OnUpdate(entt::registry& registry, Particles particles)
{
	FX_BaseEffectActor::OnUpdate(registry);

	auto& view_particles = registry.get<Particles>(entity_id_);
	view_particles = particles;
}

void KGCA41B::FX_Particles::CreateVertexData(Particles& particle_comp)
{
	for (int i = 0; i < particle_comp.particle_count; i++)
	{
		particle_comp.vertex_list.push_back({ { +0.0, +0.0, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+0.5f, +0.5f} });
	}
	
}

HRESULT KGCA41B::FX_Particles::CreateVertexBuffer(Particles& particle_comp)
{
	if (particle_comp.vertex_list.size() == 0)
		return S_OK;

	D3D11_BUFFER_DESC bufDesc;

	ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

	bufDesc.ByteWidth = sizeof(Vertex) * particle_comp.vertex_list.size();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourse;

	ZeroMemory(&subResourse, sizeof(D3D11_SUBRESOURCE_DATA));

	subResourse.pSysMem = &particle_comp.vertex_list.at(0);
	subResourse.SysMemPitch;
	subResourse.SysMemSlicePitch;

	return DX11APP->GetDevice()->CreateBuffer(&bufDesc, &subResourse, &particle_comp.vertex_buffer);
}

void KGCA41B::FX_Particles::CreateParticle(Particles& particle_comp)
{
	for (int i = 0; i < particle_comp.particle_count; i++)
	{
		Particle particle;

		particle.enable = true;
		particle.tex_id = particle_comp.tex_id_list[0];
		particle.position = { 0.0f, 0.0f, 0.0f };
		particle.velocity = { 0.0f, 10.0f, 0.0f };
		particle.duration = 3.0f;
		particle.timer = 0.0f;
		particle.color = { 0.5f, 0.5f, 0.5f, 1.0f };

		particle_comp.particle_list.push_back(particle);
	}
}
