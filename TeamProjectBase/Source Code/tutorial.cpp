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
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CTutorial::CTutorial()
{
	m_nCntState = 0;
	m_pPolygonList.clear();
	m_state = (CTutorial::TUTORIL_TYPE)CConnectUI::GetSelectMode();



	//m_state = CTutorial::TUTORIAL_REMOVE;
	m_state = CTutorial::TUTORIAL_SOLVE;

}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CTutorial::~CTutorial()
{
	m_pPolygonList.clear();
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

	//フェードしてない時
	if (CManager::GetRenderer()->GetFade()->GetFadeState() == CFade::FADE_NONE)
	{
		//何かボタン押したとき
		if (CHossoLibrary::CheckAnyButton())
		{
				//ステート変更
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_TITLE);

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
	switch (m_state)
	{
		//解除
	case CTutorial::TUTORIAL_REMOVE:
		m_pPolygonList.emplace_back(CSceneBase::ScenePolygonCreateShared<CScene2D>
			(SCREEN_CENTER_POS, SCREEN_SIZE, WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_TUTORIAL_REMOVE01), CScene::OBJTYPE_UI));
		break;

		//解読
	case CTutorial::TUTORIAL_SOLVE:
		m_pPolygonList.emplace_back(CSceneBase::ScenePolygonCreateShared<CScene2D>
			(SCREEN_CENTER_POS, SCREEN_SIZE, WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_TUTORIAL_SOLVE01), CScene::OBJTYPE_UI));

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

}
