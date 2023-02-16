#pragma once
#include "FX_BaseEffectActor.h"

namespace KGCA41B {

	class FX_TexSprite : public FX_BaseEffectActor
	{
	public:
		float	timer_;
	public:
		virtual void OnInit(entt::registry& registry, KGCA41B::AABB<3> collision_box, TextureSprite tex_sprite);
		virtual void OnUpdate(entt::registry& registry);
	public:
		void		SetTexSprite(entt::registry& registry, TexSpriteData tex_sprite);
	public:
			void	CreateVertexData(TextureSprite& sprite_comp);
			HRESULT CreateVertexBuffer(TextureSprite& sprite_comp);
			void	CreateIndexData(TextureSprite& sprite_comp);
			HRESULT CreateIndexBuffer(TextureSprite& sprite_comp);
	};
}