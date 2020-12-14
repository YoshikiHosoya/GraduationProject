//=============================================================================
//
// 接続のUI処理 [ connectUI.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "connectUI.h"
#include "polygon2D.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "client.h"
#include "mouse.h"
#include "renderer.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define INTERVAL_ANIM_LOADICON	(10)	// アニメーション処理の間隔
#define NUM_ANIMPATTER_LOADICON	(8)		// ロードアイコンのパターン数

#define TIME_FLOW_INTERVAL		(30)	// フロー切替時のインターバル
#define TIME_ALPHA_COMMENT		(10)	// コメントを透過させるまでの時間

#define FILENAME_UIINFO			("data/TEXT/ConnectUIInfo.txt")	// UI情報のファイル
#define UV_BUTTON				(D3DXVECTOR2(1.0f, 1.0f / 3))	// ボタンのUV設定用

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
CPolygon2D *CConnectUI::m_pUIBoth[2][CONNECTUI_BOTH_MAX] = {};
CPolygon2D *CConnectUI::m_pUIOnly[CONNECTUI_ONLY_MAX] = {};

CConnectUI::CONNECTUI_INFO CConnectUI::m_UIInfoBoth[2][CONNECTUI_BOTH_MAX] = {};
CConnectUI::CONNECTUI_INFO CConnectUI::m_UIInfoOnly[CONNECTUI_ONLY_MAX] = {};
CConnectUI::CONNECTFLOW_TYPE CConnectUI::m_flow = CONNECTFLOW_CONNECTING;
int CConnectUI::m_nSelectMode[PLAYER_MAX] = {};
int CConnectUI::m_nSelectLevel[PLAYER_MAX] = {};
bool CConnectUI::m_bGuestWait = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CConnectUI::CConnectUI()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CConnectUI::~CConnectUI()
{
	for (int nPlayer = 0; nPlayer < 2; nPlayer++)
	{
		for (int nUI = 0; nUI < CONNECTUI_BOTH_MAX; nUI++)
		{
			if (!m_pUIBoth[nPlayer][nUI])
			{
				continue;
			}
			delete m_pUIBoth[nPlayer][nUI];
			m_pUIBoth[nPlayer][nUI] = nullptr;
		}
	}

	for (int nUI = 0; nUI < CONNECTUI_ONLY_MAX; nUI++)
	{
		if (!m_pUIOnly[nUI])
		{
			continue;
		}
		delete m_pUIOnly[nUI];
		m_pUIOnly[nUI] = nullptr;
	}
}

//=============================================================================
// 生成
//=============================================================================
CConnectUI *CConnectUI::Create(void)
{
	// メモリ確保
	CConnectUI* pUI = new CConnectUI;

	// 確保失敗
	if (!pUI)
		return nullptr;

	CConnectUI::Load();

	// 初期化
	pUI->Init();

	// 値を返す
	return pUI;
}

//=============================================================================
// ロード
//=============================================================================
HRESULT CConnectUI::Load(void)
{
	// ファイルから1行ずつデータ読み込み
	if (CLoadFile::ReadLineByLineFromFile(FILENAME_UIINFO, ReadFromLine) != CLoadFile::LR_SUCCESS)
	{
#ifdef _DEBUG
		std::cout << "接続モードのファイル読み込みに失敗しました。" << std::endl;
#endif
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CConnectUI::Init(void)
{
	// 要素の初期化
	m_nCntAnim = 0;
	m_nCntPattern = 0;
	m_nCntState = 0;
	m_flow = CONNECTFLOW_CONNECTING;
	m_state = FLOWSTATE_BEGIN;
	m_bGuestWait = false;

	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		m_bConnect[nPlayer] = false;
		m_nSelectMode[PLAYER_TWO] = SELECTMODE_NONE;
		m_nSelectLevel[PLAYER_ONE] = SELECTLEVEL_NONE;
	}

	return S_OK;
}

//=============================================================================
// 更新
//=============================================================================
void CConnectUI::Update(void)
{
	switch (m_flow)
	{
		// 接続中の処理
	case CONNECTFLOW_CONNECTING:
		Connecting();
		break;
		// 接続完了の処理
	case CONNECTFLOW_CONNECTED:
		Connected();
		break;
		// モードの選択処理
	case CONNECTFLOW_SELECT_MODE:
		SelectMode();
		break;
	case CONNECTFLOW_SELECT_LEVEL:
		SelectLevel();
		break;
	}

	for (int nPlayer = 0; nPlayer < 2; nPlayer++)
	{
		for (int nUI = 0; nUI < CONNECTUI_BOTH_MAX; nUI++)
		{
			if (m_pUIBoth[nPlayer][nUI])
				m_pUIBoth[nPlayer][nUI]->Update();
		}
	}

	for (int nUI = 0; nUI < CONNECTUI_ONLY_MAX; nUI++)
	{
		if (m_pUIOnly[nUI])
			m_pUIOnly[nUI]->Update();
	}


	//フェードしてない時
	if (CManager::GetRenderer()->GetFade()->GetFadeState() == CFade::FADE_NONE)
	{
		if (m_flow == CONNECTFLOW_END)
		{
			//ステート変更
			if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_GAME);
			else if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE)
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_DECODING);
		}
