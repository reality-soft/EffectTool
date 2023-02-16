#include "FX_TexSprite.h"
#include "TimeMgr.h"

using namespace KGCA41B;

void FX_TexSprite::OnInit(entt::registry& registry, KGCA41B::AABB<3> collision_box, TextureSprite tex_sprite)
{
	FX_BaseEffectActor::OnInit(registry, collision_box);

	entt::type_hash<TextureSprite> type_hash_tex_sprite;
	tex_sprite.local = XMMatrixIdentity();
	tex_sprite.cur_frame = 1;
	auto& sprite_comp = registry.emplace<TextureSprite>(entity_id_, tex_sprite);

	transform_tree_.root_node->children.push_back(make_shared<TransformTreeNode>(type_hash_tex_sprite.value()));

	transform_tree_.root_node->OnUpdate(registry, entity_id_);

	HRESULT hr;

	CreateVertexData(sprite_comp);

	hr = CreateVertexBuffer(sprite_comp);
	if (FAILED(hr))
		return;

	CreateIndexData(sprite_comp);

	hr = CreateIndexBuffer(sprite_comp);
	if (FAILED(hr))
		return;

	timer_ = 1.0f;
}

void KGCA41B::FX_TexSprite::OnUpdate(entt::registry& registry)
{
	FX_BaseEffectActor::OnUpdate(registry);

	auto& view_tex_sprite = registry.get<TextureSprite>(entity_id_);

	if (view_tex_sprite.enabled_)
	{
		timer_ += TIMER->GetDeltaTime();
		view_tex_sprite.cur_frame = timer_;
		if (view_tex_sprite.cur_frame > view_tex_sprite.max_frame)
		{
			timer_ = 1.0f;
			view_tex_sprite.enabled_ = false;
			view_tex_sprite.cur_frame = 1;
		}
	}
}

void KGCA41B::FX_TexSprite::SetTexSprite(entt::registry& registry, TexSpriteData tex_sprite)
{
	auto& view_tex_sprite = registry.get<TextureSprite>(entity_id_);
	view_tex_sprite.cur_frame = tex_sprite.cur_frame;
	view_tex_sprite.max_frame = tex_sprite.max_frame;
	view_tex_sprite.tex_id_list = tex_sprite.tex_id_list;
	view_tex_sprite.vs_id = tex_sprite.vs_id;
	view_tex_sprite.ps_id = tex_sprite.ps_id;
	view_tex_sprite.enabled_ = true;
}

void KGCA41B::FX_TexSprite::CreateVertexData(TextureSprite& sprite_comp)
{
	sprite_comp.vertex_list.push_back({ { -1.0f, +1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+0.0f, +0.0f} });
	sprite_comp.vertex_list.push_back({ { +1.0f, +1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+1.0f, +0.0f} });
	sprite_comp.vertex_list.push_back({ { -1.0f, -1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+0.0f, +1.0f} });
	sprite_comp.vertex_list.push_back({ { +1.0f, -1.0f, +0.0f }, {+0.0f, +0.0f, +0.0f}, {+1.0f, +1.0f, +1.0f, +1.0f}, {+1.0f, +1.0f} });
}

HRESULT KGCA41B::FX_TexSprite::CreateVertexBuffer(TextureSprite& sprite_comp)
{
	if (sprite_comp.vertex_list.size() == 0)
		return S_OK;

	D3D11_BUFFER_DESC bufDesc;

	ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

	bufDesc.ByteWidth = sizeof(Vertex) * sprite_comp.vertex_list.size();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourse;

	ZeroMemory(&subResourse, sizeof(D3D11_SUBRESOURCE_DATA));

	subResourse.pSysMem = &sprite_comp.vertex_list.at(0);
	subResourse.SysMemPitch;
	subResourse.SysMemSlicePitch;

	return DX11APP->GetDevice()->CreateBuffer(&bufDesc, &subResourse, &sprite_comp.vertex_buffer);
}

void KGCA41B::FX_TexSprite::CreateIndexData(TextureSprite& sprite_comp)
{
	sprite_comp.index_list.push_back(0);
	sprite_comp.index_list.push_back(1);
	sprite_comp.index_list.push_back(2);
	sprite_comp.index_list.push_back(2);
	sprite_comp.index_list.push_back(1);
	sprite_comp.index_list.push_back(3);
}

HRESULT KGCA41B::FX_TexSprite::CreateIndexBuffer(TextureSprite& sprite_comp)
{
	D3D11_BUFFER_DESC bufDesc;

	ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

	bufDesc.ByteWidth = sizeof(DWORD) * sprite_comp.index_list.size();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourse;

	ZeroMemory(&subResourse, sizeof(D3D11_SUBRESOURCE_DATA));

	subResourse.pSysMem = &sprite_comp.index_list.at(0);
	subResourse.SysMemPitch;
	subResourse.SysMemSlicePitch;

	return DX11APP->GetDevice()->CreateBuffer(&bufDesc, &subResourse, &sprite_comp.index_buffer);
}
