#include "Widgets.h"

using namespace KGCA41B;

void WG_MainMenu::Update()
{
	ImGui::SetCurrentContext(GUI->GetContext());
}

void WG_MainMenu::Render()
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::MenuItem("Effect"))
		{
			GUI->AddWidget("EffectTool", new WG_EffectWindow);
		}
	}
	ImGui::EndMainMenuBar();
}

void WG_EffectWindow::Update()
{
	ImGui::SetCurrentContext(GUI->GetContext());
}

void WG_EffectWindow::Render()
{
	ImGui::Begin("Effect Tool", &open_, ImGuiWindowFlags_MenuBar);
	{
		ImGui::BeginMenuBar();
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Effect", "Ctrl+N")) {}
				if (ImGui::MenuItem("Load Effect", "Ctrl+L")) 
				{
					GUI->AddWidget("FileViewer", new WG_FileViewer);
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();

		
		static int frame = 0;
		ImGui::SliderInt("Frame", &frame, 0, 30);

		static bool bZbuffer = false;
		ImGui::Checkbox("Z buffer", &bZbuffer);
		static bool bAlphaBlending = false;
		ImGui::Checkbox("AlphaBlending", &bAlphaBlending);
		static bool bAlphaTesting = false;
		ImGui::Checkbox("AlphaTesting", &bAlphaTesting);
		static bool bWireFrame = false;
		ImGui::Checkbox("WireFrame", &bWireFrame);

		static char effectName[255];
		ImGui::InputText("effectName", effectName, IM_ARRAYSIZE(effectName));

		if (ImGui::Button("Save"))
		{

		}
	}
	ImGui::End();
}

void WG_FileViewer::Update()
{
	ImGui::SetCurrentContext(GUI->GetContext());
}

void WG_FileViewer::Render()
{
	ImGui::Begin("Load Effect File", &open_);
	{

	}
	ImGui::End();
}
