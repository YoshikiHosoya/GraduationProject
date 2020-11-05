//------------------------------------------------------------------------------
//
//モジュールのタイマー処理  [module_timer.cpp]
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
#define MISSCOUNTER_OFFSET_POS		(D3DXVECTOR3(-5.0f,22.0f,-32.0f))
#define MISSCOUNTER_SIZE			(D3DXVECTOR3(10.0f,10.0f,0.0f))
#define MISSCOUNTER_INTERVAL		(D3DXVECTOR3(13.0f,0.0f,0.0f))


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
		m_pMissCounter.emplace_back(CSceneBase::ScenePolygonCreateShared<CScene3D>(MISSCOUNTER_OFFSET_POS + D3DXVECTOR3(nCnt * MISSCOUNTER_INTERVAL.x,0.0f,0.0f), MISSCOUNTER_SIZE, RedColor, nullptr, OBJTYPE_MODULE_PARTS_SYMBOL));
		m_pMissCounter[m_pMissCounter.size() - 1]->SetParentMtxPtr(GetMtxWorldPtr());
		m_pMissCounter[m_pMissCounter.size() - 1]->SetLighting(false);
		m_pMissCounter[m_pMissCounter.size() - 1]->SetDisp(false);

	}


	CSceneX::Init();
	return S_OK;
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
//ミスのカウントアップ
//------------------------------------------------------------------------------
bool CModule_Timer::MissCountUp()
{
	m_nMissCount++;

	//上限まで行ったときはreturn
	if (CHossoLibrary::RangeLimit_Equal(m_nMissCount,0,2))
	{
		return true;
	}
	else
	{
		//ミスした分
		for (int nCnt = 0; nCnt < m_nMissCount; nCnt++)
		{
			//表示する
			m_pMissCounter[nCnt]->SetDisp(true);
		}
	}
	return false;
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Timer::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}