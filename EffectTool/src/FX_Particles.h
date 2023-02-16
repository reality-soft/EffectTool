#pragma once
#include "FX_BaseEffectActor.h"

namespace KGCA41B {
	class FX_Particles : public FX_BaseEffectActor
	{
	public:
		virtual void OnInit(entt::registry& registry, KGCA41B::AABB<3> collision_box, Particles particles);
		virtual void OnUpdate(entt::registry& registry, Particles particles);
	public:
		void	CreateVertexData(Particles& particle_comp);
		HRESULT CreateVertexBuffer(Particles& particle_comp);
		void	CreateParticle(Particles& particle_comp);
	};
}