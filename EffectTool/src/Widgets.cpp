#include "Widgets.h"
#include "SceneMgr.h"
#include "EffectTool.h"

using namespace KGCA41B;

#define LISTBOX_WIDTH 150.0f
#define TEXT_WIDTH 150.0f

#define GET_VARIABLE_NAME(n) #n

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
			GUI->AddWidget("EffectTool", new WG_EffectWindow);
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
	ImGui::SetNextWindowSize(ImVec2(600, 600));
	ImGui::Begin("Effect Tool", &open_, ImGuiWindowFlags_MenuBar);
	{
		ImGui::BeginMenuBar();
		{
			if(loading_data_id_.size() != 0)
				LoadingEffectData();

			if (ImGui::BeginMenu("New Effect"))
			{
				if (ImGui::MenuItem("UV Changing Sprite Effect"))
				{
					type_ = UV_SPRITE;
				}
				if (ImGui::MenuItem("Texture Changing Sprite Effect"))
				{
					type_ = TEX_SPRITE;
				}
				if (ImGui::MenuItem("Particles Effect"))
				{
					type_ = PARTICLES;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Load Effect"))
			{
				if (ImGui::MenuItem("Loading From DataManager"))
				{
					auto widget = new WG_DataViewer;
					GUI->AddWidget("FileViewer", widget);
				}
				ImGui::EndMenu();
			}
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
		case PARTICLES:
		{
			ParticlesBoard();
		}
		break;
		}
	}
	ImGui::End();
}

void WG_EffectWindow::UVSpriteBoard()
{
	ImVec2 img_size = { 200, 200 };
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcItemSize(img_size, img_size.x, img_size.y).x) / 4);

	Texture* tex = RESOURCE->UseResource<Texture>(uv_sprite_data.texture_id);
	if (tex)
		ImGui::Image((void*)tex->srv.Get(), img_size);

	if (uv_sprite_data.uv_list.size() > 0)
	{
		ImGui::SameLine();
		auto uv = uv_sprite_data.uv_list[min(uv_sprite_data.cur_frame - 1, (int)uv_sprite_data.uv_list.size() - 1)];
		auto texture = RESOURCE->UseResource<Texture>(uv_sprite_data.texture_id);
		float tex_width = (float)texture->texture_desc.Width;
		float tex_height = (float)texture->texture_desc.Height;
		ImVec2 start(uv.first.x / tex_width, uv.first.y / tex_height);
		ImVec2 end(uv.second.x / tex_width, uv.second.y / tex_height);
		ImGui::Image((void*)tex->srv.Get(), img_size, start, end);
	}
	


	// 프레임 선택
	SelectFrame(uv_sprite_data.max_frame, uv_sprite_data.cur_frame);

	// 텍스쳐 선택
	SelectTexture(uv_sprite_data.texture_id);
	

	// UV 값 설정
	if (uv_sprite_data.uv_list.size() > uv_sprite_data.max_frame)
		uv_sprite_data.uv_list.resize(uv_sprite_data.max_frame);
	ImGui::SameLine();
	SelectUV(uv_sprite_data.uv_list, uv_sprite_data.max_frame);

	
	// VS 선택
	SelectVertexShader(uv_sprite_data.vs_id);
	
	// PS 선택
	ImGui::SameLine();
	SelectPixelShader(uv_sprite_data.ps_id);

	// 블랜딩 옵션들
	SelectBlendOptions();

	ImGui::SetNextItemWidth(TEXT_WIDTH);
	ImGui::InputTextWithHint("effectName", "Name", uv_sprite_data.effect_name, IM_ARRAYSIZE(uv_sprite_data.effect_name));

	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		// TODO : 데이터 테이블을 통한 저장
		SaveUVSprite(uv_sprite_data);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		uv_sprite_data.max_frame = 10;
		uv_sprite_data.cur_frame = 1;
		uv_sprite_data.texture_id = "";
		uv_sprite_data.uv_list.clear();
		uv_sprite_data.vs_id = "";
		uv_sprite_data.ps_id = "";
		memset(uv_sprite_data.effect_name, 0, sizeof(char) * strlen(uv_sprite_data.effect_name));
	}
	ImGui::SameLine();
	if (ImGui::Button("Render"))
	{
		auto scene = SCENE->LoadScene("EffectTool");
		EffectTool* effect_scene = dynamic_cast<EffectTool*>(scene);
		if (effect_scene)
			effect_scene->uv_sprite_.SetUVSprite(effect_scene->reg_effect_tool_, uv_sprite_data);
	}
}