#ifdef _DEBUG
		if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT) && CManager::GetKeyboard()->GetPress(DIK_RETURN))
		{
			//ステート変更
			if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_GAME);
			else if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE)
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_DECODING);
		}
#endif
	}
}

//=============================================================================
// 描画
//=============================================================================
void CConnectUI::Draw(void)
{
	for (int nPlayer = 0; nPlayer < 2; nPlayer++)
	{
		for (int nUI = 0; nUI < CONNECTUI_BOTH_MAX; nUI++)
		{
			if (m_pUIBoth[nPlayer][nUI])
				m_pUIBoth[nPlayer][nUI]->Draw();
		}
	}

	for (int nUI = 0; nUI < CONNECTUI_ONLY_MAX; nUI++)
	{
		if (m_pUIOnly[nUI])
			m_pUIOnly[nUI]->Draw();
	}
}

//=============================================================================
// 読み込んだ行の処理分け
//=============================================================================
void CConnectUI::ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData)
{
	// 1P2P毎のUI情報設定
	if (strcmp(cnpEntryType, "BothUI") == 0)
	{
		SetBothInfo(cnpLine, cnpEntryData);
	}
	// １つしか使わないUIの情報設定
	else if (strcmp(cnpEntryType, "OnlyUI") == 0)
	{
		SetOnlyInfo(cnpLine, cnpEntryData);
	}
}

//=============================================================================
// 1P2P毎のUI情報
//=============================================================================
void CConnectUI::SetBothInfo(CONST_STRING str, CONST_STRING type)
{
	// UIのタイプを取得
	int nType = -1;
	sscanf(type, "%d", &nType);

	if (nType <= -1)
		return;

	D3DXVECTOR2 Data = ZeroVector3;
	D3DXCOLOR Col = WhiteColor;

	// 位置
	if (sscanf(str, "pos(1P) = %f %f", &Data.x, &Data.y) == 2)
	{
		m_UIInfoBoth[0][nType].pos = Data;
	}
	if (sscanf(str, "pos(2P) = %f %f", &Data.x, &Data.y) == 2)
	{
		m_UIInfoBoth[1][nType].pos = Data;
	}
	// サイズ
	if (sscanf(str, "size(1P) = %f %f", &Data.x, &Data.y) == 2)
	{
		m_UIInfoBoth[0][nType].size = Data;
	}
	if (sscanf(str, "size(2P) = %f %f", &Data.x, &Data.y) == 2)
	{
		m_UIInfoBoth[1][nType].size = Data;
	}

	// カラー
	if (sscanf(str, "color(1P) = %f %f %f %f", &Col.r, &Col.g, &Col.b, &Col.a) == 4)
	{
		m_UIInfoBoth[0][nType].color = Col;
	}
	if (sscanf(str, "color(2P) = %f %f %f %f", &Col.r, &Col.g, &Col.b, &Col.a) == 4)
	{
		m_UIInfoBoth[1][nType].color = Col;
	}
}

//=============================================================================
// 一つだけ使うUI情報
//=============================================================================
void CConnectUI::SetOnlyInfo(CONST_STRING str, CONST_STRING type)
{
	// UIのタイプを取得
	int nType = -1;
	sscanf(type, "%d", &nType);

	if (nType <= -1)
		return;

	D3DXVECTOR2 Data = ZeroVector3;
	D3DXCOLOR Col = WhiteColor;

	// 位置
	if (sscanf(str, "pos = %f %f", &Data.x, &Data.y) == 2)
	{
		m_UIInfoOnly[nType].pos = Data;
	}
	// サイズ
	if (sscanf(str, "size = %f %f", &Data.x, &Data.y) == 2)
	{
		m_UIInfoOnly[nType].size = Data;
	}
	// カラー
	if (sscanf(str, "color = %f %f %f %f", &Col.r, &Col.g, &Col.b, &Col.a) == 4)
	{
		m_UIInfoOnly[nType].color = Col;
	}
}

//=============================================================================
// 1P2P毎に使うUI生成
//=============================================================================
CPolygon2D * CConnectUI::CreateBothUI(int nPlayer, CONNECTUITYPE_BOTH type)
{
	// 生成
	CPolygon2D *pPolygon = CPolygon2D::Create();

	// 情報設定
	pPolygon->SetPos(m_UIInfoBoth[nPlayer][type].pos);
	pPolygon->SetSize(m_UIInfoBoth[nPlayer][type].size);
	pPolygon->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
	pPolygon->SetCol(m_UIInfoBoth[nPlayer][type].color);

	// ボタンのUIはUV座標を設定する
	if (type == CONNECTUI_BOTH_MODE_REMOVE ||
		type == CONNECTUI_BOTH_MODE_SOLVE ||
		type == CONNECTUI_BOTH_LEVEL_EASY ||
		type == CONNECTUI_BOTH_LEVEL_NORMAL ||
		type == CONNECTUI_BOTH_LEVEL_HARD)
	{
		pPolygon->SetAnim(ZeroVector2, D3DXVECTOR2(1.0f, 1.0f / 3));
	}
	
	// 値を返す
	return pPolygon;
}

