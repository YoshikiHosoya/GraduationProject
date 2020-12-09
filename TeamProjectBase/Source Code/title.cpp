//------------------------------------------------------------------------------
//
//タイトル処理  [title.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "fade.h"
#include "title.h"
#include "Bomb.h"
#include "Map.h"
#include "light.h"
#include "ParticleManager.h"
#include "scene2D.h"
#include "module_Timer.h"
#include "timer.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CTitle::CTitle()
{

	m_nCntState = 0;
	m_titlestate = STATE_NORMAL;

}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CTitle::~CTitle()
{

}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CTitle::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_TITLE);
	//CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	//パーティクルのマネージャ
	CParticleManager::Create();


	//ポリゴン生成
	CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(640.0f, 170.0f, 0.0f), D3DXVECTOR3(850.0f, 200.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_TITLE), CScene::OBJTYPE_UI);
	CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(640.0f, 600.0f, 0.0f), D3DXVECTOR3(800.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_PRESSBUTTON), CScene::OBJTYPE_UI);

	//爆弾生成
	m_pBomb = CBomb::CreateBomb(D3DXVECTOR3(0.0f, 200.0f, 0.0f), ZeroVector3, CBomb::TITLE);

	//モジュール生成
	m_pBomb->CreateModule();

	//マップ生成
	CMap::Create();


	return S_OK;
}


//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CTitle::Update()
{

	StateUpdate();

}

//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CTitle::Draw()
{
}

//------------------------------------------------------------------------------
//ステート更新
//------------------------------------------------------------------------------
void CTitle::StateUpdate()
{
	//カウントダウン
	m_nCntState--;

	switch (m_titlestate)
	{
	case CTitle::STATE_NONE:
		break;
	case CTitle::STATE_START:
		break;
	case CTitle::STATE_NORMAL:
		//なんかボタン押されたとき
		if (CHossoLibrary::CheckAnyButton())
		{
			SetState(STATE_BLACKOUT);
		}
		break;

	case CTitle::STATE_BLACKOUT:
		if (m_nCntState < 0)
		{
			SetState(STATE_FADE);
		}
		break;
	case CTitle::STATE_FADE:
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------
//ステート切替
//------------------------------------------------------------------------------
void CTitle::SetState(STATE state)
{
	m_titlestate = state;

	switch (m_titlestate)
	{
	case CTitle::STATE_NONE:

		break;
	case CTitle::STATE_START:

		break;
	case CTitle::STATE_NORMAL:

		break;
	case CTitle::STATE_BLACKOUT:
		CManager::GetRenderer()->GetLight()->SetBlackLight();
		m_pBomb->GetModuleTimerPtr()->GetTimerPtr()->SetStop(false);
		m_nCntState = 90;
		break;
	case CTitle::STATE_FADE:
		//チュートリアルに遷移
		CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_DECODING);
		SetState(STATE_NONE);
		break;
	default:
		break;
	}
}
