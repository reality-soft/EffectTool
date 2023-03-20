#pragma once
#include "Widgets.h"

namespace reality
{
	class EffectWidgetScene : public reality::Scene
	{
	public:
		string rt_id;
		//FX_Particles
	public:
		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnRelease();
	private:
		entt::registry reg_scene;

		reality::RenderSystem sys_render;
	};

}