//=============================================================================
// 一つだけ使うUI生成
//=============================================================================
CPolygon2D * CConnectUI::CreateOnlyUI(CONNECTUITYPE_ONLY type)
{
	// 生成
	CPolygon2D *pPolygon = CPolygon2D::Create();

	// 情報設定
	pPolygon->SetPos(m_UIInfoOnly[type].pos);
	pPolygon->SetSize(m_UIInfoOnly[type].size);
	pPolygon->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
	pPolygon->SetCol(m_UIInfoOnly[type].color);

	// ボタンのUIはUV座標を設定する
	if (type == CONNECTUI_ONLY_BUTTON_DESIDE)
	{
		pPolygon->SetAnim(ZeroVector2, D3DXVECTOR2(1.0f, 1.0f / 3));
	}

	// 値を返す
	return pPolygon;
}

//=============================================================================
// UI破棄
//=============================================================================
void CConnectUI::DeleteBothUI(int nPlayer, CONNECTUITYPE_BOTH type)
{
	delete m_pUIBoth[nPlayer][type];
	m_pUIBoth[nPlayer][type] = nullptr;
}

void CConnectUI::DeleteOnlyUI(CONNECTUITYPE_ONLY type)
{
	delete m_pUIOnly[type];
	m_pUIOnly[type] = nullptr;
}

//=============================================================================
// 接続中の処理
//=============================================================================
void CConnectUI::Connecting(void)
{
	if (FLOWSTATE_BEGIN == m_state)
	{
		// フロー開始
		Begin(CONNECTFLOW_CONNECTING);
		return;
	}
	if (FLOWSTATE_END == m_state)
	{
		// フロー終了
		End(CONNECTFLOW_CONNECTING);
		return;
	}

#ifdef _DEBUG
	// 手動でゲストの接続完了
	if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT))
	{
		if (CManager::GetKeyboard()->GetTrigger(DIK_1))
			m_bConnect[PLAYER_ONE] = true;
		if (CManager::GetKeyboard()->GetTrigger(DIK_2))
			m_bConnect[PLAYER_TWO] = true;
	}
#endif

	// 接続されているか確認
	CheckConnect();

	// 両者接続できていれば、フローの終了
	if (m_bConnect[PLAYER_ONE] && m_bConnect[PLAYER_TWO])
		m_state = FLOWSTATE_END;

	// ロードアイコンのアニメーション
	LoadIconAnim();
	return;
}

//=============================================================================
// 接続されたか確認
//=============================================================================
void CConnectUI::CheckConnect(void)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		// 接続していなければ、接続状況を取得
		if (!m_bConnect[nPlayer])
		{
			nPlayer == PLAYER_ONE ?
				m_bConnect[PLAYER_ONE] = CClient::GetAccept() :
				m_bConnect[PLAYER_TWO] = CClient::GetAcceptOther();
		}

		// [接続されていない] [ UIが表示されている] なら、処理しない
		if (!m_bConnect[nPlayer] || m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTED])
			continue;

		// ロードアイコン・接続中のUIを破棄
		if (m_pUIBoth[nPlayer][CONNECTUI_BOTH_LOADICON])
			DeleteBothUI(nPlayer, CONNECTUI_BOTH_LOADICON);
		if (m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTING])
			DeleteBothUI(nPlayer, CONNECTUI_BOTH_STATE_CONNECTING);


		// 接続完了UIを生成
		m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTED] = CreateBothUI(nPlayer, CONNECTUI_BOTH_STATE_CONNECTED);
		m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTED]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_CONNECTED));
	}
}

//=============================================================================
// 接続完了の処理
//=============================================================================
void CConnectUI::Connected(void)
{
	if (FLOWSTATE_BEGIN == m_state)
	{
		// 即通常へ
		m_state = FLOWSTATE_NORMAL;
		return;
	}
	if (FLOWSTATE_END == m_state)
	{
		// フロー終了
		End(CONNECTFLOW_CONNECTED);
		return;
	}

	// 状態カウンタを加算
	m_nCntState++;

	// 時間で終了
	if (m_nCntState >= TIME_FLOW_INTERVAL)
		m_state = FLOWSTATE_END;
}

