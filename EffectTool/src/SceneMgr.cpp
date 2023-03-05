#include "SceneMgr.h"

using namespace reality;

void reality::SceneMgr::PushScene(string id, Scene* scene)
{
	if (resdic_scene.find(id) != resdic_scene.end())
		return;

	resdic_scene.insert({ id, scene });
}

Scene* reality::SceneMgr::LoadScene(string id)
{
	if (resdic_scene.find(id) == resdic_scene.end())
		return nullptr;
	else
		return resdic_scene[id];
}
