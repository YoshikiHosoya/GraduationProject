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
#define TabletButtonSetPos(cnt) D3DXVECTOR3(100.0f, 0.0f - 30.0f *cnt, -8.0f)

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
	D3DXVECTOR3* pRayDir = &CManager::GetRay()->vec;
	BOOL bHit = 0;
	int nHitTypr = -1;

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "レイの向き[%f][%f][%f]\n", pRayDir->x, pRayDir->y, pRayDir->z);

	D3DXVECTOR3 pos;
	D3DXVECTOR3 CameraPos = CManager::GetRenderer()->GetCamera()->GetCameraPosV();

	for (int nCntTtpe = 0; nCntTtpe < CTabletButton::TYPE_MAX; nCntTtpe++)
	{
		CTabletButton* pButton = m_Button[nCntTtpe].get();
		CModelInfo * pModelInfo = pButton->GetModelInfo();
		LPD3DXMESH pMesh = pModelInfo->GetMesh();

		pos.x = CameraPos.x - pButton->GetMtxWorldPtr()->_41;
		pos.y = CameraPos.y - pButton->GetMtxWorldPtr()->_42;
		pos.z = CameraPos.z -pButton->GetMtxWorldPtr()->_43;

		D3DXIntersect(pMesh, &pos, pRayDir, &bHit, NULL, NULL, NULL, NULL, NULL, NULL);

		if (bHit == 1)
		{
			nHitTypr = nCntTtpe;
		}
	}
	if (nHitTypr != -1)
	{
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "[%d]当たった\n", nHitTypr);
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