//=============================================================================
// モードの選択処理
//=============================================================================
void CConnectUI::SelectMode(void)
{
	if (FLOWSTATE_BEGIN == m_state)
	{
		// フロー開始
		Begin(CONNECTFLOW_SELECT_MODE);
		return;
	}
	if (FLOWSTATE_END == m_state)
	{
		End(CONNECTFLOW_SELECT_MODE);
		return;
	}
	if (FLOWSTATE_WAIT == m_state)
	{
		Wait(CONNECTFLOW_SELECT_MODE);
		return;
	}

#ifdef _DEBUG
	// 手動でゲストのモード設定
	if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT))
	{
		if (CManager::GetKeyboard()->GetTrigger(DIK_1))
			SetGuestMode(SELECTMODE_REMOVE);
		else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
			SetGuestMode(SELECTMODE_SOLVE);
	}
#endif

	// ボタンのアニメーション
	ButtonAnim();

	// マウスのポインタ取得
	CMouse *pMouse = CManager::GetMouse();
	// マウス座標を取得
	D3DXVECTOR2 mousePos = pMouse->GetMousePos();

	// カーソル位置がUIと重なっている
	if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE]->ReturnHit(mousePos))
	{
		// 段々表示
		D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_REMOVE]->GetCol();
		if (col.a < 1.0f)
			m_pUIOnly[CONNECTUI_ONLY_COMMENT_REMOVE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a + (1.0f / TIME_ALPHA_COMMENT)));

		// クリックしてなければ、処理しない
		if (pMouse->GetPress(0))
		{
			// アニメーションを設定
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3), UV_BUTTON);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
			m_nSelectMode[PLAYER_ONE] = SELECTMODE_REMOVE;
		}
	}
	else
	{
		// 段々透明化
		D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_REMOVE]->GetCol();
		if (col.a > 0.0f)
			m_pUIOnly[CONNECTUI_ONLY_COMMENT_REMOVE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a - (1.0f / TIME_ALPHA_COMMENT)));
	}

	// マウスカーソルとUIが重なっている
	if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE]->ReturnHit(mousePos))
	{
		// 段々表示
		D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_SOLVE]->GetCol();
		if (col.a < 1.0f)
			m_pUIOnly[CONNECTUI_ONLY_COMMENT_SOLVE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a + (1.0f / TIME_ALPHA_COMMENT)));

		// クリックしてなければ、処理しない
		if (pMouse->GetPress(0))
		{
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3), UV_BUTTON);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
			m_nSelectMode[PLAYER_ONE] = SELECTMODE_SOLVE;
		}
	}
	else
	{
		// 段々透明化
		D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_SOLVE]->GetCol();
		if (col.a > 0.0f)
			m_pUIOnly[CONNECTUI_ONLY_COMMENT_SOLVE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a - (1.0f / TIME_ALPHA_COMMENT)));
	}

	// 同じモードを選択していないか確認
	CheckSameMode();

	// 決定ボタンがあれば、クリックの確認
	if (m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
		ClickDecide(pMouse);
}

//=============================================================================
// レベルの選択処理
//=============================================================================
void CConnectUI::SelectLevel(void)
{
	if (FLOWSTATE_BEGIN == m_state)
	{
		Begin(CONNECTFLOW_SELECT_LEVEL);
		return;
	}
	if (FLOWSTATE_END == m_state)
	{
		End(CONNECTFLOW_SELECT_LEVEL);
		return;
	}
	if (FLOWSTATE_WAIT == m_state)
	{
		Wait(CONNECTFLOW_SELECT_LEVEL);
		return;
	}

	// マウスのポインタ取得
	CMouse *pMouse = CManager::GetMouse();
	// マウス座標を取得
	D3DXVECTOR2 mousePos = pMouse->GetMousePos();

	if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
	{
		// ボタンのアニメーション
		ButtonAnim();

		// カーソル位置がUIと重なっている
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]->ReturnHit(mousePos))
		{
			// 段々表示
			D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_EASY]->GetCol();
			if (col.a < 1.0f)
				m_pUIOnly[CONNECTUI_ONLY_COMMENT_EASY]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a + (1.0f / TIME_ALPHA_COMMENT)));

			// クリックしてなければ、処理しない
			if (pMouse->GetPress(0))
			{
				// アニメーションを設定
				m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3), UV_BUTTON);
				m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
				m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
				m_nSelectLevel[PLAYER_ONE] = SELECTLEVEL_EASY;
			}
		}
		else
		{
			// 段々透明化
			D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_EASY]->GetCol();
			if (col.a > 0.0f)
				m_pUIOnly[CONNECTUI_ONLY_COMMENT_EASY]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a - (1.0f / TIME_ALPHA_COMMENT)));
		}

		// マウスカーソルとUIが重なっている
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL]->ReturnHit(mousePos))
		{
			// 段々表示
			D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_NORMAL]->GetCol();
			if (col.a < 1.0f)
				m_pUIOnly[CONNECTUI_ONLY_COMMENT_NORMAL]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a + (1.0f / TIME_ALPHA_COMMENT)));

			// クリックしてなければ、処理しない
			if (pMouse->GetPress(0))
			{
				m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3), UV_BUTTON);
				m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
				m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
				m_nSelectLevel[PLAYER_ONE] = SELECTLEVEL_NORMAL;
			}
		}
		else
		{
			// 段々透明化
			D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_NORMAL]->GetCol();
			if (col.a > 0.0f)
				m_pUIOnly[CONNECTUI_ONLY_COMMENT_NORMAL]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a - (1.0f / TIME_ALPHA_COMMENT)));
		}

		// マウスカーソルとUIが重なっている
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD]->ReturnHit(mousePos))
		{
			// 段々表示
			D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_HARD]->GetCol();
			if (col.a < 1.0f)
				m_pUIOnly[CONNECTUI_ONLY_COMMENT_HARD]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a + (1.0f / TIME_ALPHA_COMMENT)));

			// クリックしてなければ、処理しない
			if (pMouse->GetPress(0))
			{
				m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3), UV_BUTTON);
				m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
				m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
				m_nSelectLevel[PLAYER_ONE] = SELECTLEVEL_HARD;
			}
		}
		else
		{
			// 段々透明化
			D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_HARD]->GetCol();
			if (col.a > 0.0f)
				m_pUIOnly[CONNECTUI_ONLY_COMMENT_HARD]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a - (1.0f / TIME_ALPHA_COMMENT)));
		}

		if (!m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
		{
			if (m_nSelectLevel[PLAYER_ONE])
			{
				// 決定ボタンを生成
				m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE] = CreateOnlyUI(CONNECTUI_ONLY_BUTTON_DESIDE);
				m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_DECIDE));
				m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);

				// 注意書きを破棄
				if (m_pUIOnly[CONNECTUI_ONLY_CAUTION_LEVEL])
					DeleteOnlyUI(CONNECTUI_ONLY_CAUTION_LEVEL);
			}
		}
	}
	else
	{
#ifdef _DEBUG
		// 手動でゲストのモード設定
		if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT))
		{
			if (CManager::GetKeyboard()->GetTrigger(DIK_1))
				SetGuestLevel(SELECTLEVEL_EASY);
			else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
				SetGuestLevel(SELECTLEVEL_NORMAL);
			else if (CManager::GetKeyboard()->GetTrigger(DIK_3))
				SetGuestLevel(SELECTLEVEL_HARD);
		}
