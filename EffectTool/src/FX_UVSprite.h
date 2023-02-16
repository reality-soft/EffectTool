#pragma once
#include "FX_BaseEffectActor.h"

namespace KGCA41B {
	class FX_UVSprite : public FX_BaseEffectActor
	{
	public:
		float	timer_;
	public:
		virtual void OnInit(entt::registry& registry, KGCA41B::AABB<3> collision_box, UVSprite uv_sprite);
		virtual void OnUpdate(entt::registry& registry);
	public:
		void	SetUVSprite(entt::registry& registry, UVSpriteData uv_sprite);
	public:
		void	CreateVertexData(UVSprite& sprite_comp);
		HRESULT CreateVertexBuffer(UVSprite& sprite_comp);
		void	CreateIndexData(UVSprite& sprite_comp);
		HRESULT CreateIndexBuffer(UVSprite& sprite_comp);
	};
}