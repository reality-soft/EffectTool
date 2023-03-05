#pragma once
#include "FX_BaseEffectActor.h"

namespace reality {

	class StageActor : public Actor
	{
	public:
		virtual void OnInit(entt::registry& registry, reality::AABBShape collision_box);
		virtual void OnUpdate(entt::registry& registry);
	public:
			void	CreateVertexData(C_BoxShape& box_comp);
			HRESULT CreateVertexBuffer(C_BoxShape& box_comp);
			void	CreateIndexData(C_BoxShape& box_comp);
			HRESULT CreateIndexBuffer(C_BoxShape& box_comp);
	};
}