#endif
		if (!m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
		{
			// 決定ボタンを生成
			if (m_nSelectLevel[PLAYER_TWO] != SELECTLEVEL_NONE)
			{
				m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE] = CreateOnlyUI(CONNECTUI_ONLY_BUTTON_DESIDE);
				m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_DECIDE));
				m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
			}

			// 決定ボタンを破棄
			if (m_pUIOnly[CONNECTUI_ONLY_CAUTION_LEVEL])
				DeleteOnlyUI(CONNECTUI_ONLY_CAUTION_LEVEL);
		}
	}

	// 決定ボタンがあるば、クリックの確認
	if (m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
		ClickDecide(pMouse);
}

//=============================================================================
// ゲストのモード選択
//=============================================================================
void CConnectUI::SetGuestMode(SELECTMODE mode)
{
	// フローが違えば処理しない
	if (m_flow != CONNECTFLOW_SELECT_MODE)
		return;

	m_nSelectMode[PLAYER_TWO] = mode;

	// 選択
	if (!m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED])
	{
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED] = CreateBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTED);
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_GUEST_SELECTED));

		// 選択中を破棄
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTING);
	}

	// 解除
	if (mode == SELECTMODE_REMOVE)
	{
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_REMOVE] = CreateBothUI(PLAYER_TWO, CONNECTUI_BOTH_MODE_REMOVE);
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_REMOVE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_REMOVE));
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_REMOVE]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);

		// 解読を破棄
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_SOLVE])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_MODE_SOLVE);
	}
	// 解読
	if (mode == SELECTMODE_SOLVE)
	{
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_SOLVE] = CreateBothUI(PLAYER_TWO, CONNECTUI_BOTH_MODE_SOLVE);
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_SOLVE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_SOLVE));
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_SOLVE]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);

		// 解除を破棄
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_REMOVE])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_MODE_REMOVE);
	}
}

//=============================================================================
// ゲストのレベル選択
//=============================================================================
void CConnectUI::SetGuestLevel(SELECTLEVEL level)
{
	// フローが違えば処理しない
	if (m_flow != CONNECTFLOW_SELECT_LEVEL)
		return;

	m_nSelectLevel[PLAYER_TWO] = level;

	// 選択
	if (!m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED])
	{
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED] = CreateBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTED);
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_GUEST_SELECTED));

		// 選択中を破棄
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTING);
	}

	// イージー
	if (level == SELECTLEVEL_EASY)
	{
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY] = CreateBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_EASY);
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_EASY));
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);

		// 他の難易度を破棄
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_NORMAL);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_HARD);
	}
	// 解読
	if (level == SELECTLEVEL_NORMAL)
	{
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL] = CreateBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_NORMAL);
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_NORMAL));
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);

		// 他の難易度を破棄
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_EASY);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_HARD);
	}
	if (level == SELECTLEVEL_HARD)
	{
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD] = CreateBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_HARD);
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_HARD));
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);

		// 他の難易度を破棄
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_NORMAL);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_EASY);
	}
}

