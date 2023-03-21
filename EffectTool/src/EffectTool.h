#pragma once
#include "EffectWidgetScene.h"
#include "StageActor.h"

namespace reality
{
	class EffectTool : public reality::Scene
	{
	public:
		entt::registry		reg_effect_tool_;
	public:
		RenderSystem		sys_render_;
		CameraSystem		sys_camera_;
		EffectSystem		sys_effect_;
	public:
		WG_MainMenu			tool_window_;

	public:
		StageActor			stage_;

	public:
		bool bWireFrame = false;

	public:
		FX_BaseEffectActor	effect_; 

	public:
		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnRelease();

	public:
		void AddEmitter(Emitter& emitter);
		void AddEffect(map<string, Emitter>& emitter_list);
		void ResetEmitter();
	};
}
