//------------------------------------------------------------------------------
//
//ゲーム処理  [game.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "Bomb.h"
#include "Map.h"
#include "ParticleManager.h"
#include "picture.h"
#include "scene2D.h"
#include "chatBase.h"
#include "light.h"
#include "client.h"
#include "tablet.h"
#include "TabletButton.h"
#include "client.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
bool CGame::m_bClear = false;
CBomb::DIFFICULTY CGame::m_bDifficulty = CBomb::HARD;

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CGame::CGame()
{
	m_nCntState = 0;
	m_State = STATE::STATE_NORMAL;
	m_Gaze = GAZE_BOMB;
	m_pBomb.reset();

	m_bClear = false;

}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CGame::~CGame()
{
	m_pBomb.reset();

	// タブレットボタンの開放
	CTabletButton::Unload();

	// ピクチャの静的メンバの終了
	CPicture::UninitStaticMember();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CGame::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	//初期化
	SetState(STATE::STATE_READY);

#ifdef _DEBUG

	//初期化
	SetState(STATE::STATE_NORMAL);

#endif

	//パーティクルのマネージャ
	CParticleManager::Create();

	//爆弾生成
	m_pBomb	= CBomb::CreateBomb(D3DXVECTOR3(0.0f,200.0f,0.0f), ZeroVector3, m_bDifficulty);

	//モジュール生成
	m_pBomb->CreateModule();

	//マップ生成
	CMap::Create();

	//// タブレットの読み込み
	//CTablet::Load();
	//// タブレットボタンの読み込み
	//CTabletButton::Load();
	//// タブレットの生成
	//m_pTablet = CTablet::Create();
	// ピクチャの静的メンバの初期化
	//CPicture::InitStaticMember();
	//m_pPicture = CPicture::Create(m_pTablet->GetMtxWorldPtr());

	// チャットの生成
	m_pChatBase = CChatBase::Create();

	return S_OK;
}


//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CGame::Update()
{
	UpdateState();
}

//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CGame::Draw()
{

}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CGame::ShowDebugInfo()
{
#ifdef _DEBUG


#endif //_DEBUG
}

//------------------------------------------------------------------------------
//ステートの更新
//------------------------------------------------------------------------------
void CGame::UpdateState()
{
	m_nCntState--;

	switch (m_State)
	{
	case CGame::STATE_READY:
		if (m_nCntState <= 0)
		{
			SetState(CGame::STATE_NORMAL);
		}
		break;
	case CGame::STATE_NORMAL:
		break;
	case CGame::STATE_PAUSE:
		break;
	case CGame::STATE_GAMEOVER:
		if (m_nCntState <= 0)
		{
			SetState(CGame::STATE_NONE);
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT, BlackColor);
		}
		break;
	case CGame::STATE_GAMECLEAR:
		m_bClear = true;

		if (m_nCntState <= 0)
		{
			SetState(CGame::STATE_NONE);
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT);
		}
		break;
	}
}

//------------------------------------------------------------------------------
//ゲームステートセット
//------------------------------------------------------------------------------
void CGame::SetState(STATE state)
{
	//ステートが進んでいる場合
	if (m_State != CGame::STATE_NONE && m_State != state)
	{
		//ステート変更
		m_State = state;

		switch (m_State)
		{
		case CGame::STATE_NONE:
			break;
		case CGame::STATE_READY:
			m_nCntState = 120;
			CManager::GetRenderer()->GetLight()->SetBlackLight();

			break;
		case CGame::STATE_NORMAL:
			CManager::GetRenderer()->GetLight()->SetDefaultLight();

			break;
		case CGame::STATE_PAUSE:
			break;
		case CGame::STATE_GAMEOVER:
			m_nCntState = 120;

			//画面全体に真っ黒ポリゴン
			CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(SCREEN_CENTER_POS, SCREEN_SIZE, BlackColor, nullptr, CScene::OBJTYPE_FRONT);

			//音再生
			CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION_00);

			// ゲームオーバー送信
			CClient::SendGameOver();

			break;
		case CGame::STATE_GAMECLEAR:
			m_nCntState = 120;
			// ゲームクリア送信
			CClient::SendGameClear();

			break;
		case CGame::STATE_DEBUG:
			break;
		case CGame::STATE_MAX:
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
//視線設定
//------------------------------------------------------------------------------
void CGame::SetGaze(GAZE gaze)
{
	//前の状態と違う時
	if (m_Gaze != gaze)
	{
		//視線情報設定
		m_Gaze = gaze;
		switch (m_Gaze)
		{
		case CGame::GAZE_DEFAULT:

			break;
		case CGame::GAZE_BOMB:
			CManager::GetRenderer()->GetCamera()->LeaveCamera();
			break;
		case CGame::GAZE_MODULE:

			break;
		default:
			break;
		}


	}

}
