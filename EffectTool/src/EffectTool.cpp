#include "EffectTool.h"
#include "ComponentSystem.h"
#include "SceneMgr.h"
#include "DXStates.h"

using namespace reality;

void EffectTool::OnInit()
{
	GUI->AddWidget("MainMenu", &tool_window_);
	DATA->Init("../../Data");
	RESOURCE->Init("../../Contents"); 

	RENDER_TARGET->Init("BackBuffer");

	SCENE->PushScene("EffectTool", this);


	sys_camera_.OnCreate(reg_effect_tool_);
	sys_render_.OnCreate(reg_effect_tool_);
	sys_effect_.OnCreate(reg_effect_tool_);

	stage_.OnInit(reg_effect_tool_, {});

	effect_.OnInit(reg_effect_tool_, {}, {});
}

void EffectTool::OnUpdate()
{
	sys_camera_.OnUpdate(reg_effect_tool_);
	sys_effect_.OnUpdate(reg_effect_tool_);
	stage_.OnUpdate(reg_effect_tool_);
	effect_.OnUpdate(reg_effect_tool_);
}

void EffectTool::OnRender()
{
	// 와이어 프레임 체크
	if (bWireFrame)
		DX11APP->GetDeviceContext()->RSSetState(DXStates::rs_wireframe_cull_none());
	else
		DX11APP->GetDeviceContext()->RSSetState(DXStates::rs_solid_cull_none());

	sys_render_.OnUpdate(reg_effect_tool_);
	GUI->RenderWidgets();
}

void EffectTool::OnRelease()
{

}

void EffectTool::AddEmitter(Emitter& emitter)
{
	map<string, Emitter> map;
	map.insert({ "", emitter });
	effect_.AddEffect(reg_effect_tool_, map);
}

void EffectTool::AddEffect(map<string, Emitter>& emitter_list)
{
	effect_.AddEffect(reg_effect_tool_, emitter_list);
}

void EffectTool::ResetEmitter()
{
	effect_.ResetEmitter(reg_effect_tool_);
}

