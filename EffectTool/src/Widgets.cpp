#include "Widgets.h"
#include "imfilebrowser.h"
#include "SceneMgr.h"
#include "EffectTool.h"

using namespace KGCA41B;

#define LISTBOX_WIDTH 150.0f
#define TEXT_WIDTH 150.0f

#define GET_VARIABLE_NAME(n) #n

#define MIN 0
#define MAX 1

void WG_MainMenu::Update()
{
	ImGui::SetCurrentContext(GUI->GetContext());
	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void WG_MainMenu::Render()
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::MenuItem("Effect"))
		{
			auto effect_tool = GUI->FindWidget("EffectTool");
			if (effect_tool == nullptr)
				GUI->AddWidget("EffectTool", new WG_EffectWindow);
			else
				effect_tool->InvertOpen();
		}
	}
	ImGui::EndMainMenuBar();
}

void WG_EffectWindow::Update()
{
	ImGui::SetCurrentContext(GUI->GetContext());
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void WG_EffectWindow::Render()
{
	ImGui::SetNextWindowSize(ImVec2(450, 600));
	ImGui::Begin("Effect Tool", &open_, ImGuiWindowFlags_MenuBar);
	{
		ImGui::BeginMenuBar();
		{
			if (ImGui::BeginMenu("New Effect"))
			{
				if (ImGui::MenuItem("UV Sprite"))
				{
					type_ = UV_TAB;
				}
				if (ImGui::MenuItem("Texture Sprite"))
				{
					type_ = TEX_TAB;
				}
				if (ImGui::MenuItem("Emitter"))
				{
					type_ = EMITTER_TAB;
				}
				ImGui::EndMenu();
			}
			FileBrowser();
		}
		ImGui::EndMenuBar();

		switch (type_)
		{
		case NONE:
		{

		}
		break;
		case UV_SPRITE:
		{
			UVSpriteBoard();
		}
		break;
		case TEX_SPRITE:
		{
			TexSpriteBoard();
		}
		break;
		case EMITTER:
		{
			EmitterBoard();
		}
		break;
		}
	}
	ImGui::End();
}

void WG_EffectWindow::FileBrowser()
{
	static ImGui::FileBrowser fileDialog;
	static int type = 0;
	if (ImGui::BeginMenu("Load Effect"))
	{
		if (ImGui::MenuItem("Load Sprite"))
		{
			type = 1;
			fileDialog.Open();
			fileDialog.SetTypeFilters({ ".csv" });
		}
		if (ImGui::MenuItem("Load Emitter"))
		{
			type = 2;
			fileDialog.Open();
			fileDialog.SetTypeFilters({ ".csv" });
		}
		ImGui::EndMenu();
	}
		
	fileDialog.Display();

	if (fileDialog.HasSelected())
	{
		if (type == 1)
			LoadingSpriteData(fileDialog.GetSelected().string());
		else if (type == 2)
			LoadingEmitterData(fileDialog.GetSelected().string());

		fileDialog.ClearSelected();
		fileDialog.Close();
	}
}

void WG_EffectWindow::UVSpriteBoard()
{
	static Texture* tex = nullptr;
	static int cur_frame = 1;
	static char sprite_name[255] = { 0, };

	ImVec2 img_size = { 200, 200 };
	//ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcItemSize(img_size, img_size.x, img_size.y).x) / 2);

	
	tex = RESOURCE->UseResource<Texture>(uv_sprite_data.tex_id);

	if(tex)
		ImGui::Image((void*)tex->srv.Get(), img_size);

	if (uv_sprite_data.uv_list.size() > 0 && tex)
	{
		ImGui::SameLine();
		auto uv = uv_sprite_data.uv_list[min(cur_frame - 1, (int)uv_sprite_data.uv_list.size() - 1)];
		float tex_width = (float)tex->texture_desc.Width;
		float tex_height = (float)tex->texture_desc.Height;
		ImVec2 start(uv.first.x / tex_width, uv.first.y / tex_height);
		ImVec2 end(uv.second.x / tex_width, uv.second.y / tex_height);
		ImGui::Image((void*)tex->srv.Get(), img_size, start, end);
	}

	// 프레임 선택
	SelectFrame(uv_sprite_data.max_frame, cur_frame);

	// 텍스쳐 선택
	SelectTexture(uv_sprite_data.tex_id);
	
	// UV 값 설정
	if (uv_sprite_data.uv_list.size() > uv_sprite_data.max_frame)
		uv_sprite_data.uv_list.resize(uv_sprite_data.max_frame);
	SelectUV(uv_sprite_data.uv_list, uv_sprite_data.max_frame);

	ImGui::SetNextItemWidth(TEXT_WIDTH);
	ImGui::InputTextWithHint("sprite name", "Name", sprite_name, IM_ARRAYSIZE(sprite_name));

	if (ImGui::Button("Save"))
	{
		SaveUVSprite(sprite_name);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		tex = nullptr;
		cur_frame = 1;
		memset(sprite_name, 0, sizeof(char) * strlen(sprite_name));

		uv_sprite_data.tex_id = "";
		uv_sprite_data.uv_list.clear();
		uv_sprite_data.max_frame = 5;
	}
}

void WG_EffectWindow::TexSpriteBoard()
{
	static string texture_id = "";
	static Texture* tex = nullptr;
	static int cur_frame = 1;
	static char sprite_name[255] = { 0, };

	ImVec2 img_size = { 200, 200 };
	//ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcItemSize(img_size, img_size.x, img_size.y).x) / 2);
	

	tex = RESOURCE->UseResource<Texture>(texture_id);

	if (tex)
		ImGui::Image((void*)tex->srv.Get(), img_size);

	
	if (tex_sprite_data.tex_id_list.size() > 0)
	{
		ImGui::SameLine();
		tex = RESOURCE->UseResource<Texture>(tex_sprite_data.tex_id_list[min((int)cur_frame - 1, (int)tex_sprite_data.tex_id_list.size() - 1)]);
		ImGui::Image((void*)tex->srv.Get(), img_size);
	}
		

	// 프레임 선택
	SelectFrame(tex_sprite_data.max_frame, cur_frame);

	// 텍스쳐 선택
	SelectTexture(texture_id);

	if (tex_sprite_data.tex_id_list.size() > tex_sprite_data.max_frame)
		tex_sprite_data.tex_id_list.resize(tex_sprite_data.max_frame);

	// 텍스쳐 배열 추가
	if (ImGui::Button("Add"))
	{
		if (tex_sprite_data.tex_id_list.size() == tex_sprite_data.max_frame)
			tex_sprite_data.max_frame++;
		tex_sprite_data.tex_id_list.push_back(texture_id);
	}

	//ImGui::SameLine();
	ImGui::SetNextItemWidth(LISTBOX_WIDTH);
	static int item_current_idx = 0;
	if (ImGui::BeginListBox("Tex ID per Frame"))
	{
		for (int n = 0; n < tex_sprite_data.tex_id_list.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			string str = "[" + to_string(n + 1) + "] " + tex_sprite_data.tex_id_list[n];
			if (ImGui::Selectable(str.c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	if (ImGui::Button("Delete Last Frame"))
	{
		if (tex_sprite_data.tex_id_list.size() > 0)
			tex_sprite_data.tex_id_list.pop_back();
	}

	ImGui::SetNextItemWidth(TEXT_WIDTH);
	ImGui::InputTextWithHint("sprite name", "Name", sprite_name, IM_ARRAYSIZE(sprite_name));

	if (ImGui::Button("Save"))
	{
		// TODO : 데이터 테이블을 통한 저장
		SaveTexSprite(sprite_name);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		texture_id = "";
		tex = nullptr;
		cur_frame = 1;
		memset(sprite_name, 0, sizeof(char) * strlen(sprite_name));

		tex_sprite_data.tex_id_list.clear();
		tex_sprite_data.max_frame = 5;
	}
	ImGui::SameLine();
	if (ImGui::Button("Render"))
	{
		
	}
}

void WG_EffectWindow::EmitterBoard()
{
	static char particle_name[255] = { 0, };
	static int cur_frame	= 1;
	static float timer		= 1.0f;


	// Render Selected Sprite
	auto sprite = RESOURCE->UseResource<Sprite>(emitter_data.sprite_id);
	if (sprite)
	{
		Texture* tex = nullptr;
		ImVec2 img_size = { 200, 200 };
		// 스프라이트 타입에 따라 다르게 랜더링
		// UV Sprite
		if (sprite->type == UV)
		{
			UVSprite* uv_sprite = (UVSprite*)sprite;
			tex = RESOURCE->UseResource<Texture>(uv_sprite->tex_id);

			// 프레임 처리
			timer += TIMER->GetDeltaTime();
			if (timer > uv_sprite->max_frame + 1)
				timer -= uv_sprite->max_frame;
			cur_frame = (int)timer;

			auto uv = uv_sprite->uv_list[min((int)cur_frame - 1, (int)uv_sprite->uv_list.size() - 1)];
			float tex_width = (float)tex->texture_desc.Width;
			float tex_height = (float)tex->texture_desc.Height;
			ImVec2 start(uv.first.x / tex_width, uv.first.y / tex_height);
			ImVec2 end(uv.second.x / tex_width, uv.second.y / tex_height);

			
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcItemSize(img_size, img_size.x, img_size.y).x) / 2);
			ImGui::Image((void*)tex->srv.Get(), img_size, start, end);
		}
		// Texture Sprite
		else if (sprite->type == TEX)
		{
			TextureSprite* tex_sprite = (TextureSprite*)sprite;
			// 프레임 처리
			timer += TIMER->GetDeltaTime();
			if (timer > tex_sprite->max_frame + 1)
				timer -= tex_sprite->max_frame;
			cur_frame = (int)timer;

			tex = RESOURCE->UseResource<Texture>(tex_sprite->tex_id_list[min((int)cur_frame - 1, (int)tex_sprite->tex_id_list.size() - 1)]);

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcItemSize(img_size, img_size.x, img_size.y).x) / 2);
			ImGui::Image((void*)tex->srv.Get(), img_size);
		}
	}
	

	// Sprite_id or Something?
	ImGui::Text("Sprite");
	SelectSprite(emitter_data.sprite_id);

	// Particle Count
	ImGui::Text("Emit per Second");
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputInt("Emit per Second", &emitter_data.emit_per_second);

	// Color
	ImGui::Text("Particle Color");
	static ImVec4 color;
	ImGui::ColorEdit3("Color", (float*)&emitter_data.color);
	emitter_data.color.w = 1.0f;

	// Life Time
	ImGui::Text("Life Time (seconds)");
	ImGui::SetNextItemWidth(50.0f);
	ImGui::InputFloat("Life Time Min", &emitter_data.life_time[MIN]);
	ImGui::SetNextItemWidth(50.0f);
	ImGui::InputFloat("Life Time Max", &emitter_data.life_time[MAX]);

	// Initial Size
	ImGui::Text("Initial Size (x,y,z)");
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat3("Size Min", (float*)&emitter_data.initial_size[MIN], "%.1f");
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat3("Size Max", (float*)&emitter_data.initial_size[MAX], "%.1f");


	// Initial Position
	ImGui::Text("Initial Position (x,y,z)");
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat3("Pos Min", (float*)&emitter_data.initial_position[MIN], "%.1f");
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat3("Pos Max", (float*)&emitter_data.initial_position[MAX], "%.1f");


	// Size Per lifetime
	ImGui::Text("Size per lifetime (x,y,z)");
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat3("Size per Life Min", (float*)&emitter_data.size_per_lifetime[MIN], "%.1f");
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat3("Size per Life Max", (float*)&emitter_data.size_per_lifetime[MAX], "%.1f");


	// Rotation Per lifetime
	ImGui::Text("Rotation Per lifetime (Angle)");
	ImGui::SetNextItemWidth(50.0f);
	ImGui::InputFloat("Rot per life Min", &emitter_data.rotation_per_lifetime[MIN]);
	ImGui::SetNextItemWidth(50.0f);
	ImGui::InputFloat("Rot per life Max", &emitter_data.rotation_per_lifetime[MAX]);

	// Velocity Per lifetime
	ImGui::Text("Velocity per lifetime (x,y,z)");
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat3("Vel per Life Min", (float*)&emitter_data.velocity_per_lifetime[MIN], "%.1f");
	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputFloat3("Vel per Life Max", (float*)&emitter_data.velocity_per_lifetime[MAX], "%.1f");

	// Shader Selection
	SelectVertexShader(emitter_data.vs_id);
	SelectGeometryShader(emitter_data.geo_id);
	SelectPixelShader(emitter_data.ps_id);

	SelectBlendOptions();

	ImGui::SetNextItemWidth(TEXT_WIDTH);
	ImGui::InputTextWithHint("sprite name", "Name", particle_name, IM_ARRAYSIZE(particle_name));
	if (ImGui::Button("Save"))
	{
		SaveEmitter(particle_name);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{

	}
	ImGui::SameLine();
	if (ImGui::Button("Render"))
	{
		auto scene = (EffectTool*)SCENE->LoadScene("EffectTool");
		if (scene)
			scene->AddEmitter(emitter_data);
	}
}

void WG_EffectWindow::SelectBlendOptions()
{
	static bool bZbufferComp = false;
	static bool bZbufferWrite = false;
	static bool bAlphaBlending = false;
	static bool bAlphaTesting = false;
	static bool bWireFrame = false;
	ImGui::Checkbox("Z buffer Comp", &bZbufferComp);
	ImGui::SameLine();
	ImGui::Checkbox("Z buffer Write", &bZbufferWrite);
	ImGui::Checkbox("AlphaBlending", &bAlphaBlending);
	ImGui::SameLine();
	ImGui::Checkbox("AlphaTesting", &bAlphaTesting);
	ImGui::Checkbox("WireFrame", &bWireFrame);

	auto scene = (EffectTool*)SCENE->LoadScene("EffectTool");
	scene->bZbufferComp		= bZbufferComp;
	scene->bZbufferWrite	= bZbufferWrite;
	scene->bAlphaBlending	= bAlphaBlending;
	scene->bWireFrame		= bWireFrame;

	// 알파 테스팅?
}

void WG_EffectWindow::SelectFrame(int& max_frame, int& cur_frame)
{
	// currentFrame 선택
	ImGui::SetNextItemWidth(200.0f);
	ImGui::SliderInt("Frame", &cur_frame, 1, max_frame);
	// MaxFrame 선택
	ImGui::SetNextItemWidth(100.0f);
	ImGui::InputInt("Max Frame", &max_frame);
	

	static bool bPlay = false;
	static float timer = cur_frame;
	// Play 버튼
	if (ImGui::Button("Play"))
	{
		bPlay = true;
	}
	// Pause
	ImGui::SameLine();
	if (ImGui::Button("Pause"))
	{
		bPlay = false;
	}
	// Stop
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		bPlay = false;
		cur_frame = 1;
		timer = cur_frame;
	}

	if (bPlay)
	{
		timer += TIMER->GetDeltaTime();
		if (timer > max_frame + 1)
			timer -= max_frame;
		cur_frame = (int)timer;
	}

}

void WG_EffectWindow::SelectUV(vector<pair<POINT, POINT>>& list, int& max_frame)
{
	static int start[2] = { 0, 0 };
	static int end[2] = { 0, 0 };
	ImGui::SetNextItemWidth(100.0f);
	ImGui::InputInt2("Start UV", start);
	ImGui::SetNextItemWidth(100.0f);
	ImGui::InputInt2("End UV", end);

	if (ImGui::Button("Add"))
	{
		if (list.size() == max_frame)
			max_frame++;
		list.push_back({ {start[0], start[1]}, { end[0], end[1]} });
	}
	ImGui::SetNextItemWidth(LISTBOX_WIDTH + 100);
	static int item_current_idx = 0;
	if (ImGui::BeginListBox("UV per Frame"))
	{
		for (int n = 0; n < list.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			string str = "[" + to_string(n + 1) + "]"
				+ " Start (" + to_string(list[n].first.x) + ", " + to_string(list[n].first.y) + ")"
				+ " End (" + to_string(list[n].second.x) + ", " + to_string(list[n].second.y) + ")";
			if (ImGui::Selectable(str.c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	if (ImGui::Button("Delete Last Frame"))
	{
		if(list.size() > 0)
			list.pop_back();
	}
	
}

void WG_EffectWindow::SelectVertexShader(string& id)
{
	static int item_current_idx = 0;

	auto vs_set = RESOURCE->GetTotalVSID();
	vector<string> vs_vec(vs_set.begin(), vs_set.end());

	// 위에서 설정된 vs_id가 있다면 그 값으로 item_current_idx 변경
	for (int i = 0; i < vs_vec.size(); i++)
	{
		if (vs_vec[i] == id)
			item_current_idx = i;
	}

	ImGui::SetNextItemWidth(LISTBOX_WIDTH);
	if (ImGui::BeginListBox("Vertex Shader"))
	{
		for (int n = 0; n < vs_vec.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(vs_vec[n].c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	id = vs_vec[item_current_idx];
}

void WG_EffectWindow::SelectGeometryShader(string& id)
{
	static int item_current_idx = 0;

	auto gs_set = RESOURCE->GetTotalGSID();
	vector<string> gs_vec(gs_set.begin(), gs_set.end());

	// 위에서 설정된 gs_id가 있다면 그 값으로 item_current_idx 변경
	for (int i = 0; i < gs_vec.size(); i++)
	{
		if (gs_vec[i] == id)
			item_current_idx = i;
	}

	ImGui::SetNextItemWidth(LISTBOX_WIDTH);
	if (ImGui::BeginListBox("Geometry Shader"))
	{
		for (int n = 0; n < gs_vec.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(gs_vec[n].c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	id = gs_vec[item_current_idx];
}

void WG_EffectWindow::SelectPixelShader(string& id)
{
	static int item_current_idx = 0;

	auto ps_set = RESOURCE->GetTotalPSID();
	vector<string> ps_vec(ps_set.begin(), ps_set.end());

	// 위에서 설정된 ps_id가 있다면 그 값으로 item_current_idx 변경
	for (int i = 0; i < ps_vec.size(); i++)
	{
		if (ps_vec[i] == id)
			item_current_idx = i;
	}

	ImGui::SetNextItemWidth(LISTBOX_WIDTH);
	if (ImGui::BeginListBox("Pixel Shader"))
	{
		for (int n = 0; n < ps_vec.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(ps_vec[n].c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	id = ps_vec[item_current_idx];
}

void WG_EffectWindow::SelectTexture(string& id)
{
	static int item_current_idx = 0;

	auto tex_set = RESOURCE->GetTotalTexID();
	vector<string> tex_vec(tex_set.begin(), tex_set.end());

	// 위에서 설정된 tex_id가 있다면 그 값으로 item_current_idx 변경
	for (int i = 0; i < tex_vec.size(); i++)
	{
		if (tex_vec[i] == id)
			item_current_idx = i;
	}

	ImGui::SetNextItemWidth(LISTBOX_WIDTH);
	if (ImGui::BeginListBox("Texture"))
	{
		for (int n = 0; n < tex_vec.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(tex_vec[n].c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	id = tex_vec[item_current_idx];
}

void WG_EffectWindow::SelectSprite(string& id)
{
	static int item_current_idx = 0;

	// 가지고 있는 스프라이트 아이디를 리스트화 하기 위해 vector로 id 받아오기
	auto sprite_id_set = RESOURCE->GetTotalSpriteID();
	vector<string> sprite_id(sprite_id_set.begin(), sprite_id_set.end());

	// 위에서 설정된 tex_id가 있다면 그 값으로 item_current_idx 변경
	for (int i = 0; i < sprite_id.size(); i++)
	{
		if (sprite_id[i] == id)
			item_current_idx = i;
	}

	ImGui::SetNextItemWidth(LISTBOX_WIDTH);
	if (ImGui::BeginListBox("Sprite"))
	{
		for (int n = 0; n < sprite_id.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(sprite_id[n].c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	if(sprite_id.size() > 0)
		id = sprite_id[item_current_idx];
}

void WG_EffectWindow::SaveUVSprite(string name)
{
	string sheetName = name;
	if (sheetName.size() == 0)
		return;

	auto sheet = DATA->AddNewSheet(sheetName);

	auto effect = sheet->AddItem(name);
	auto list = sheet->AddItem("uvList");
	
	sheet->AddCategory("MaxFrame");
	sheet->AddCategory("tex_id");
	sheet->AddCategory("type");

	for (int i = 0; i < uv_sprite_data.uv_list.size(); i++)
		sheet->AddCategory(to_string(i+1));

	effect->SetValue("MaxFrame", to_string(uv_sprite_data.max_frame));
	effect->SetValue("tex_id", uv_sprite_data.tex_id);
	effect->SetValue("type", to_string(UV_SPRITE));

	for (int i = 0; i < uv_sprite_data.uv_list.size(); i++)
	{
		string uvStr = to_string(uv_sprite_data.uv_list[i].first.x) + " " + to_string(uv_sprite_data.uv_list[i].first.y) + " " 
			+ to_string(uv_sprite_data.uv_list[i].second.x) + " " + to_string(uv_sprite_data.uv_list[i].second.y);
		list->SetValue(to_string(i + 1), uvStr);
	}
		
	DATA->SaveSheetFile(sheetName);

	RESOURCE->SaveSprite(name, make_shared<UVSprite>(uv_sprite_data));
}

void WG_EffectWindow::SaveTexSprite(string name)
{
	string sheetName = name;
	if (sheetName.size() == 0)
		return;

	auto sheet = DATA->AddNewSheet(sheetName);

	auto effect = sheet->AddItem(name);
	auto list = sheet->AddItem("texList");

	sheet->AddCategory("MaxFrame");
	sheet->AddCategory("type");

	for (int i = 0; i < tex_sprite_data.tex_id_list.size(); i++)
		sheet->AddCategory(to_string(i + 1));

	effect->SetValue("MaxFrame", to_string(tex_sprite_data.max_frame));
	effect->SetValue("type", to_string(TEX_SPRITE));

	for (int i = 0; i < tex_sprite_data.tex_id_list.size(); i++)
	{
		list->SetValue(to_string(i + 1), tex_sprite_data.tex_id_list[i]);
	}

	DATA->SaveSheetFile(sheetName);

	// 세이브 한 스프라이트를 스프라이트 리스트에 추가한다.
	RESOURCE->SaveSprite(name, make_shared<TextureSprite>(tex_sprite_data));
}

void WG_EffectWindow::SaveEmitter(string name)
{
	string sheetName = name;
	if (sheetName.size() == 0)
		return;

	auto sheet = DATA->AddNewSheet(sheetName);

	auto effect = sheet->AddItem(name);

	// 카테고리 추가
	sheet->AddCategory("type");

	sheet->AddCategory("sprite_id");

	sheet->AddCategory("emit_per_second");

	sheet->AddCategory("color");

	sheet->AddCategory("life_time");

	sheet->AddCategory("initial_size");
	sheet->AddCategory("initial_position");

	sheet->AddCategory("size_per_lifetime");
	sheet->AddCategory("rotation_per_lifetime");
	sheet->AddCategory("velocity_per_lifetime");

	sheet->AddCategory("vs_id");
	sheet->AddCategory("geo_id");
	sheet->AddCategory("ps_id");

	// 값 추가

	// type
	effect->SetValue("type", to_string(EMITTER));

	// sprite_id
	effect->SetValue("sprite_id", emitter_data.sprite_id);

	// emit_per_second
	effect->SetValue("emit_per_second", to_string(emitter_data.emit_per_second));

	string fmt = "";

	// color
	fmt = to_string(emitter_data.color.x) + " " + to_string(emitter_data.color.y) + " " + to_string(emitter_data.color.z) + " " + to_string(emitter_data.color.w);
	effect->SetValue("color", fmt);

	// life_time
	fmt = to_string(emitter_data.life_time[0]) + " " + to_string(emitter_data.life_time[1]);
	effect->SetValue("life_time", fmt);

	// initial_size
	fmt = to_string(emitter_data.initial_size[0].x) + " " + to_string(emitter_data.initial_size[0].y) + " " + to_string(emitter_data.initial_size[0].z) + "~"
		+ to_string(emitter_data.initial_size[1].x) + " " + to_string(emitter_data.initial_size[1].y) + " " + to_string(emitter_data.initial_size[1].z);
	effect->SetValue("initial_size", fmt);

	// initial_position
	fmt = to_string(emitter_data.initial_position[0].x) + " " + to_string(emitter_data.initial_position[0].y) + " " + to_string(emitter_data.initial_position[0].z) + "~"
		+ to_string(emitter_data.initial_position[1].x) + " " + to_string(emitter_data.initial_position[1].y) + " " + to_string(emitter_data.initial_position[1].z);
	effect->SetValue("initial_position", fmt);

	// size_per_lifetime
	fmt = to_string(emitter_data.size_per_lifetime[0].x) + " " + to_string(emitter_data.size_per_lifetime[0].y) + " " + to_string(emitter_data.size_per_lifetime[0].z) + "~"
		+ to_string(emitter_data.size_per_lifetime[1].x) + " " + to_string(emitter_data.size_per_lifetime[1].y) + " " + to_string(emitter_data.size_per_lifetime[1].z);
	effect->SetValue("size_per_lifetime", fmt);
	// rotation_per_lifetime
	fmt = to_string(emitter_data.rotation_per_lifetime[0]) + " " + to_string(emitter_data.rotation_per_lifetime[1]);
	effect->SetValue("rotation_per_lifetime", fmt);
	// velocity_per_lifetime
	fmt = to_string(emitter_data.velocity_per_lifetime[0].x) + " " + to_string(emitter_data.velocity_per_lifetime[0].y) + " " + to_string(emitter_data.velocity_per_lifetime[0].z) + "~"
		+ to_string(emitter_data.velocity_per_lifetime[1].x) + " " + to_string(emitter_data.velocity_per_lifetime[1].y) + " " + to_string(emitter_data.velocity_per_lifetime[1].z);
	effect->SetValue("velocity_per_lifetime", fmt);

	// vs_id
	effect->SetValue("vs_id", emitter_data.vs_id);
	// geo_id
	effect->SetValue("geo_id", emitter_data.geo_id);
	// ps_id
	effect->SetValue("ps_id", emitter_data.ps_id);


	DATA->SaveSheetFile(sheetName);
}

void WG_EffectWindow::LoadingSpriteData(string path)
{
	auto strs1 = split(path, '\\');
	auto name = strs1[max((int)strs1.size() - 1, 0)];
	auto strs2 = split(name, '.');
	name = strs2[0];

	auto sprite = RESOURCE->UseResource<Sprite>(name);

	if (!sprite)
	{
		if (!RESOURCE->ImportSprite(path))
			return;
		sprite = RESOURCE->UseResource<Sprite>(name);
	}

	UVSprite* uv_sprite = nullptr;
	TextureSprite* tex_sprite = nullptr;

	switch (sprite->type)
	{
	case UV:
		type_ = UV_TAB;
		uv_sprite = (UVSprite*)sprite;
		if(uv_sprite)
			uv_sprite_data = *uv_sprite;
		break;
	case TEX:
		type_ = TEX_TAB;
		tex_sprite = (TextureSprite*)sprite;
		if (tex_sprite)
			tex_sprite_data = *tex_sprite;
		break;
	}
	//DATA->LoadSheetFile(loading_data_id_);
	//auto sheet = DATA->LoadSheet(loading_data_id_);

	//auto strs1 = split(loading_data_id_, '\\');
	//auto name = strs1[max((int)strs1.size() - 1, 0)];

	//auto strs2 = split(name, '.');
	//name = strs2[0];

	//auto item = sheet->LoadItem(name);
	//if (item == NULL)
	//	return;

	//string str_type = item->GetValue("type");

	//if (str_type == "")
	//	return;

	//E_Effect type = (E_Effect)stoi(str_type);

	//switch (type)
	//{
	//case UV_SPRITE:
	//{
	//	type_ = UV_TAB;
	//	uv_sprite_data.max_frame = stoi(item->GetValue("MaxFrame"));
	//	uv_sprite_data.tex_id = item->GetValue("tex_id");

	//	// TODO : UVList 파싱... 데이터 형태 수정해야할듯
	//	auto uvListItem = sheet->LoadItem("uvList");
	//	// 리스트에서 가장 높은 프레임의 값을 가져온다.
	//	int max = 0;
	//	for (int i = 1; i <= uv_sprite_data.max_frame + 1; i++)
	//	{
	//		if (uvListItem->values[to_string(i)] == "")
	//		{
	//			max = i-1;
	//			break;
	//		}
	//			
	//	}
	//	// 가장 낮은 프레임부터 가장 높은 프레임까지 파싱해서 uv값을 넣어준다.
	//	uv_sprite_data.uv_list.clear();
	//	for (int i = 0; i < max; i++)
	//	{
	//		// 0 0 25 25 형식
	//		auto splited_str = split(uvListItem->values[to_string(i + 1)], ' ');
	//		uv_sprite_data.uv_list.push_back({ { stol(splited_str[0]), stol(splited_str[1]) }, { stol(splited_str[2]), stol(splited_str[3]) } });
	//	}
	//	
	//	// 로딩한 스프라이트를 리스트에 넣는다.
	//	sprite_list.insert({ name, make_shared<UVSprite>(uv_sprite_data) });
	//} break;
	//case TEX_SPRITE:
	//{
	//	type_ = TEX_TAB;
	//	tex_sprite_data.max_frame = stoi(item->GetValue("MaxFrame"));

	//	// TODO : 데이터 형태 수정해야할듯
	//	auto texListItem = sheet->LoadItem("texList");
	//	// 리스트에서 가장 높은 프레임의 값을 가져온다.
	//	int max = 0;
	//	for (int i = 1; i <= tex_sprite_data.max_frame + 1; i++)
	//	{
	//		if (texListItem->values[to_string(i)] == "")
	//		{
	//			max = i-1;
	//			break;
	//		}
	//	}
	//	// 가장 낮은 프레임부터 가장 높은 프레임까지 파싱해서 tex_id값을 넣어준다.
	//	tex_sprite_data.tex_id_list.clear();
	//	for (int i = 0; i < max; i++)
	//	{
	//		tex_sprite_data.tex_id_list.push_back(texListItem->values[to_string(i + 1)]);
	//	}
	//	// 로딩한 스프라이트를 리스트에 넣는다.
	//	sprite_list.insert({ name, make_shared<TextureSprite>(tex_sprite_data) });
	//} break;
	//case PARTICLES:
	//{
	//	type_ = PARTICLE_TAB;
	//} break;
	//default:
	//{
	//	
	//} break;
	//}

	//loading_data_id_ = "";
}

void WG_EffectWindow::LoadingEmitterData(string path)
{
	auto strs1 = split(path, '\\');
	auto name = strs1[max((int)strs1.size() - 1, 0)];
	auto strs2 = split(name, '.');
	name = strs2[0];

	auto sheet = DATA->LoadSheet(name);

	if (sheet == NULL)
	{
		DATA->LoadSheetFile(path);
		sheet = DATA->LoadSheet(name);
	}
		
	if (sheet == NULL)
		return;

	auto effect = sheet->LoadItem(name);

	// type
	effect->GetValue("type");

	// sprite_id
	emitter_data.sprite_id			= effect->GetValue("sprite_id");

	// emit_per_second
	emitter_data.emit_per_second	= stoi(effect->GetValue("emit_per_second"));

	vector<string> splited_str;
	vector<string> splited_str2;

	// color
	{
		splited_str = split(effect->GetValue("color"), ' ');
		if (splited_str.size() < 4)
			return;
		emitter_data.color.x = stof(splited_str[0]);
		emitter_data.color.y = stof(splited_str[1]);
		emitter_data.color.z = stof(splited_str[2]);
		emitter_data.color.w = stof(splited_str[3]);
	}
	

	// life_time
	{
		splited_str = split(effect->GetValue("life_time"), ' ');
		if (splited_str.size() < 2)
			return;
		emitter_data.life_time[0] = stof(splited_str[0]);
		emitter_data.life_time[1] = stof(splited_str[1]);
	}
	

	// initial_size
	{
		splited_str = split(effect->GetValue("initial_size"), '~');
		if (splited_str.size() < 2)
			return;
		// min
		splited_str2 = split(splited_str[0], ' ');
		if (splited_str2.size() < 3)
			return;
		emitter_data.initial_size[0].x = stof(splited_str2[0]);
		emitter_data.initial_size[0].y = stof(splited_str2[1]);
		emitter_data.initial_size[0].z = stof(splited_str2[2]);
		// max
		splited_str2 = split(splited_str[1], ' ');
		if (splited_str2.size() < 3)
			return;
		emitter_data.initial_size[1].x = stof(splited_str2[0]);
		emitter_data.initial_size[1].y = stof(splited_str2[1]);
		emitter_data.initial_size[1].z = stof(splited_str2[2]);
	}

	// initial_position
	{
		splited_str = split(effect->GetValue("initial_position"), '~');
		if (splited_str.size() < 2)
			return;
		// min
		splited_str2 = split(splited_str[0], ' ');
		if (splited_str2.size() < 3)
			return;
		emitter_data.initial_position[0].x = stof(splited_str2[0]);
		emitter_data.initial_position[0].y = stof(splited_str2[1]);
		emitter_data.initial_position[0].z = stof(splited_str2[2]);
		// max
		splited_str2 = split(splited_str[1], ' ');
		if (splited_str2.size() < 3)
			return;
		emitter_data.initial_position[1].x = stof(splited_str2[0]);
		emitter_data.initial_position[1].y = stof(splited_str2[1]);
		emitter_data.initial_position[1].z = stof(splited_str2[2]);
	}

	// size_per_lifetime
	{
		splited_str = split(effect->GetValue("size_per_lifetime"), '~');
		if (splited_str.size() < 2)
			return;
		// min
		splited_str2 = split(splited_str[0], ' ');
		if (splited_str2.size() < 3)
			return;
		emitter_data.size_per_lifetime[0].x = stof(splited_str2[0]);
		emitter_data.size_per_lifetime[0].y = stof(splited_str2[1]);
		emitter_data.size_per_lifetime[0].z = stof(splited_str2[2]);
		// max
		splited_str2 = split(splited_str[1], ' ');
		if (splited_str2.size() < 3)
			return;
		emitter_data.size_per_lifetime[1].x = stof(splited_str2[0]);
		emitter_data.size_per_lifetime[1].y = stof(splited_str2[1]);
		emitter_data.size_per_lifetime[1].z = stof(splited_str2[2]);
	}

	// rotation_per_lifetime
	{
		splited_str = split(effect->GetValue("rotation_per_lifetime"), ' ');
		if (splited_str.size() < 2)
			return;
		emitter_data.rotation_per_lifetime[0] = stof(splited_str[0]);
		emitter_data.rotation_per_lifetime[1] = stof(splited_str[1]);
	}

	// velocity_per_lifetime
	{
		splited_str = split(effect->GetValue("velocity_per_lifetime"), '~');
		if (splited_str.size() < 2)
			return;
		// min
		splited_str2 = split(splited_str[0], ' ');
		if (splited_str2.size() < 3)
			return;
		emitter_data.velocity_per_lifetime[0].x = stof(splited_str2[0]);
		emitter_data.velocity_per_lifetime[0].y = stof(splited_str2[1]);
		emitter_data.velocity_per_lifetime[0].z = stof(splited_str2[2]);
		// max
		splited_str2 = split(splited_str[1], ' ');
		if (splited_str2.size() < 3)
			return;
		emitter_data.velocity_per_lifetime[1].x = stof(splited_str2[0]);
		emitter_data.velocity_per_lifetime[1].y = stof(splited_str2[1]);
		emitter_data.velocity_per_lifetime[1].z = stof(splited_str2[2]);
	}

	emitter_data.vs_id		= effect->GetValue("vs_id");
	emitter_data.geo_id		= effect->GetValue("geo_id");
	emitter_data.ps_id		= effect->GetValue("ps_id");

	type_ = EMITTER_TAB;
}
