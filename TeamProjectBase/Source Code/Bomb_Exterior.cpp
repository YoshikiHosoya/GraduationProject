//------------------------------------------------------------------------------
//
//モジュールのボタン処理  [module_Button.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "Bomb_Exterior.h"
#include "modelinfo.h"
#include "Mylibrary.h"
#include "scene3D.h"
#include "sceneX.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define EXTERIOR_OFFSET_TOP				(D3DXVECTOR3(145.0f,123.0f,0.0f))			//外装のオフセット 上下のほう
#define EXTERIOR_OFFSET_SIDE			(D3DXVECTOR3(420.0f,70.0f,0.0f))			//外装のオフセット 左右のほう

#define MAX_EXTERIOR_NUM				(10)										//外装の総数

#define MAX_BUTTERY_NUM					(MAX_EXTERIOR_NUM)							//バッテリーの最大数

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CBomb_Exterior::CBomb_Exterior()
{
	m_pBatteryList.clear();
	m_ExteriorList.clear();
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CBomb_Exterior::~CBomb_Exterior()
{
	m_pBatteryList.clear();
	m_ExteriorList.clear();
}

//------------------------------------------------------------------------------
//爆弾の外装生成
//------------------------------------------------------------------------------
U_ptr<CBomb_Exterior> CBomb_Exterior::CreateBombExterior(D3DXMATRIX * pBombMtx)
{
	//メモリ確保
	U_ptr<CBomb_Exterior> pPtr = std::make_unique<CBomb_Exterior>();

	//nullcheck
	if (pPtr)
	{
		//生成
		pPtr->Create(pBombMtx);

		//return
		return std::move(pPtr);
	}

	//return
	return nullptr;
}

//------------------------------------------------------------------------------
//外装の数を数える
//------------------------------------------------------------------------------
int CBomb_Exterior::CountExteriorNum(EXTERIOR_TYPE type)
{
	return std::count(m_ExteriorList.begin(), m_ExteriorList.end(), type);
}

//------------------------------------------------------------------------------
//外装に何をセットするか決める
//------------------------------------------------------------------------------
void CBomb_Exterior::SetExterior()
{
	//バッテリー
	for (int nCnt = 0; nCnt < (rand() % MAX_BUTTERY_NUM); nCnt++)
	{
		//バッテリー追加
		//大きいバッテリーか小さいバッテリーかランダム
		rand() % 2 == 0 ?
			m_ExteriorList.emplace_back(EXTERIOR_TYPE::BATTERY_SMALL) :
			m_ExteriorList.emplace_back(EXTERIOR_TYPE::BATTERY_BIG);
	}

	//最大数になるまで
	while ((int)m_ExteriorList.size() < MAX_EXTERIOR_NUM)
	{
		//何もない状態
		m_ExteriorList.emplace_back(EXTERIOR_TYPE::NONE);
	}
}

//------------------------------------------------------------------------------
//生成関数
//------------------------------------------------------------------------------
void CBomb_Exterior::Create(D3DXMATRIX * pBombMtx)
{
	//何をセットするか決める
	SetExterior();

	//リストシャッフル
	CHossoLibrary::Vec_Shuffle(m_ExteriorList);

	//計算用のローカル変数
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	//最大数になるまで
	for (int nCnt = 0; nCnt < (int)m_ExteriorList.size(); nCnt++)
	{
		//外装の座標計算
		CalcExteriorPosition(nCnt, pos, rot);

		//種類に応じてオブジェクトを変える
		switch (m_ExteriorList[nCnt])
		{
		case EXTERIOR_TYPE::BATTERY_SMALL:

			//小バッテリー生成
			m_pBatteryList.emplace_back(CSceneX::CreateShared(pos, rot,
				pBombMtx, CScene::OBJTYPE_BOMB_EXTERIOR, CModelInfo::MODEL_BATTERY_SMALL));
			break;

		case EXTERIOR_TYPE::BATTERY_BIG:

			//大バッテリー生成
			m_pBatteryList.emplace_back(CSceneX::CreateShared(pos, rot,
				pBombMtx, CScene::OBJTYPE_BOMB_EXTERIOR, CModelInfo::MODEL_BATTERY_BIG));
			break;
		}
	}
}

//------------------------------------------------------------------------------
//外装の座標や回転量を計算
//------------------------------------------------------------------------------
void CBomb_Exterior::CalcExteriorPosition(int const nCnt,D3DXVECTOR3 & pos, D3DXVECTOR3 & rot)
{
	//初期化
	pos = ZeroVector3;
	rot = ZeroVector3;

	//0-2
	if (nCnt < 3)
	{
		pos.x = CHossoLibrary::CalcEvenPosition(3, nCnt, EXTERIOR_OFFSET_TOP.x);
		pos.y = EXTERIOR_OFFSET_TOP.y;
	}
	//3-7
	else if (nCnt >= 3 && nCnt < 7)
	{
		pos.x = CHossoLibrary::CalcEvenPosition(2, (nCnt - 3) % 2, EXTERIOR_OFFSET_SIDE.x);
		pos.y = CHossoLibrary::CalcEvenPosition(2, (nCnt - 3) / 2, EXTERIOR_OFFSET_SIDE.y);
		rot.z = (nCnt - 3) % 2 ? -D3DX_PI * 0.5f : D3DX_PI * 0.5f;
	}
	//8-10
	else if (nCnt >= 7)
	{
		pos.x = CHossoLibrary::CalcEvenPosition(3, (nCnt - 7), EXTERIOR_OFFSET_TOP.x);
		pos.y = -EXTERIOR_OFFSET_TOP.y;
		rot.z = D3DX_PI;
	}
}
