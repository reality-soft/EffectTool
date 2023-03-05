#pragma once
#include "Engine_include.h"
#include "DataTypes.h"

namespace reality
{
	class WG_MainMenu : public reality::GuiWidget
	{
	public:
		virtual void Update() override;
		virtual void Render() override;
	};

	class WG_EffectWindow : public reality::GuiWidget
	{
	public:
		virtual void Update() override;
		virtual void Render() override;
	private:
		void	UVSpriteBoard();
		void	TexSpriteBoard();
		void	EmitterBoard(Emitter& emitter);
		void	EffectBoard();
	private:
		void	SelectBSOptions(E_EffectBS& bs_state);
		void	SelectDSOptions(E_EffectDS& ds_state);
		void	SelectFrame(int max_frame, int& cur_frame);
		void	SelectUV(vector<pair<POINT, POINT>>& list);
		void	SelectVertexShader(string& id);
		void	SelectGeometryShader(string& id);
		void	SelectMaterial(string& id);
		void	SelectTexture(string& id);
		void	SelectSprite(string& id);
	private:
		void	LoadingData();
	private:
		void	LoadingSpriteData(string path);
		void	LoadingEmitterData(string path, Emitter& emitter);
		void	LoadingEffectData(string path);
	private:
		void	SaveUVSprite(string name);
		void	SaveTexSprite(string name);
		void	SaveEmitter(string name);
		void	SaveEffect(string name);

	private:
		E_EffectType	type_;
	private:
		UVSprite				uv_sprite_data_;
		TextureSprite			tex_sprite_data_;
		Emitter					emitter_data_;
		map<string, Emitter>	effect_data_;
	
	};
}

