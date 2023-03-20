#include "EffectTool.h"

using namespace reality;

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ENGINE->OnInit(hinstance, L"EffectTool", { 1600, 900 });


	SCENE_MGR->SetScene<EffectTool>();
	ENGINE->Run();

	ENGINE->OnRelease();

	return 0;
}