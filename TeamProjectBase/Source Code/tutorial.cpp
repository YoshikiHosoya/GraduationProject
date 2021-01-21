//------------------------------------------------------------------------------
//
//チュートリアル処理  [tutorial.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "tutorial.h"
#include "camera.h"
#include "fade.h"
#include "result.h"
#include "sound.h"
#include "scene2D.h"
#include "connectUI.h"
#include "mouse.h"
#include "UI_Base.h"
#include "client.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
bool CTutorial::m_bGuestFlag = false;

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CTutorial::CTutorial()
{
	m_nCntState = 0;
	m_pTutorialPolygon.reset();
	m_type = (CTutorial::TUTORIL_TYPE)CConnectUI::GetSelectMode();
	m_bTutorialEndFlag = false;
	m_bGuestFlag = false;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CTutorial::~CTutorial()
{
	m_pTutorialPolygon.reset();
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CTutorial::Init(HWND hWnd)
{

	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	CreateUI();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CTutorial::Update()
{
	//ステートに応じた更新
	UpdateState();

	Collision();

	//フェードしてない時
	if (CManager::GetRenderer()->GetFade()->GetFadeState() == CFade::FADE_NONE)
	{
		//ゲスト・プレイヤーが待機中のとき
		if (m_bGuestFlag && m_bTutorialEndFlag)
		{
			// ゲームへ遷移
			if (m_type == CTutorial::TUTORIAL_REMOVE)
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_GAME);
			else if (m_type == CTutorial::TUTORIAL_SOLVE)
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_DECODING);
			// SE
			CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		}
	}
}

//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CTutorial::Draw()
{

}
//------------------------------------------------------------------------------
//UI生成
//------------------------------------------------------------------------------
void CTutorial::CreateUI()
{

	m_pReady = CSceneBase::ScenePolygonCreateShared<CScene2D>
		(D3DXVECTOR3(1110.0f, 140.0f, 0.0f), D3DXVECTOR3(250.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_TUTORIAL_PREPARE), CScene::OBJTYPE_FRONT);


	switch (m_type)
	{
		//解除
	case CTutorial::TUTORIAL_REMOVE:
		m_pTutorialPolygon = CSceneBase::ScenePolygonCreateShared<CScene2D>
			(SCREEN_CENTER_POS, SCREEN_SIZE, WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_TUTORIAL_REMOVE01), CScene::OBJTYPE_UI);
		break;

		//解読
	case CTutorial::TUTORIAL_SOLVE:
		m_pTutorialPolygon = CSceneBase::ScenePolygonCreateShared<CScene2D>
			(SCREEN_CENTER_POS, SCREEN_SIZE, WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_TUTORIAL_SOLVE01), CScene::OBJTYPE_UI);

		break;
	}
}
//------------------------------------------------------------------------------
//ステート更新
//------------------------------------------------------------------------------
void CTutorial::UpdateState()
{

}
//------------------------------------------------------------------------------
//当たり判定
//------------------------------------------------------------------------------
void CTutorial::Collision()
{

	CMouse *pMouse = CManager::GetMouse();
	D3DXVECTOR3 pos = pMouse->GetMousePos();

	static int nMousePressCnt = 0;

	//左クリック
	if(pMouse->GetTrigger(0))
	{
		m_nPage--;
		if (!CHossoLibrary::RangeLimit_Equal(m_nPage, 0, 3))
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_TUTORIALPAGE);
		}
	}
	//右クリック
	if (pMouse->GetTrigger(1))
	{
		m_nPage++;
		if (!CHossoLibrary::RangeLimit_Equal(m_nPage, 0, 3))
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_TUTORIALPAGE);
		}
	}

	//長押し
	if (pMouse->GetPress(0) || pMouse->GetPress(1))
	{
		//カウントアップ
		nMousePressCnt++;
	}
	//押してない時リセット
	else
	{
		nMousePressCnt = 0;
	}

	//ある程度長押しで準備完了
	if (nMousePressCnt >= 60)
	{
		//フラグたってない時
		if (!m_bTutorialEndFlag)
		{
			//準備かんりょう
			Ready();
		}
		//フラグを立てる
		m_bTutorialEndFlag = true;
	}



	//ページ変更
	PageChange();

}


//------------------------------------------------------------------------------
//ページ変更
//------------------------------------------------------------------------------
void CTutorial::PageChange()
{
	switch (m_type)
	{
		//解除
	case CTutorial::TUTORIAL_REMOVE:
		m_pTutorialPolygon->BindTexture(CTexture::GetTexture((CTexture::TEX_TYPE)(m_nPage + CTexture::TEX_UI_TUTORIAL_REMOVE01)));
		break;

		//解読
	case CTutorial::TUTORIAL_SOLVE:
		m_pTutorialPolygon->BindTexture(CTexture::GetTexture((CTexture::TEX_TYPE)(m_nPage + CTexture::TEX_UI_TUTORIAL_SOLVE01)));

		break;
	}


}

//------------------------------------------------------------------------------
//準備完了
//------------------------------------------------------------------------------
void CTutorial::Ready()
{
	//テクスチャ差し替え
	m_pReady->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_TUTORIAL_ENDPREPARE));
	// 待ち状態の送信
	CClient::SendWait();
}
