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
		void	ComputeColorTimeline(map<int, XMFLOAT4>& timeline, XMFLOAT4* arr);
		void	ComputeSizeTimeline(map<int, XMFLOAT3>& timeline, XMFLOAT3* arr);
		void	ComputeRotationTimeline(map<int, float>& timeline, float* arr);
		void	ComputeVelocityTimeline(map<int, XMFLOAT3>& timeline, XMFLOAT3* arr);
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
		UVSprite							uv_sprite_data_;
		TextureSprite						tex_sprite_data_;
		Emitter								emitter_data_;
		map<string, shared_ptr<Emitter>>	effect_data_;
	
	};
}

