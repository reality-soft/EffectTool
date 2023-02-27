#pragma once
#include "FX_BaseEffectActor.h"

namespace KGCA41B {

	class StageActor : public Actor
	{
	public:
		virtual void OnInit(entt::registry& registry, KGCA41B::AABBShape collision_box);
		virtual void OnUpdate(entt::registry& registry);
	public:
			void	CreateVertexData(C_BoxShape& box_comp);
			HRESULT CreateVertexBuffer(C_BoxShape& box_comp);
			void	CreateIndexData(C_BoxShape& box_comp);
			HRESULT CreateIndexBuffer(C_BoxShape& box_comp);
	};
}