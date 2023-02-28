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
		void	LoadingData();
		void	UVSpriteBoard();
		void	TexSpriteBoard();
		void	SpriteEmitterBoard(SpriteEmitter& emitter);
		void	EffectBoard();
	private:
		void	SelectBSOptions(E_EffectBS& bs_state);
		void	SelectDSOptions(E_EffectDS& ds_state);
		void	SelectFrame(int& max_frame, int& cur_frame);
		void	SelectUV(vector<pair<POINT, POINT>>& list, int& max_frame);
		void	SelectVertexShader(string& id);
		void	SelectGeometryShader(string& id);
		void	SelectMaterial(string& id);
		void	SelectTexture(string& id);
		void	SelectSprite(string& id);

	private:
		void	RenderWireFrame();
	private:
		void	SaveUVSprite(string name);
		void	SaveTexSprite(string name);
		void	SaveSpriteEmitter(string name);

	private:
		E_EffectType	type_;
	private:
		UVSprite		uv_sprite_data;
		TextureSprite	tex_sprite_data;
		SpriteEmitter	sprite_emitter_data;
		map<string, shared_ptr<Emitter>> emitter_map;
	public:
		void LoadingSpriteData(string path);
		void LoadingEmitterData(string path, SpriteEmitter& emitter);
		void LoadingEffectData(string path);
	};
}

