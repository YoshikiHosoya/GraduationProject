//------------------------------------------------------------------------------
//
//モジュールの処理  [module.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_parts_lamp.h"
#include "renderer.h"
#include "manager.h"
#include "modelinfo.h"
#include "timer.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_Parts_Lamp::CModule_Parts_Lamp()
{
	m_LampState = LAMP_STATE::OFF;
	m_nCntLampCnt = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_Parts_Lamp::~CModule_Parts_Lamp()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_Parts_Lamp::Init()
{
	CSceneX::Init();
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CModule_Parts_Lamp::Uninit()
{
	CSceneX::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_Parts_Lamp::Update()
{
	switch (m_LampState)
	{
	case CModule_Parts_Lamp::LAMP_STATE::OFF:
		break;
	case CModule_Parts_Lamp::LAMP_STATE::RED:
		m_nCntLampCnt--;
		if (m_nCntLampCnt < 0)
		{
			SetLampState(LAMP_STATE::GREEN);
		}
		break;
	case CModule_Parts_Lamp::LAMP_STATE::GREEN:
		break;
	}
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_Parts_Lamp::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Parts_Lamp::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//生成関数
//Bombが管理
//------------------------------------------------------------------------------
std::shared_ptr<CModule_Parts_Lamp> CModule_Parts_Lamp::Create(D3DXVECTOR3 const pos, D3DXMATRIX * const pBomb)
{
	//メモリ確保
	std::shared_ptr<CModule_Parts_Lamp> pModuleTimer = std::make_shared<CModule_Parts_Lamp>();

	//初期化
	pModuleTimer->Init();

	//座標とサイズ設定
	pModuleTimer->SetPos(pos);
	pModuleTimer->SetParentMtxPtr(pBomb);

	//モデル情報設定
	pModuleTimer->BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_BOMBOBJECT_CLEARLAMP));

	//Scene側で管理
	pModuleTimer->SetObjType(CScene::OBJTYPE_MODULE);
	pModuleTimer->AddSharedList(pModuleTimer);

	return pModuleTimer;
}
