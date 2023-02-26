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
		InputSystem			sys_input_;
	public:
		entt::entity		player_;
		Camera				debug_camera_;
		InputMapping		debug_input_;
	public:
		WG_MainMenu			tool_window_;

	public:
		StageActor			stage_;

	public:
		bool bZbufferComp = false;
		bool bZbufferWrite = false;
		bool bAlphaBlending = false;
		bool bWireFrame = false;

	public:
		FX_Effect	effect_; 

	public:
		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnRelease();

	public:
		void AddEmitter(Emitter emitter);
	};
}
