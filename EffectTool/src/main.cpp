#include "EffectTool.h"

using namespace reality;

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ENGINE->OnInit(hinstance, L"EffectTool", E_Resolution::R1280x720, true);

	SCENE_MGR->AddScene<EffectTool>(E_SceneType::START);
	SCENE_MGR->ChangeScene(E_SceneType::START);
	ENGINE->Run();

	ENGINE->OnRelease();

	return 0;
}