//------------------------------------------------------------------------------
//
//マップ処理  [map.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "Map.h"
#include "renderer.h"
#include "manager.h"
#include "meshfield.h"
#include "sound.h"
#include "sceneX.h"
#include "modelinfo.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define MESH_SIZE		(100.0f)
#define MESH_BLOCK_NUM	(INTEGER2(40,40))

#define BG_ORIGIN_POS	(D3DXVECTOR3(0.0f,-600.0f,0.0f))
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CMap::CMap()
{
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CMap::~CMap()
{
}
//------------------------------------------------------------------------------
//生成処理
//------------------------------------------------------------------------------
std::unique_ptr<CMap> CMap::Create()
{
	//メモリ確保
	std::unique_ptr<CMap> pMap(new CMap);

	//フィールド生成
	//CreateField(D3DXVECTOR3(MESH_SIZE, 0.0f, MESH_SIZE), INTEGER2(MESH_BLOCK_NUM.nX, MESH_BLOCK_NUM.nY));

	S_ptr<CSceneX> LocalPtr;
	Vec<int> LocalVec = { 0,CTexture::TEX_MATERIAL_FIELD004 };

	//マップモデル生成
	LocalPtr = CSceneX::CreateShared(BG_ORIGIN_POS, ZeroVector3, nullptr, CScene::OBJTYPE::OBJTYPE_BACK, CModelInfo::MODEL_BG);
	LocalPtr->SetTextureNum(LocalVec);

	CSceneX::CreateShared(BG_ORIGIN_POS, ZeroVector3, nullptr, CScene::OBJTYPE::OBJTYPE_BACK, CModelInfo::MODEL_BG_TABLE);

	return pMap;
}
//------------------------------------------------------------------------------
//フィールドの広さ取得
//------------------------------------------------------------------------------
D3DXVECTOR3 CMap::GetFieldSize()
{
	return D3DXVECTOR3(MESH_SIZE * MESH_BLOCK_NUM.nX, 0.0f, MESH_SIZE * MESH_BLOCK_NUM.nY);
}
//------------------------------------------------------------------------------
//フィールド作成
//------------------------------------------------------------------------------
void CMap::CreateField(D3DXVECTOR3 size, INTEGER2 BlockNum)
{
	//フィールド
	CMeshField::Create(ZeroVector3, size, BlockNum);

}
