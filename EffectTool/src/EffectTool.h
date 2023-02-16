#pragma once
#include "Engine_include.h"
#include "Widgets.h"
#include "EffectWidgetScene.h"
#include "FX_UVSprite.h"
#include "FX_TexSprite.h"

namespace KGCA41B
{
	class EffectTool : public KGCA41B::Scene
	{
	public:
		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnRelease();

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
		EffectWidgetScene	widget_scene_;
	public:
		WG_MainMenu			window_;
	public:
		FX_UVSprite			uv_sprite_;
		FX_TexSprite		tex_sprite_;
	};
}
