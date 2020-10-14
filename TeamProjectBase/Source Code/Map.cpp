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
#include "meshsphere.h"
#include "sound.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define MESH_SIZE		(100)
#define MESH_BLOCK_NUM	(INTEGER2(40,40))
#define WALL_BLOCK_NUM	(INTEGER2(8,8))
#define WALL_SIZE		(400.0f)
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
	CreateField(D3DXVECTOR3(MESH_SIZE, 0.0f, MESH_SIZE), INTEGER2(MESH_BLOCK_NUM.x, MESH_BLOCK_NUM.y));

	////壁生成　フィールドの半分のサイズ
	CreateWall(INTEGER2(WALL_BLOCK_NUM.x, WALL_BLOCK_NUM.x));

	return pMap;
}
//------------------------------------------------------------------------------
//フィールドの広さ取得
//------------------------------------------------------------------------------
D3DXVECTOR3 CMap::GetFieldSize()
{
	return D3DXVECTOR3(WALL_SIZE * WALL_BLOCK_NUM.x - 130.0f, 0.0f, WALL_SIZE * WALL_BLOCK_NUM.y - 130.0f);
}
//------------------------------------------------------------------------------
//フィールド作成
//------------------------------------------------------------------------------
void CMap::CreateField(D3DXVECTOR3 size, INTEGER2 BlockNum)
{
	//フィールド
	CMeshField::Create(ZeroVector3, size, BlockNum);

}
//------------------------------------------------------------------------------
//□になるように壁を配置　□の大きさはBlockNumによって決まる
//------------------------------------------------------------------------------
void CMap::CreateWall(INTEGER2 BlockNum)
{
	////前後の壁
	//for (int nCntV = 0; nCntV < BlockNum.x; nCntV++)
	//{
	//	CModel_Object::Create(D3DXVECTOR3(BlockNum.x * WALL_SIZE * 0.5f - nCntV * WALL_SIZE - (WALL_SIZE * 0.5f), 0.0f, WALL_SIZE * BlockNum.y * 0.5f), ZeroVector3);									//前
	//	CModel_Object::Create(D3DXVECTOR3(BlockNum.x * WALL_SIZE * 0.5f - nCntV * WALL_SIZE - (WALL_SIZE * 0.5f), 0.0f, -WALL_SIZE * BlockNum.y * 0.5f), ZeroVector3);									//後
	//}
	////左右の壁
	//for (int nCntH = 0; nCntH < BlockNum.x; nCntH++)
	//{
	//	CModel_Object::Create(D3DXVECTOR3(-WALL_SIZE * BlockNum.x * 0.5f, 0.0f, BlockNum.y * WALL_SIZE * 0.5f - nCntH * WALL_SIZE - (WALL_SIZE * 0.5f)), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));		//左
	//	CModel_Object::Create(D3DXVECTOR3(WALL_SIZE * BlockNum.x * 0.5f, 0.0f, BlockNum.y * WALL_SIZE * 0.5f - nCntH * WALL_SIZE - (WALL_SIZE * 0.5f)), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));		//右
	//}
}
