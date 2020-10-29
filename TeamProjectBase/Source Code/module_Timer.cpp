//------------------------------------------------------------------------------
//
//モジュールの処理  [module.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_timer.h"
#include "renderer.h"
#include "manager.h"
#include "modelinfo.h"
#include "scene3D.h"
#include "timer.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define TIMER_OFFSET_POS			(D3DXVECTOR3(0.0f,-8.0f,-2.0f))
#define MISSCOUNTER_OFFSET_POS		(D3DXVECTOR3(-5.0f,22.0f,-33.0f))
#define MISSCOUNTER_SIZE			(D3DXVECTOR3(10.0f,10.0f,0.0f))


//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_Timer::CModule_Timer()
{
	m_pTimer.reset();
	m_pMissCounter.clear();
	m_nMissCount = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_Timer::~CModule_Timer()
{
	m_pTimer.reset();
	m_pMissCounter.clear();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_Timer::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_TIMER));

	//モジュール選択不可
	CModule_Base::SetCanModuleSelect(false);

	//タイマーの生成
	m_pTimer = CTimer::Create(TIMER_OFFSET_POS, 360, GetMtxWorldPtr());

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_pMissCounter.emplace_back(CSceneBase::ScenePolygonCreateShared<CScene3D>(MISSCOUNTER_OFFSET_POS + D3DXVECTOR3(nCnt * 12.0f,0.0f,0.0f), MISSCOUNTER_SIZE, RedColor, nullptr, OBJTYPE_MODULE_PARTS_SYMBOL));
		m_pMissCounter[m_pMissCounter.size() - 1]->SetParentMtxPtr(GetMtxWorldPtr());
		m_pMissCounter[m_pMissCounter.size() - 1]->SetLighting(false);

	}


	CSceneX::Init();
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CModule_Timer::Uninit()
{
	CSceneX::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_Timer::Update()
{
	m_pTimer->UpdateTimer();

	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_Timer::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Timer::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}