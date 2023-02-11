#include "EffectTool.h"
#include "ComponentSystem.h"

using namespace KGCA41B;

void EffectTool::OnInit()
{
	GUI->AddWidget("MainMenu", &window);
}

void EffectTool::OnUpdate()
{
	
}

void EffectTool::OnRender()
{
	GUI->RenderWidgets();
}

void EffectTool::OnRelease()
{

}

