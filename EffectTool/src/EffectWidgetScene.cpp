#include "EffectWidgetScene.h"

void reality::EffectWidgetScene::OnInit()
{
	rt_id = "Widget";
	RENDER_TARGET->MakeRT(rt_id, ENGINE->GetWindowSize().x, ENGINE->GetWindowSize().y);
	
}

void reality::EffectWidgetScene::OnUpdate()
{
}

void reality::EffectWidgetScene::OnRender()
{
	auto rt = RENDER_TARGET->LoadRT(rt_id);
	rt->SetRenderTarget();
	sys_render.OnUpdate(reg_scene);
	rt = RENDER_TARGET->LoadRT("BackBuffer");
	rt->SetRenderTarget();
}

void reality::EffectWidgetScene::OnRelease()
{
}
