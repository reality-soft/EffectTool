#include "StageActor.h"
#include "TimeMgr.h"

using namespace reality;

void StageActor::OnInit(entt::registry& registry, reality::AABBShape collision_box)
{
	//collision_box_ = collision_box;

	entity_id_ = registry.create();

	// 트랜스폼 추가
	C_Transform& transform = registry.emplace<C_Transform>(entity_id_, C_Transform());
	transform.local = XMMatrixIdentity();
	transform.world = XMMatrixIdentity();
	transform_tree_.root_node = make_shared<TransformTreeNode>(TYPE_ID(reality::C_Transform));

	// 박스 컴포넌트 추가
	C_BoxShape& box_comp = registry.emplace<C_BoxShape>(entity_id_, C_BoxShape());
	//box_comp.local = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixRotationX(XMConvertToRadians(-90.0f)));
	box_comp.vs_id = "DefaultShapeVS.cso";
	box_comp.material_id = "Capsule.mat";

	transform_tree_.AddNodeToNode(TYPE_ID(reality::C_Transform), TYPE_ID(reality::C_BoxShape));

	transform_tree_.root_node->OnUpdate(registry, entity_id_);

	CreateVertexData(box_comp);
	CreateVertexBuffer(box_comp);
	CreateIndexData(box_comp);
	CreateIndexBuffer(box_comp);
}

void reality::StageActor::OnUpdate(entt::registry& registry)
{
	//this->node_num_ = SpacePartition::GetInst()->UpdateNodeObjectBelongs(0, collision_box_, entity_id_);
	//vector<int> node_to_search = SpacePartition::GetInst()->FindCollisionSearchNode(0, collision_box_);
	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}


void reality::StageActor::CreateVertexData(C_BoxShape& box_comp)
{
	box_comp.vertex_list.push_back({ { -1.0f, +1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+0.0f, +0.0f} });
	box_comp.vertex_list.push_back({ { +1.0f, +1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +0.0f} });
	box_comp.vertex_list.push_back({ { -1.0f, -1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+0.0f, +1.0f} });
	box_comp.vertex_list.push_back({ { +1.0f, -1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f} });
}

HRESULT reality::StageActor::CreateVertexBuffer(C_BoxShape& box_comp)
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

void reality::StageActor::CreateIndexData(C_BoxShape& box_comp)
{
	box_comp.index_list.push_back(0);
	box_comp.index_list.push_back(1);
	box_comp.index_list.push_back(2);
	box_comp.index_list.push_back(2);
	box_comp.index_list.push_back(1);
	box_comp.index_list.push_back(3);
}

HRESULT reality::StageActor::CreateIndexBuffer(C_BoxShape& box_comp)
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
