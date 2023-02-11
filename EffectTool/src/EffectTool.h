#pragma once
#include "Engine_include.h"
#include "Widgets.h"

class EffectTool : public KGCA41B::Scene
{
public:
	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnRelease();

public:
	WG_MainMenu window;
private:
};

