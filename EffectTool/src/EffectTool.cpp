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
	DINPUT->Init(ENGINE->GetWindowHandle(), ENGINE->GetInstanceHandle());

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

	reg_effect_tool_.emplace<Camera>(player_, debug_camera_);

	debug_input_.tag = "Player";
	reg_effect_tool_.emplace<InputMapping>(player_, debug_input_);

	sys_camera_.TargetTag(reg_effect_tool_, "Player");
	sys_camera_.OnCreate(reg_effect_tool_);
	sys_input_.OnCreate(reg_effect_tool_);
	sys_render_.OnCreate(reg_effect_tool_);

	stage_.OnInit(reg_effect_tool_, {});

	effect_.OnInit(reg_effect_tool_, {}, {});
}

void EffectTool::OnUpdate()
{
	sys_input_.OnUpdate(reg_effect_tool_);
	sys_camera_.OnUpdate(reg_effect_tool_);

	stage_.OnUpdate(reg_effect_tool_);

	effect_.OnUpdate(reg_effect_tool_);
}

void EffectTool::OnRender()
{
	// Z 버퍼 비교 & Z 버퍼 쓰기
	if (bZbufferComp && bZbufferWrite)
		DX11APP->GetDeviceContext()->OMSetDepthStencilState(DXStates::ds_defalut(), 1);
	else if (bZbufferComp)
		DX11APP->GetDeviceContext()->OMSetDepthStencilState(DXStates::ds_depth_enable_no_write(), 1);
	else
		DX11APP->GetDeviceContext()->OMSetDepthStencilState(DXStates::ds_depth_disable(), 1);

	// 알파 블랜딩
	if (bAlphaBlending)
		DX11APP->GetDeviceContext()->OMSetBlendState(DXStates::bs_default(), 0, -1);
	else
		DX11APP->GetDeviceContext()->OMSetBlendState(0, 0, -1);

	// 알파 테스팅?

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

void KGCA41B::EffectTool::AddEmitter(Emitter emitter)
{
	
	effect_.AddEmitter(reg_effect_tool_, emitter);
}

