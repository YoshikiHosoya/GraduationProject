//------------------------------------------------------------------------------
//
//解読処理  [Decoding.h]
//Author:Ikuto Sekine
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "Decoding.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "Map.h"
#include "ParticleManager.h"
#include "picture.h"
#include "scene2D.h"
#include "chatBase.h"
#include "light.h"
#include "client.h"
#include "tablet.h"
#include "TabletButton.h"
#include "DecodingManager.h"
#include "DecodingWindow.h"
#include "keyboard.h"
#include "game.h"

//------------------------------------------------------------------------------
// マクロ定義
//------------------------------------------------------------------------------
#define GAMEEND_UI_SIZE	(D3DXVECTOR3(800.0f, 250.0f, 0.0f))

//------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//------------------------------------------------------------------------------
CDecoding::GAMEENDSTATE CDecoding::m_endState = CDecoding::GAMEEND_NONE;
int CDecoding::m_nCntState = 0;

//------------------------------------------------------------------------------
// コンストラクタ
//------------------------------------------------------------------------------
CDecoding::CDecoding()
{
}

//------------------------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------------------------
CDecoding::~CDecoding()
{
	// タブレットボタンの開放
	CTabletButton::Unload();

	// ピクチャの静的メンバの終了
	CPicture::UninitStaticMember();

	// 解読ウィンドウの読み込み
	CDecodingWindow::UnLoad();
	CDecodingManager::Unload();
}

//------------------------------------------------------------------------------
// 初期化
//------------------------------------------------------------------------------
HRESULT CDecoding::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);
	CManager::GetRenderer()->GetCamera()->SetDecodingCamera();
	//初期化
	SetState(STATE::STATE_NORMAL);
	//// タブレットの読み込み
	//CTablet::Load();
	//// タブレットボタンの読み込み
	//CTabletButton::Load();
	//// タブレットの生成
	//m_pTablet = CTablet::Create();
	//// ピクチャの静的メンバの初期化
	//CPicture::InitStaticMember();
	//m_pPicture = CPicture::Create(m_pTablet->GetMtxWorldPtr());
	// 解読マネージャーの読み込み
	CDecodingManager::Load();
	// 解読ウィンドウの読み込み
	CDecodingWindow::Load();
	// 解読マネージャーの生成
	m_pDecodingManager = CDecodingManager::Create();
	// 終了状態の初期化
	m_endState = CDecoding::GAMEEND_NONE;

	//マップ生成
	CMap::Create();

	//チャット機能
	CChatBase::Create();
	return S_OK;
}

//------------------------------------------------------------------------------
// 更新
//------------------------------------------------------------------------------
void CDecoding::Update()
{
	// ゲームを終了するとき
	if (m_endState != GAMEEND_NONE)
	{
		UpdateEndState();
	}

#ifdef _DEBUG
	//なんかボタン押されたとき
	if (CManager::GetKeyboard()->GetPress(DIK_RSHIFT) &&
		CManager::GetKeyboard()->GetPress(DIK_RETURN))
	{
		//チュートリアルに遷移
		CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_GAME);
	}
#endif
}

//------------------------------------------------------------------------------
// 描画
//------------------------------------------------------------------------------
void CDecoding::Draw()
{
}

//------------------------------------------------------------------------------
// デバッグ情報の表示
//------------------------------------------------------------------------------
void CDecoding::ShowDebugInfo()
{
}

//------------------------------------------------------------------------------
// 状態の更新
//------------------------------------------------------------------------------
void CDecoding::UpdateState()
{
	m_nCntState--;

	switch (m_State)
	{
	case CDecoding::STATE_READY:
		if (m_nCntState <= 0)
		{
			SetState(CDecoding::STATE_NORMAL);
		}
		break;
	case CDecoding::STATE_NORMAL:
		break;
	case CDecoding::STATE_PAUSE:
		break;
	case CDecoding::STATE_GAMEOVER:
		if (m_nCntState <= 0)
		{
			SetState(CDecoding::STATE_NONE);
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT, BlackColor);
		}
		break;
	case CDecoding::STATE_GAMECLEAR:
		if (m_nCntState <= 0)
		{
			SetState(CDecoding::STATE_NONE);
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT);
		}
		break;
	}
}

//------------------------------------------------------------------------------
//　状態の設定
//------------------------------------------------------------------------------
void CDecoding::SetState(STATE state)
{
	//ステートが進んでいる場合
	if (m_State != CDecoding::STATE_NONE && m_State != state)
	{
		//ステート変更
		m_State = state;

		switch (m_State)
		{
		case CDecoding::STATE_NONE:
			break;
		case CDecoding::STATE_READY:
			m_nCntState = 120;
			CManager::GetRenderer()->GetLight()->SetBlackLight();

			break;
		case CDecoding::STATE_NORMAL:
			CManager::GetRenderer()->GetLight()->SetDefaultLight();

			break;
		case CDecoding::STATE_PAUSE:
			break;
		case CDecoding::STATE_GAMEOVER:

			break;
		case CDecoding::STATE_GAMECLEAR:
			m_nCntState = 120;

			break;
		case CDecoding::STATE_DEBUG:
			break;
		case CDecoding::STATE_MAX:
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// 視線の設定
//------------------------------------------------------------------------------
void CDecoding::SetGaze(GAZE gaze)
{
	//前の状態と違う時
	if (m_Gaze != gaze)
	{
		//視線情報設定
		m_Gaze = gaze;
		switch (m_Gaze)
		{
		case CDecoding::GAZE_DEFAULT:
			break;
		default:
			break;
		}


	}
}

//------------------------------------------------------------------------------
// 終了時の更新
//------------------------------------------------------------------------------
void CDecoding::UpdateEndState(void)
{
	// 最初
	if (m_nCntState >= 120)
	{
		// UIを生成し、クリアフラグを設定
		if (m_endState == GAMEEND_CLEAR)
		{
			CGame::SetClearFlag(true);
			CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(SCREEN_CENTER_POS, GAMEEND_UI_SIZE, WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_GAMECLEAR), CScene::OBJTYPE_FRONT);
		}
		else if (m_endState == GAMEEND_MISS)
		{
			CGame::SetClearFlag(false);
			CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(SCREEN_CENTER_POS, GAMEEND_UI_SIZE, WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_GAMEOVER), CScene::OBJTYPE_FRONT);
		}
	}

	m_nCntState--;

	// カウント経過
	if (m_nCntState <= 0)
	{
		SetState(CDecoding::STATE_NONE);
		// UIを生成
		m_endState == GAMEEND_CLEAR ?
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT) :
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT, BlackColor);
	}
}
