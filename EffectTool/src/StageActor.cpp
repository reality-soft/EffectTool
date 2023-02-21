#include "StageActor.h"
#include "TimeMgr.h"

using namespace KGCA41B;

void StageActor::OnInit(entt::registry& registry, KGCA41B::AABBShape collision_box)
{
	collision_box_ = collision_box;

	entity_id_ = registry.create();

	// 트랜스폼 추가
	
	Transform& transform = registry.emplace<Transform>(entity_id_, Transform());
	transform.local = XMMatrixIdentity();
	transform.world = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixRotationX(90.0f));
	transform_tree_.root_node = make_shared<TransformTreeNode>(TYPE_ID(KGCA41B::Transform));

	// 박스 컴포넌트 추가
	BoxShape& box_comp = registry.emplace<BoxShape>(entity_id_, BoxShape());
	transform.local = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixRotationX(-90.0f));
	box_comp.vs_id = "EffectDefaultVS.cso";
	transform_tree_.AddNodeToNode(TYPE_ID(KGCA41B::Transform), TYPE_ID(KGCA41B::BoxShape));

	// 머터리얼 컴포넌트 추가
	Material& material = registry.emplace<Material>(entity_id_, Material());
	material.shader_id = "EffectDefaultPS.cso";
	material.texture_id.push_back("stage.png");


	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}

void KGCA41B::StageActor::OnUpdate(entt::registry& registry)
{
	//this->node_num_ = SpacePartition::GetInst()->UpdateNodeObjectBelongs(0, collision_box_, entity_id_);
	//vector<int> node_to_search = SpacePartition::GetInst()->FindCollisionSearchNode(0, collision_box_);
	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}


void KGCA41B::StageActor::CreateVertexData(BoxShape& box_comp)
{
	box_comp.vertex_list.push_back({ { -1.0f, +1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+0.0f, +0.0f} });
	box_comp.vertex_list.push_back({ { +1.0f, +1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+1.0f, +0.0f} });
	box_comp.vertex_list.push_back({ { -1.0f, -1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+0.0f, +1.0f} });
	box_comp.vertex_list.push_back({ { +1.0f, -1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+1.0f, +1.0f} });
}

HRESULT KGCA41B::StageActor::CreateVertexBuffer(BoxShape& box_comp)
{
	if (box_comp.vertex_list.size() == 0)
		return S_OK;

	D3D11_BUFFER_DESC bufDesc;

	ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

	bufDesc.ByteWidth = sizeof(Vertex) * box_comp.vertex_list.size();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourse;

	ZeroMemory(&subResourse, sizeof(D3D11_SUBRESOURCE_DATA));

	subResourse.pSysMem = &box_comp.vertex_list.at(0);
	subResourse.SysMemPitch;
	subResourse.SysMemSlicePitch;

	return DX11APP->GetDevice()->CreateBuffer(&bufDesc, &subResourse, &box_comp.vertex_buffer);
}

void KGCA41B::StageActor::CreateIndexData(BoxShape& box_comp)
{
	box_comp.index_list.push_back(0);
	box_comp.index_list.push_back(1);
	box_comp.index_list.push_back(2);
	box_comp.index_list.push_back(2);
	box_comp.index_list.push_back(1);
	box_comp.index_list.push_back(3);
}

HRESULT KGCA41B::StageActor::CreateIndexBuffer(BoxShape& box_comp)
{
	D3D11_BUFFER_DESC bufDesc;

	ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

	bufDesc.ByteWidth = sizeof(DWORD) * box_comp.index_list.size();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourse;

	ZeroMemory(&subResourse, sizeof(D3D11_SUBRESOURCE_DATA));

	subResourse.pSysMem = &box_comp.index_list.at(0);
	subResourse.SysMemPitch;
	subResourse.SysMemSlicePitch;

	return DX11APP->GetDevice()->CreateBuffer(&bufDesc, &subResourse, &box_comp.index_buffer);
}
