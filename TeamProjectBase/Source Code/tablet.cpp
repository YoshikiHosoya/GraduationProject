//*************************************************************************************************************
//
// タブレット処理 [tablet.cpp]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "tablet.h"
#include "modelinfo.h"
#include "TabletButton.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ関数定義
//-------------------------------------------------------------------------------------------------------------
#define TabletButtonSetPos(cnt) D3DXVECTOR3(100.0f, 0.0f - 30.0f * cnt, -8.0f)

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CTablet::CTablet()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CTablet::~CTablet()
{
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CTablet::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_TABLET));
	CSceneX::Init();

	// 容量を変更
	m_Button.reserve(CTabletButton::TYPE_MAX);
	// タイプ数分ループ
	for (int nCntTtpe = 0; nCntTtpe < CTabletButton::TYPE_MAX; nCntTtpe++)
	{// 新しい要素を末尾に追加
		m_Button.push_back();
		// 末尾に生成
		m_Button.back() =
			CTabletButton::Create(
				this->GetMtxWorldPtr(),
				TabletButtonSetPos(nCntTtpe),
				(CTabletButton::TYPE)nCntTtpe);
	}

	return E_NOTIMPL;
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CTablet::Update()
{
	// 変数宣言
	D3DXVECTOR3*   pNearPos   = &CManager::GetRay()->NearPos;	// レイの近い位置
	D3DXVECTOR3*   pFarPos    = &CManager::GetRay()->FarPos;	// レイの遠い位置
	BOOL           bHit       = 0;								// ヒットフラグ
	D3DXMATRIX     invmat;										// 算出した逆行列
	D3DXVECTOR3    ray;											// レイ
	D3DXVECTOR3    InvNirePos;									// 算出した近い位置
	D3DXVECTOR3    InvForePos;									// 算出した遠い位置
	CTabletButton* pButton;										// ボタン情報
	LPD3DXMESH     pMesh;										// メッシュ情報
	int nCntTtpe;
	for (nCntTtpe = 0; nCntTtpe < CTabletButton::TYPE_MAX; nCntTtpe++)
	{
		// ボタン情報の取得
		pButton = m_Button[nCntTtpe].get();
		// メッシュ情報の取得
		pMesh = pButton->GetModelInfo()->GetMesh();

		/* 対処いう物からみたレイに変換する */
		//	逆行列の取得
		D3DXMatrixInverse(&invmat, NULL, pButton->GetMtxWorldPtr());
		//	逆行列を使用し、レイ始点情報を変換　位置と向きで変換する関数が異なるので要注意
		D3DXVec3TransformCoord(&InvNirePos, pNearPos, &invmat);
		//	レイ終点情報を変換
		D3DXVec3TransformCoord(&InvForePos, pFarPos, &invmat);
		//	レイ方向情報を変換
		D3DXVec3Normalize(&ray, &(InvForePos - InvNirePos));
		//Rayを飛ばす
		D3DXIntersect(pMesh, &InvNirePos, &ray, &bHit, NULL, NULL, NULL, NULL, NULL, NULL);
		// HITしている時
		if (bHit == 1)
		{
			// モードの設定
			pButton->SetMode();
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "[%d]当たったものが存在しました。\n", nCntTtpe);
		}
		else
		{
			pButton->SetToOffPressFlag();
			pButton->SetChangeFlag();
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CTablet::Draw()
{
	CSceneX::Draw();
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
std::shared_ptr<CTablet> CTablet::Create(CONST D3DXVECTOR3 & pos)
{
	// スマートポインタの生成
	std::shared_ptr<CTablet> pTablet = std::make_shared<CTablet>();
	pTablet->SetPos(pos);
	pTablet->Init();

	//Sceneで管理
	pTablet->SetObjType(OBJTYPE_PICTURE);
	pTablet->AddSharedList(pTablet);
	return pTablet;
}