//=============================================================================
// フローごとの開始時の処理
//=============================================================================
void CConnectUI::Begin(CONNECTFLOW_TYPE flow)
{
	switch (flow)
	{
		// 接続中
	case CONNECTFLOW_CONNECTING:
		// 生成
		for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
		{
			// 背景
			m_pUIBoth[nPlayer][CONNECTUI_BOTH_BACK] = CreateBothUI(nPlayer, CONNECTUI_BOTH_BACK);
			// ロードアイコン
			m_pUIBoth[nPlayer][CONNECTUI_BOTH_LOADICON] = CreateBothUI(nPlayer, CONNECTUI_BOTH_LOADICON);
			m_pUIBoth[nPlayer][CONNECTUI_BOTH_LOADICON]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f / NUM_ANIMPATTER_LOADICON, 1.0f));
			// 接続中
			m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTING] = CreateBothUI(nPlayer, CONNECTUI_BOTH_STATE_CONNECTING);
		}
		// テクスチャのバインド
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_BACK]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_BACK_00));
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_BACK]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_BACK_01));
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LOADICON]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_LOAD));
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LOADICON]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_LOAD));
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_STATE_CONNECTING]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_CONNECTING));
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_CONNECTING]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_CONNECTING));
		break;

		// モード選択
	case CONNECTFLOW_SELECT_MODE:
		// プレイヤーのUI
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_MODE] = CreateBothUI(PLAYER_ONE, CONNECTUI_BOTH_SELECT_MODE);
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE] = CreateBothUI(PLAYER_ONE, CONNECTUI_BOTH_MODE_REMOVE);
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE] = CreateBothUI(PLAYER_ONE, CONNECTUI_BOTH_MODE_SOLVE);
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_MODE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_MODE));
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_REMOVE));
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_SOLVE));
		// ゲストのUI
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING] = CreateBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTING);
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_GUEST_SELECTING));
		// 他のUI
		m_pUIOnly[CONNECTUI_ONLY_CAUTION_MODE] = CreateOnlyUI(CONNECTUI_ONLY_CAUTION_MODE);
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_REMOVE] = CreateOnlyUI(CONNECTUI_ONLY_COMMENT_REMOVE);
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_SOLVE] = CreateOnlyUI(CONNECTUI_ONLY_COMMENT_SOLVE);
		m_pUIOnly[CONNECTUI_ONLY_CAUTION_MODE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_CAUTION_MODE));
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_REMOVE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_COMMENT_REMOVE));
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_SOLVE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_COMMENT_SOLVE));
		break;

		// レベル選択時
	case CONNECTFLOW_SELECT_LEVEL:
		// プレイヤーのUI
		if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
		{
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_LEVEL]	= CreateBothUI(PLAYER_ONE, CONNECTUI_BOTH_SELECT_LEVEL);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]	= CreateBothUI(PLAYER_ONE, CONNECTUI_BOTH_LEVEL_EASY);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL]	= CreateBothUI(PLAYER_ONE, CONNECTUI_BOTH_LEVEL_NORMAL);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD]	= CreateBothUI(PLAYER_ONE, CONNECTUI_BOTH_LEVEL_HARD);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_LEVEL]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_LEVEL));
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_EASY));
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_NORMAL));
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_HARD));
		}
		// ゲストのUI
		else if (m_nSelectMode[PLAYER_TWO] == SELECTMODE_REMOVE)
		{
			m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING] = CreateBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTING);
			m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_GUEST_SELECTING));
		}

		// 他のUI
		m_pUIOnly[CONNECTUI_ONLY_CAUTION_LEVEL] = CreateOnlyUI(CONNECTUI_ONLY_CAUTION_LEVEL);
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_EASY] = CreateOnlyUI(CONNECTUI_ONLY_COMMENT_EASY);
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_NORMAL] = CreateOnlyUI(CONNECTUI_ONLY_COMMENT_NORMAL);
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_HARD] = CreateOnlyUI(CONNECTUI_ONLY_COMMENT_HARD);
		m_pUIOnly[CONNECTUI_ONLY_CAUTION_LEVEL]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_CAUTION_LEVEL));
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_EASY]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_COMMENT_EASY));
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_NORMAL]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_COMMENT_NORMAL));
		m_pUIOnly[CONNECTUI_ONLY_COMMENT_HARD]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_COMMENT_HARD));

		break;
	}

	// 次の状態へ
	m_state = FLOWSTATE_NORMAL;
}

