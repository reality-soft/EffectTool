#pragma once
#include "Engine_include.h"
#include "DataTypes.h"

namespace KGCA41B
{
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
	private:
		void	UVSpriteBoard();
		void	TexSpriteBoard();
		void	ParticlesBoard();
		void	SelectBlendOptions();
		void	SelectFrame(int& max_frame, int& cur_frame);
		void	SelectUV(vector<pair<POINT, POINT>>& list, int& max_frame);
		void	SelectVertexShader(string& id);
		void	SelectGeometryShader(string& id);
		void	SelectPixelShader(string& id);
		void	SelectTexture(string& id);

	private:
		void SaveUVSprite(UVSpriteData& data);
		void SaveTexSprite(TexSpriteData& data);
		void SaveParticles();

	private:
		string loading_data_id_;
		E_Effect type_;// = NONE;
		UVSpriteData uv_sprite_data;
		TexSpriteData tex_sprite_data;
	public:
		void set_loading_data_id(string loading_data_id) { loading_data_id_ = loading_data_id; }
		void LoadingEffectData();
	};

	class WG_DataViewer : public KGCA41B::GuiWidget
	{
		virtual void Update() override;
		virtual void Render() override;
	public:
		string	data_id_;
	};
}