void WG_EffectWindow::TexSpriteBoard()
{
	ImVec2 img_size = { 200, 200 };
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcItemSize(img_size, img_size.x, img_size.y).x) / 4);

	Texture* tex = RESOURCE->UseResource<Texture>(tex_sprite_data.texture_id);
	if (tex)
		ImGui::Image((void*)tex->srv.Get(), img_size);

	
	if (tex_sprite_data.tex_id_list.size() > 0)
	{
		ImGui::SameLine();
		tex = RESOURCE->UseResource<Texture>(tex_sprite_data.tex_id_list[min(tex_sprite_data.cur_frame - 1, (int)tex_sprite_data.tex_id_list.size() - 1)]);
		ImGui::Image((void*)tex->srv.Get(), img_size);
	}
		

	// 프레임 선택
	SelectFrame(tex_sprite_data.max_frame, tex_sprite_data.cur_frame);

	// 텍스쳐 선택
	SelectTexture(tex_sprite_data.texture_id);

	if (tex_sprite_data.tex_id_list.size() > tex_sprite_data.max_frame)
		tex_sprite_data.tex_id_list.resize(tex_sprite_data.max_frame);

	// 텍스쳐 배열 추가
	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		if (tex_sprite_data.tex_id_list.size() == tex_sprite_data.max_frame)
			tex_sprite_data.max_frame++;
		tex_sprite_data.tex_id_list.push_back(tex_sprite_data.texture_id);
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
	ImGui::SameLine();
	if (ImGui::Button("Delete Last Frame"))
	{
		if (tex_sprite_data.tex_id_list.size() > 0)
			tex_sprite_data.tex_id_list.pop_back();
	}

	// VS 선택
	SelectVertexShader(tex_sprite_data.vs_id);

	// PS 선택
	ImGui::SameLine();
	SelectPixelShader(tex_sprite_data.ps_id);

	// 블랜딩 옵션들
	SelectBlendOptions();

	ImGui::SetNextItemWidth(TEXT_WIDTH);
	ImGui::InputTextWithHint("effectName", "Name", tex_sprite_data.effect_name, IM_ARRAYSIZE(tex_sprite_data.effect_name));
	ImGui::SameLine();

	if (ImGui::Button("Save"))
	{
		// TODO : 데이터 테이블을 통한 저장
		SaveTexSprite(tex_sprite_data);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		 tex_sprite_data.max_frame = 10;
		 tex_sprite_data.cur_frame = 1;
		 tex_sprite_data.texture_id = "";
		 tex_sprite_data.tex_id_list.clear();
		 tex_sprite_data.vs_id = "";
		 tex_sprite_data.ps_id = "";
		 memset(tex_sprite_data.effect_name, 0, sizeof(char) * strlen(tex_sprite_data.effect_name));
	}
	ImGui::SameLine();
	if (ImGui::Button("Render"))
	{
		auto scene = SCENE->LoadScene("EffectTool");
		EffectTool* effect_scene = dynamic_cast<EffectTool*>(scene);
		if (effect_scene)
			effect_scene->tex_sprite_.SetTexSprite(effect_scene->reg_effect_tool_, tex_sprite_data);
	}
}