//=============================================================================
// フローごとの終了時の処理
//=============================================================================
void CConnectUI::End(CONNECTFLOW_TYPE flow)
{
	switch (flow)
	{
		// 接続中
	case CONNECTFLOW_CONNECTING:
		// 次のフローへ
		m_flow = CONNECTFLOW_CONNECTED;
		break;

	case CONNECTFLOW_CONNECTED:
		// 接続完了のUIを破棄
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_STATE_CONNECTED])
			DeleteBothUI(PLAYER_ONE, CONNECTUI_BOTH_STATE_CONNECTED);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_CONNECTED])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_CONNECTED);
		// カウンタをリセット
		m_nCntState = 0;
		// 次のフローへ
		m_flow = CONNECTFLOW_SELECT_MODE;
		break;

	case CONNECTFLOW_SELECT_MODE:
		// プレイヤー
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_MODE])
			DeleteBothUI(PLAYER_ONE, CONNECTUI_BOTH_SELECT_MODE);
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE])
			DeleteBothUI(PLAYER_ONE, CONNECTUI_BOTH_MODE_REMOVE);
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE])
			DeleteBothUI(PLAYER_ONE, CONNECTUI_BOTH_MODE_SOLVE);
		// ゲスト
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTED);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTING);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_REMOVE])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_MODE_REMOVE);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_SOLVE])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_MODE_SOLVE);
		// コメント
		if (m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
			DeleteOnlyUI(CONNECTUI_ONLY_BUTTON_DESIDE);
		if (m_pUIOnly[CONNECTUI_ONLY_CAUTION_MODE])
			DeleteOnlyUI(CONNECTUI_ONLY_CAUTION_MODE);
		if (m_pUIOnly[CONNECTUI_ONLY_COMMENT_REMOVE])
			DeleteOnlyUI(CONNECTUI_ONLY_COMMENT_REMOVE);
		if (m_pUIOnly[CONNECTUI_ONLY_COMMENT_SOLVE])
			DeleteOnlyUI(CONNECTUI_ONLY_COMMENT_SOLVE);
		// 次のフローへ
		m_flow = CONNECTFLOW_SELECT_LEVEL;
		break;
	case CONNECTFLOW_SELECT_LEVEL:
		// プレイヤー
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_LEVEL])
			DeleteBothUI(PLAYER_ONE, CONNECTUI_BOTH_SELECT_LEVEL);
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY])
			DeleteBothUI(PLAYER_ONE, CONNECTUI_BOTH_LEVEL_EASY);
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL])
			DeleteBothUI(PLAYER_ONE, CONNECTUI_BOTH_LEVEL_NORMAL);
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD])
			DeleteBothUI(PLAYER_ONE, CONNECTUI_BOTH_LEVEL_HARD);
		// ゲスト
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_EASY);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_NORMAL);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_LEVEL_HARD);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTED);
		if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED])
			DeleteBothUI(PLAYER_TWO, CONNECTUI_BOTH_STATE_SELECTED);
		// 他のUI
		if (m_pUIOnly[CONNECTUI_ONLY_CAUTION_LEVEL])
			DeleteOnlyUI(CONNECTUI_ONLY_CAUTION_LEVEL);
		if (m_pUIOnly[CONNECTUI_ONLY_COMMENT_EASY])
			DeleteOnlyUI(CONNECTUI_ONLY_COMMENT_EASY);
		if (m_pUIOnly[CONNECTUI_ONLY_COMMENT_NORMAL])
			DeleteOnlyUI(CONNECTUI_ONLY_COMMENT_NORMAL);
		if (m_pUIOnly[CONNECTUI_ONLY_COMMENT_HARD])
			DeleteOnlyUI(CONNECTUI_ONLY_COMMENT_HARD);
		if (m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
			DeleteOnlyUI(CONNECTUI_ONLY_BUTTON_DESIDE);
		m_flow = CONNECTFLOW_END;
		break;
	}

	// 次の状態へ
	m_state = FLOWSTATE_BEGIN;
}

//=============================================================================
// フローごとの待ち状態の処理
//=============================================================================
void CConnectUI::Wait(CONNECTFLOW_TYPE flow)
{
	switch (flow)
	{
	case CONNECTFLOW_SELECT_MODE:
		// 両者とも待ち状態で、選択も重複しない
		if (m_bGuestWait && 
			((m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE && m_nSelectMode[PLAYER_TWO] == SELECTMODE_SOLVE) ||
			(m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE && m_nSelectMode[PLAYER_TWO] == SELECTMODE_REMOVE)))
		{
			// 次へ
			m_state = FLOWSTATE_END;
		}
#ifdef _DEBUG
		else if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT) &&
			CManager::GetKeyboard()->GetTrigger(DIK_3))
		{
			// 強制書き換え
			m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE ?
				m_nSelectMode[PLAYER_TWO] = SELECTMODE_SOLVE :
				m_nSelectMode[PLAYER_TWO] = SELECTMODE_REMOVE;
			// 次へ
			m_state = FLOWSTATE_END;
		}
