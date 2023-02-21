#include "EffectWidgetScene.h"

void KGCA41B::EffectWidgetScene::OnInit()
{
	rt_id = "Widget";
	RENDER_TARGET->MakeRT(rt_id, ENGINE->GetWindowSize().x, ENGINE->GetWindowSize().y);
	
}

void KGCA41B::EffectWidgetScene::OnUpdate()
{
}

void KGCA41B::EffectWidgetScene::OnRender()
{
	auto rt = RENDER_TARGET->LoadRT(rt_id);
	rt->SetRenderTarget();
	sys_render.OnUpdate(reg_scene);
	rt = RENDER_TARGET->LoadRT("BackBuffer");
	rt->SetRenderTarget();
}

void KGCA41B::EffectWidgetScene::OnRelease()
{
}