void WG_EffectWindow::ParticlesBoard()
{
	
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
	ImGui::SameLine();
	ImGui::Checkbox("AlphaBlending", &bAlphaBlending);
	ImGui::SameLine();
	ImGui::Checkbox("AlphaTesting", &bAlphaTesting);
	ImGui::SameLine();
	ImGui::Checkbox("WireFrame", &bWireFrame);

	//if(bZbufferComp && bZbufferWrite)
	//	DX11APP->GetDeviceContext()->OMSetDepthStencilState(DX11APP->GetCommonStates()->DepthDefault(), 0xff);
	//else if(bZbufferComp)
	//	DX11APP->GetDeviceContext()->OMSetDepthStencilState(DX11APP->GetCommonStates()->DepthRead(), 0xff);
	//else if (bZbufferWrite) // 아직 모름
	//	DX11APP->GetDeviceContext()->OMSetDepthStencilState(DX11APP->GetCommonStates()->DepthRead(), 0xff);
	//else
	//	DX11APP->GetDeviceContext()->OMSetDepthStencilState(DX11APP->GetCommonStates()->DepthNone(), 0xff);

	//if(bAlphaBlending && bAlphaTesting)
	//	DX11APP->GetDeviceContext()->OMSetBlendState(DX11APP->GetCommonStates()->Additive(), 0, -1);
	//else if(bAlphaBlending)
	//	DX11APP->GetDeviceContext()->OMSetBlendState(DX11APP->GetCommonStates()->AlphaBlend(), 0, -1);
	//else
	//	DX11APP->GetDeviceContext()->OMSetBlendState(DX11APP->GetCommonStates()->Opaque(), 0, -1);

}

void WG_EffectWindow::SelectFrame(int& max_frame, int& cur_frame)
{
	// MaxFrame 선택
	ImGui::SetNextItemWidth(100.0f);
	ImGui::InputInt("Max Frame", &max_frame);

	// currentFrame 선택
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.0f);
	ImGui::SliderInt("Frame", &cur_frame, 1, max_frame);

	static bool bPlay = false;
	static float timer = cur_frame;
	// Play 버튼
	ImGui::SameLine();
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
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.0f);
	ImGui::InputInt2("End UV", end);
	ImGui::SameLine();

	if (ImGui::Button("Add"))
	{
		if (list.size() == max_frame)
			max_frame++;
		list.push_back({ {start[0], start[1]}, { end[0], end[1]} });
	}
	//ImGui::SameLine();
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
	ImGui::SameLine();
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

void WG_EffectWindow::SaveUVSprite(UVSpriteData& data)
{
	string sheetName = data.effect_name;
	if (sheetName.size() == 0)
		return;
	sheetName += ".csv";

	auto sheet = DATA->AddNewSheet(sheetName);

	auto effect = sheet->AddItem(data.effect_name);
	auto list = sheet->AddItem("uvList");
	
	sheet->AddCategory("MaxFrame");
	sheet->AddCategory("tex_id");
	sheet->AddCategory("vs_id");
	sheet->AddCategory("ps_id");
	sheet->AddCategory("type");

	for (int i = 0; i < data.uv_list.size(); i++)
		sheet->AddCategory(to_string(i+1));

	effect->SetValue("MaxFrame", to_string(data.max_frame));
	effect->SetValue("tex_id", data.texture_id);
	effect->SetValue("vs_id", data.vs_id);
	effect->SetValue("ps_id", data.ps_id);
	effect->SetValue("type", to_string(UV_SPRITE));

	for (int i = 0; i < data.uv_list.size(); i++)
	{
		string uvStr = to_string(data.uv_list[i].first.x) + " " + to_string(data.uv_list[i].first.y) + " " + to_string(data.uv_list[i].second.x) + " " + to_string(data.uv_list[i].second.y);
		list->SetValue(to_string(i + 1), uvStr);
	}
		
	DATA->SaveSheetFile(sheetName);
}

void WG_EffectWindow::SaveTexSprite(TexSpriteData& data)
{
	string sheetName = data.effect_name;
	if (sheetName.size() == 0)
		return;
	sheetName += ".csv";

	auto sheet = DATA->AddNewSheet(sheetName);

	auto effect = sheet->AddItem(data.effect_name);
	auto list = sheet->AddItem("texList");

	sheet->AddCategory("MaxFrame");
	sheet->AddCategory("vs_id");
	sheet->AddCategory("ps_id");
	sheet->AddCategory("type");

	for (int i = 0; i < data.tex_id_list.size(); i++)
		sheet->AddCategory(to_string(i + 1));

	effect->SetValue("MaxFrame", to_string(data.max_frame));
	effect->SetValue("vs_id", data.vs_id);
	effect->SetValue("ps_id", data.ps_id);
	effect->SetValue("type", to_string(TEX_SPRITE));

	for (int i = 0; i < data.tex_id_list.size(); i++)
	{
		list->SetValue(to_string(i + 1), data.tex_id_list[i]);
	}

	DATA->SaveSheetFile(sheetName);
}

