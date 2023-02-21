#include "FX_Emitter.h"
#include "Components.h"
using namespace KGCA41B;

void FX_Emitter::OnInit(entt::registry& registry, AABBShape collision_box, Emitter emitter)
{
	FX_BaseEffectActor::OnInit(registry, collision_box);

	emitter.local = XMMatrixIdentity();
	auto& particles_comp = registry.emplace<Emitter>(entity_id_, emitter);

	transform_tree_.AddNodeToNode(TYPE_ID(KGCA41B::Transform), TYPE_ID(KGCA41B::Emitter));
	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}

void KGCA41B::FX_Emitter::OnUpdate(entt::registry& registry)
{
	FX_BaseEffectActor::OnUpdate(registry);

	auto& emitter = registry.get<Emitter>(entity_id_);

}

void KGCA41B::FX_Emitter::CreateVertexData(Emitter& emitter)
{
	for (int i = 0; i < emitter.particle_count; i++)
	{
		emitter.vertex_list.push_back({ { +0.0f, +0.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+0.5f, +0.5f} });
	}
	
}

HRESULT KGCA41B::FX_Emitter::CreateVertexBuffer(Emitter& emitter)
{
	if (emitter.vertex_list.size() == 0)
		return S_OK;

	D3D11_BUFFER_DESC bufDesc;

	ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

	bufDesc.ByteWidth = sizeof(Vertex) * emitter.vertex_list.size();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourse;

	ZeroMemory(&subResourse, sizeof(D3D11_SUBRESOURCE_DATA));

	subResourse.pSysMem = &emitter.vertex_list.at(0);
	subResourse.SysMemPitch;
	subResourse.SysMemSlicePitch;

	return DX11APP->GetDevice()->CreateBuffer(&bufDesc, &subResourse, &emitter.vertex_buffer);
}
