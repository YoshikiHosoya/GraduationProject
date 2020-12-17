//------------------------------------------------------------------------------
//
//リザルト処理  [result.cpp]
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
#include "result.h"
#include "sound.h"
#include "scene2D.h"
#include "game.h"
#include "timer.h"
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
CResult::CResult()
{
	m_nCntState = 0;
	m_state = RESULT_STATE::UP_BAR_IN;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CResult::~CResult()
{
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CResult::Init(HWND hWnd)
{

	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	//ロゴ
	m_pPolygonList.emplace_back(
		CUI_Base::Create(D3DXVECTOR3(640.0f, 100.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_LOGO), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::TRANSPARENT_TO_APPEAR, 60));


	SetState(UP_BAR_IN);



	//CSceneBase::ScenePolygonCreateSceneManagement<CScene2D>(D3DXVECTOR3(640.0f, 600.0f, 0.0f), D3DXVECTOR3(800.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_PRESSBUTTON), CScene::OBJTYPE_UI);

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CResult::Update()
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
void CResult::Draw()
{

}
//------------------------------------------------------------------------------
//ステート更新
//------------------------------------------------------------------------------
void CResult::UpdateState()
{
	//カウンタ++
	m_nCntState--;

	switch (m_state)
	{
	case CResult::UP_BAR_IN:
		if (m_nCntState < 0)
		{
			SetState(RESULT_DISP);
		}
		break;
	case CResult::RESULT_DISP:
		if (m_nCntState < 0)
		{
			SetState(PLAYTIME_DISP);
		}
		break;
	case CResult::PLAYTIME_DISP:
		if (m_nCntState < 0)
		{
			SetState(STAY);
		}
		break;
	case CResult::STAY:
		if (CManager::GetMouse()->GetTrigger(0))
		{
			////暗転
			SetState(CResult::RESULT_STATE::SELECT);
		}
		break;
	case CResult::SELECT:
		Collision();
		break;

	case CResult::FADE:

		break;
	default:
		break;
	}
}
//------------------------------------------------------------------------------
//当たり判定
//------------------------------------------------------------------------------
void CResult::Collision()
{

	CMouse *pMouse = CManager::GetMouse();
	D3DXVECTOR3 pos = pMouse->GetMousePos();

	//クリック時
	if (pMouse->GetTrigger(0))
	{
		//マウスとポリゴンの判定
		if (CHossoLibrary::Collision_PointTo2DPolygon(pos, m_pPolygonList[RESULT_POLYGON_TYPE::RETRY].get()))
		{
			//暗転
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_GAME);
		}
		//マウスとポリゴンの判定
		if (CHossoLibrary::Collision_PointTo2DPolygon(pos, m_pPolygonList[RESULT_POLYGON_TYPE::END].get()))
		{
			//ゲーム終了
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_TITLE);
		}
	}

}
//------------------------------------------------------------------------------
//ステート切り替え
//------------------------------------------------------------------------------
void CResult::SetState(RESULT_STATE state)
{

	m_state = state;
	m_nCntState = 0;

	switch (state)
	{
	case CResult::UP_BAR_IN:
		//バー
		m_pPolygonList.emplace_back(
			CUI_Base::Create(D3DXVECTOR3(640.0f, 150.0f, 0.0f), D3DXVECTOR3(1300.0f, 5.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_BAR), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::RIGHT_TO_CENTER, 60));
		m_nCntState = 60;

		//バー
		m_pPolygonList.emplace_back(
			CUI_Base::Create(D3DXVECTOR3(640.0f, 550.0f, 0.0f), D3DXVECTOR3(1300.0f, 5.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_BAR), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::RIGHT_TO_CENTER, 60));
		m_nCntState = 60;

		break;
	case CResult::RESULT_DISP:
		//クリアしたか
		m_pPolygonList.emplace_back(
			CGame::GetClearFlag() ?
			CUI_Base::Create(D3DXVECTOR3(640.0f, 200.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_CLEAR), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::TRANSPARENT_TO_APPEAR, 60) :
			CUI_Base::Create(D3DXVECTOR3(640.0f, 200.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_FAILED), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::TRANSPARENT_TO_APPEAR, 60));
		m_nCntState = 60;


		break;

	case CResult::PLAYTIME_DISP:
		m_nCntState = 60;

		//プレイタイム
		m_pPolygonList.emplace_back(
			CUI_Base::Create(D3DXVECTOR3(450.0f, 500.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_TIMER), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::TRANSPARENT_TO_APPEAR, 60));

		break;


	case CResult::STAY:
		//タイマー
		m_pTimer = CTimer::Create(D3DXVECTOR3(900.0f, 500.0f, 0.0f), nullptr, false);

		//セミコロン
		m_pPolygonList.emplace_back(
			CUI_Base::Create(D3DXVECTOR3(855.0f, 500.0f, 0.0f), D3DXVECTOR3(40.0f, 55.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_SEMICOLON), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::NORMAL, 0));

		//コンマ
		m_pPolygonList.emplace_back(
			CUI_Base::Create(D3DXVECTOR3(930.0f, 520.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_COMMA), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::NORMAL, 0));

		break;
	case CResult::SELECT:

		//ウィンドウ
		m_pPolygonList.emplace_back(
			CUI_Base::Create(SCREEN_CENTER_POS, D3DXVECTOR3(400.0f, 500.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_WINDOW), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::TRANSPARENT_TO_APPEAR, 20));

		//リトライ
		m_pPolygonList.emplace_back(
			CUI_Base::Create(D3DXVECTOR3(640.0f, 260.0f, 0.0f), D3DXVECTOR3(300.0f, 150.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_RETRY), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::TRANSPARENT_TO_APPEAR, 20));

		//エンド
		m_pPolygonList.emplace_back(
			CUI_Base::Create(D3DXVECTOR3(640.0f, 460.0f, 0.0f), D3DXVECTOR3(300.0f, 150.0f, 0.0f), WhiteColor, CTexture::GetTexture(CTexture::TEX_UI_RESULT_END), CScene::OBJTYPE_UI, CUI_Base::APPEAR_PATTERN::TRANSPARENT_TO_APPEAR, 20));

	case CResult::FADE:
		break;

	default:
		break;
	}
}
