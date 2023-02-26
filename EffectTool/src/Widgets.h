#pragma once
#include "Engine_include.h"
#include "DataTypes.h"

namespace KGCA41B
{
	enum E_EffectToolTab
	{
		DEFAULT_TAB = 0,
		UV_TAB = 1,
		TEX_TAB = 2,
		EMITTER_TAB = 3,
	};

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
		void	FileBrowser();
		void	UVSpriteBoard();
		void	TexSpriteBoard();
		void	EmitterBoard();
		void	SelectBlendOptions();
		void	SelectFrame(int& max_frame, int& cur_frame);
		void	SelectUV(vector<pair<POINT, POINT>>& list, int& max_frame);
		void	SelectVertexShader(string& id);
		void	SelectGeometryShader(string& id);
		void	SelectPixelShader(string& id);
		void	SelectTexture(string& id);
		void	SelectSprite(string& id);
	private:
		void	SaveUVSprite(string name);
		void	SaveTexSprite(string name);
		void	SaveEmitter(string name);

	private:
		E_EffectToolTab		type_;
	private:
		UVSprite		uv_sprite_data;
		TextureSprite	tex_sprite_data;
		Emitter			emitter_data;
	public:
		void LoadingSpriteData(string path);
		void LoadingEmitterData(string path);
	};
}