#endif
		break;
	case CONNECTFLOW_SELECT_LEVEL:
		if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
		{
			if (m_bGuestWait)
				m_state = FLOWSTATE_END;
#ifdef _DEBUG
			else if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT) &&
				CManager::GetKeyboard()->GetTrigger(DIK_4))
			{
				// 強制書き換え
				m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE ?
					m_nSelectMode[PLAYER_TWO] = SELECTMODE_SOLVE :
					m_nSelectMode[PLAYER_TWO] = SELECTMODE_REMOVE;
				// 次へ
				m_state = FLOWSTATE_END;
			}
#endif
		}
		else if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE)
		{
			if (m_nSelectLevel[PLAYER_TWO] != SELECTLEVEL_NONE)
				m_state = FLOWSTATE_END;
		}
		break;
	}
}

//=============================================================================
// ロードアイコンのアニメーション
//=============================================================================
void CConnectUI::LoadIconAnim(void)
{
	// ロードアイコンが存在すれば、カウンタ加算
	if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LOADICON])
		m_nCntAnim++;
	else if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LOADICON])
		m_nCntAnim++;

	// 一定時間内なら、処理を終える
	if (m_nCntAnim < INTERVAL_ANIM_LOADICON)
		return;

	// 一定フレームおきにアニメーション
	m_nCntPattern++;
	if (m_nCntPattern >= NUM_ANIMPATTER_LOADICON)
		m_nCntPattern = 0;
	m_nCntAnim = 0;

	if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LOADICON])
		m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LOADICON]->SetAnim(D3DXVECTOR2(0.0f + (1.0f / NUM_ANIMPATTER_LOADICON * m_nCntPattern), 0.0f), D3DXVECTOR2(1.0f / NUM_ANIMPATTER_LOADICON, 1.0f));
	if (m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LOADICON])
		m_pUIBoth[PLAYER_TWO][CONNECTUI_BOTH_LOADICON]->SetAnim(D3DXVECTOR2(0.0f + (1.0f / NUM_ANIMPATTER_LOADICON * m_nCntPattern), 0.0f), D3DXVECTOR2(1.0f / NUM_ANIMPATTER_LOADICON, 1.0f));

}

//=============================================================================
// ボタンのアニメーション
//=============================================================================
void CConnectUI::ButtonAnim(void)
{
	if (m_flow == CONNECTFLOW_SELECT_MODE)
	{
		// 解除
		if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);
		// 解読
		if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE)
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);
		// どちらでもない
		if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_NONE)
		{
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
		}
	}
	if (m_flow == CONNECTFLOW_SELECT_LEVEL)
	{
		// イージー
		if (m_nSelectLevel[PLAYER_ONE] == SELECTLEVEL_EASY)
		{
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
		}
		// ノーマル
		if (m_nSelectLevel[PLAYER_ONE] == SELECTLEVEL_NORMAL)
		{
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
		}
		// ハード
		if (m_nSelectLevel[PLAYER_ONE] == SELECTLEVEL_HARD)
		{
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3 * 2), UV_BUTTON);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
			m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
		}
	}
}

//=============================================================================
// 同じモードを選択していないか確認処理
//=============================================================================
void CConnectUI::CheckSameMode(void)
{
	// 別々のモードであれば、処理しない
	if ((m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE && m_nSelectMode[PLAYER_TWO] == SELECTMODE_SOLVE) ||
		(m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE && m_nSelectMode[PLAYER_TWO] == SELECTMODE_REMOVE))
	{
		// 決定ボタンを生成
		if (!m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
		{
			m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE] = CreateOnlyUI(CONNECTUI_ONLY_BUTTON_DESIDE);
			m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_DECIDE));
			m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), UV_BUTTON);
		}

		// 決定ボタンを破棄
		if (m_pUIOnly[CONNECTUI_ONLY_CAUTION_MODE])
			DeleteOnlyUI(CONNECTUI_ONLY_CAUTION_MODE);
		return;
	}

	// 決定ボタンを破棄
	if (m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
		DeleteOnlyUI(CONNECTUI_ONLY_BUTTON_DESIDE);


	// 注意書きを生成
	if (!m_pUIOnly[CONNECTUI_ONLY_CAUTION_MODE])
	{
		m_pUIOnly[CONNECTUI_ONLY_CAUTION_MODE] = CreateOnlyUI(CONNECTUI_ONLY_CAUTION_MODE);
		m_pUIOnly[CONNECTUI_ONLY_CAUTION_MODE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_CAUTION_MODE));
	}
}

//=============================================================================
// 決定ボタンをクリック
//=============================================================================
void CConnectUI::ClickDecide(CMouse *pMouse)
{
	// マウスカーソルとUIが重なっている
	if (!m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE]->ReturnHit(pMouse->GetMousePos()))
		return;

	// クリックしている
	if (pMouse->GetPress(0))
	{
		// ゲストを待つ状態へ
		m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE]->SetAnim(D3DXVECTOR2(0.0f, 1.0f / 3), UV_BUTTON);
		m_state = FLOWSTATE_WAIT;
	}
}
