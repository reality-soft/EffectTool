#include "EffectTool.h"
#include "ComponentSystem.h"
#include "SceneMgr.h"
#include "DXStates.h"

using namespace KGCA41B;

void EffectTool::OnInit()
{
	GUI->AddWidget("MainMenu", &tool_window_);
	DATA->Init("D:/Data");
	RESOURCE->Init("D:/Contents"); 

	RENDER_TARGET->Init("BackBuffer");

	SCENE->PushScene("EffectTool", this);

	debug_camera_.position = { 0, 0, -10, 0 };
	debug_camera_.look = { 0, 0, 0, 0 };
	debug_camera_.up = { 0, 1, 0, 0 };
	debug_camera_.near_z = 1.f;
	debug_camera_.far_z = 10000.f;
	debug_camera_.fov = XMConvertToRadians(45);
	debug_camera_.yaw = 0;
	debug_camera_.pitch = 0;
	debug_camera_.roll = 0;
	debug_camera_.speed = 10;
	debug_camera_.tag = "Player";

	reg_effect_tool_.emplace<C_Camera>(player_, debug_camera_);


	sys_camera_.TargetTag(reg_effect_tool_, "Player");
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

void KGCA41B::EffectTool::AddEmitter(shared_ptr<Emitter> emitter)
{
	
	effect_.AddEmitter(reg_effect_tool_, emitter);
}

void KGCA41B::EffectTool::ResetEmitter()
{
	effect_.ResetEmitter(reg_effect_tool_);
}

