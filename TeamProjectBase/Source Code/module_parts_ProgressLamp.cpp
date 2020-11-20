//------------------------------------------------------------------------------
//
//モジュールの進捗度のランプ  [module_parts_Progresslamp.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_parts_ProgressLamp.h"
#include "modelinfo.h"
#include "scene3D.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define PROGRESS_LAMP_OFFSET			(D3DXVECTOR3(0.0f,0.0f,-2.0f))		//進捗度ランプの間隔
#define PROGRESS_LAMP_INTERVAL			(D3DXVECTOR3(10.0f,0.0f,0.0f))		//進捗度ランプの間隔
#define PROGRESS_LAMP_SIZE				(D3DXVECTOR3(7.0f,10.0f,0.0f))		//進捗度ランプの大きさ
#define PROGRESS_LAMP_NUM				(4)

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_Parts_ProgressLamp::CModule_Parts_ProgressLamp()
{
	m_nProgress = 0;
	m_pProgressLamp.clear();
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_Parts_ProgressLamp::~CModule_Parts_ProgressLamp()
{
	m_pProgressLamp.clear();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_Parts_ProgressLamp::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULEPARTS_PROGRESSLAMP));

	//ランプの生成
	CreateProgressLamp();

	CSceneX::Init();
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_Parts_ProgressLamp::Update()
{
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_Parts_ProgressLamp::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Parts_ProgressLamp::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//進捗のランプ生成
//------------------------------------------------------------------------------
void CModule_Parts_ProgressLamp::CreateProgressLamp()
{
	//ランプ生成
	for (size_t nCnt = 0; nCnt < PROGRESS_LAMP_NUM; nCnt++)
	{
		//ランプ生成
		m_pProgressLamp.emplace_back(CSceneBase::ScenePolygonCreateShared<CScene3D>
			(PROGRESS_LAMP_OFFSET - D3DXVECTOR3(CHossoLibrary::CalcEvenPosition(PROGRESS_LAMP_NUM, nCnt, PROGRESS_LAMP_INTERVAL.x), 0.0f, 0.0f),
				PROGRESS_LAMP_SIZE,
				BlackColor,
				nullptr,
				CScene::OBJTYPE_MODULE_PARTS_SYMBOL));

		//親のマトリックス設定
		m_pProgressLamp[m_pProgressLamp.size() - 1]->SetParentMtxPtr(GetMtxWorldPtr());
		m_pProgressLamp[m_pProgressLamp.size() - 1]->SetLighting(false);
	}
}

//------------------------------------------------------------------------------
//進捗度設定
//------------------------------------------------------------------------------
void CModule_Parts_ProgressLamp::SetProgress(int nProgress)
{
	m_nProgress = nProgress;

	for (int nCnt = 0; nCnt < m_nProgress; nCnt++)
	{
		m_pProgressLamp[nCnt]->SetColor(GreenColor);
	}

}
