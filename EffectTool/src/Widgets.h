#pragma once
#include "Engine_include.h"

class WG_MainMenu : public KGCA41B::GuiWidget
{
public:
	virtual void Update() override;
	virtual void Render() override;
};

class WG_EffectWindow : public KGCA41B::GuiWidget
{
public:
	virtual void Update() override;
	virtual void Render() override;
};

class WG_FileViewer : public KGCA41B::GuiWidget
{
public:
	virtual void Update() override;
	virtual void Render() override;
};
