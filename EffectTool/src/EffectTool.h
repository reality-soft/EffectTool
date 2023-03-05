#pragma once
#include "Engine_include.h"
#include "Widgets.h"
#include "EffectWidgetScene.h"
#include "StageActor.h"
#include "FX_Effect.h"

namespace KGCA41B
{
	class EffectTool : public KGCA41B::Scene
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
		FX_Effect	effect_; 

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
