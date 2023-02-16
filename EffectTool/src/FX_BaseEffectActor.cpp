#include "FX_BaseEffectActor.h"

void KGCA41B::FX_BaseEffectActor::OnInit(entt::registry& registry, AABB<3> collision_box)
{
	collision_box_ = collision_box;

	entity_id_ = registry.create();
	//this->node_num_ = SpacePartition::GetInst()->UpdateNodeObjectBelongs(0, collision_box_, entity_id_);

	entt::type_hash<Transform> type_hash_transform;
	Transform transform;
	transform.local = XMMatrixIdentity();
	registry.emplace<Transform>(entity_id_, transform);

	transform_tree_.root_node = make_shared<TransformTreeNode>(type_hash_transform.value());

	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}

void KGCA41B::FX_BaseEffectActor::OnUpdate(entt::registry& registry)
{
	//this->node_num_ = SpacePartition::GetInst()->UpdateNodeObjectBelongs(0, collision_box_, entity_id_);
	//vector<int> node_to_search = SpacePartition::GetInst()->FindCollisionSearchNode(0, collision_box_);
	transform_tree_.root_node->OnUpdate(registry, entity_id_);
}
