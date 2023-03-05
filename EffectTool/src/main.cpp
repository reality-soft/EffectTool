#include "EffectTool.h"

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ENGINE->OnInit(hinstance, L"EffectTool", { 1600, 900 });

	reality::EffectTool tool;

	ENGINE->Run(&tool);

	ENGINE->OnRelease();

	return 0;
}