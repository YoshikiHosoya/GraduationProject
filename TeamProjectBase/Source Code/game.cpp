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
#include "chatBase.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CGame::CGame()
{
	m_nCntState = 0;
	m_State = STATE_NORMAL;
	m_Gaze = GAZE_BOMB;
	m_pBomb.reset();
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CGame::~CGame()
{
	m_pBomb.reset();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CGame::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	//初期化
	SetState(CGame::STATE_NORMAL);

	//パーティクルのマネージャ
	CParticleManager::Create();

	//爆弾生成
	m_pBomb	= CBomb::CreateBomb(D3DXVECTOR3(0.0f,120.0f,0.0f), ZeroVector3, 12);

	//マップ生成
	CMap::Create();

	// ピクチャの生成
	CPicture::Load();
	CPicture::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), ZeroVector3);

	// チャットの生成
	m_pChatBase = CChatBase::Create();

	return S_OK;
}

//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CGame::Uninit()
{
	CPicture::InitStaticMember();

	// チャットの破棄
	m_pChatBase->Uninit();
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
		break;
	case CGame::STATE_NORMAL:
		break;
	case CGame::STATE_PAUSE:
		break;
	case CGame::STATE_GAMEOVER:
		break;
	case CGame::STATE_GAMECLEAR:
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
			break;
		case CGame::STATE_NORMAL:
			break;
		case CGame::STATE_PAUSE:
			break;
		case CGame::STATE_GAMEOVER:
			break;
		case CGame::STATE_GAMECLEAR:
			m_nCntState = 120;

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
			//CManager::GetRenderer()->GetCamera()->
			break;
		case CGame::GAZE_MODULE:

			break;
		default:
			break;
		}


	}

}
