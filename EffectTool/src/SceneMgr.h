#pragma once
#include "DllMacro.h"
#include "Scene.h"

namespace reality
{
	class SceneMgr
	{
		SINGLETON(SceneMgr)
#define SCENE SceneMgr::GetInst()
	private:
		map<string, Scene*> resdic_scene;
	public:
		void	PushScene(string id, Scene* scene);
		Scene*	LoadScene(string id);
	};
}