void WG_EffectWindow::SaveParticles()
{
}

void WG_EffectWindow::LoadingEffectData()
{
	auto sheet = DATA->LoadSheet(loading_data_id_);

	auto strs = split(loading_data_id_, '.');
	auto name = strs[0];

	auto item = sheet->LoadItem(name);
	if (item == NULL)
		return;

	string str_type = item->GetValue("type");

	if (str_type == "")
		return;

	type_ = (E_Effect)stoi(str_type);

	switch (type_)
	{
	case UV_SPRITE:
	{
		type_ = UV_SPRITE;
		uv_sprite_data.max_frame = stoi(item->GetValue("MaxFrame"));
		uv_sprite_data.texture_id = item->GetValue("tex_id");
		uv_sprite_data.vs_id = item->GetValue("vs_id");
		uv_sprite_data.ps_id = item->GetValue("ps_id");

		// TODO : UVList 파싱... 데이터 형태 수정해야할듯
		auto uvListItem = sheet->LoadItem("uvList");
		// 리스트에서 가장 높은 프레임의 값을 가져온다.
		int max = 0;
		for (int i = 1; i <= uv_sprite_data.max_frame + 1; i++)
		{
			if (uvListItem->values[to_string(i)] == "")
			{
				max = i-1;
				break;
			}
				
		}
		// 가장 낮은 프레임부터 가장 높은 프레임까지 파싱해서 uv값을 넣어준다.
		uv_sprite_data.uv_list.clear();
		for (int i = 0; i < max; i++)
		{
			// 0 0 25 25 형식
			auto splited_str = split(uvListItem->values[to_string(i + 1)], ' ');
			uv_sprite_data.uv_list.push_back({ { stol(splited_str[0]), stol(splited_str[1]) }, { stol(splited_str[2]), stol(splited_str[3]) } });
		}
	} break;
	case TEX_SPRITE:
	{
		type_ = TEX_SPRITE;
		tex_sprite_data.max_frame = stoi(item->GetValue("MaxFrame"));
		tex_sprite_data.vs_id = item->GetValue("vs_id");
		tex_sprite_data.ps_id = item->GetValue("ps_id");

		// TODO : 데이터 형태 수정해야할듯
		auto texListItem = sheet->LoadItem("texList");
		// 리스트에서 가장 높은 프레임의 값을 가져온다.
		int max = 0;
		for (int i = 1; i <= uv_sprite_data.max_frame + 1; i++)
		{
			if (texListItem->values[to_string(i + 1)] == "")
			{
				max = i-1;
				break;
			}
		}
		// 가장 낮은 프레임부터 가장 높은 프레임까지 파싱해서 tex_id값을 넣어준다.
		tex_sprite_data.tex_id_list.clear();
		for (int i = 0; i < max; i++)
		{
			tex_sprite_data.tex_id_list.push_back(texListItem->values[to_string(i + 1)]);
		}
	} break;
	case PARTICLES:
	{
		type_ = PARTICLES;
	} break;
	default:
	{
		
	} break;
	}

	loading_data_id_ = "";
}

void WG_DataViewer::Update()
{
	ImGui::SetCurrentContext(GUI->GetContext());
}

void WG_DataViewer::Render()
{
	ImGui::Begin("Load Effect File", &open_);
	{
		static int item_current_idx = 0;

		DATA->LoadAllData();
		auto id_list = DATA->GetAllDataSheetID();
		if (ImGui::BeginListBox("Data File"))
		{
			for (int n = 0; n < id_list.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(id_list[n].c_str(), is_selected))
					item_current_idx = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			if (id_list.size() == 0)
			{
			}
			else
			{
				auto tool_window = dynamic_cast<WG_EffectWindow*>(GUI->FindWidget("EffectTool"));
				tool_window->set_loading_data_id(id_list[item_current_idx]);
			}
			GUI->FindWidget("FileViewer")->open_ = false;
			
			
		}

	}
	ImGui::End();
}
