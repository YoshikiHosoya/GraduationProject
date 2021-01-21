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
#include "Debug\debugproc.h"
#include "game.h"
#include "Bomb.h"
#include "sound.h"

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

bool CConnectUI::m_bCreateFlagBoth[2][CONNECTUI_BOTH_MAX] = {};
bool CConnectUI::m_bCreateFlagOnly[CONNECTUI_ONLY_MAX] = {};
bool CConnectUI::m_bDeleteFlagBoth[2][CONNECTUI_BOTH_MAX] = {};
bool CConnectUI::m_bDeleteFlagOnly[CONNECTUI_ONLY_MAX] = {};

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

		for (int nUI = 0; nUI < CONNECTUI_BOTH_MAX; nUI++)
		{
			m_bCreateFlagBoth[nPlayer][nUI] = false;
			m_bDeleteFlagBoth[nPlayer][nUI] = false;
			m_UIInfoBoth[nPlayer][nUI].buttonState = BUTTON_NORMAL;
		}
	}

	for (int nUI = 0; nUI < CONNECTUI_ONLY_MAX; nUI++)
	{
		m_bCreateFlagOnly[nUI] = false;
		m_bDeleteFlagOnly[nUI] = false;
		m_UIInfoOnly[nUI].buttonState = BUTTON_NORMAL;
	}

	return S_OK;
}

//=============================================================================
// 更新
//=============================================================================
void CConnectUI::Update(void)
{
	// UI生成
	CreateUI();
	// UI破棄
	DeleteUI();

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
			{
				if (m_UIInfoBoth[nPlayer][nUI].type == UITYPE_BUTTON)
					ButtonAnimBoth(nPlayer, nUI);

				m_pUIBoth[nPlayer][nUI]->Update();
			}
		}
	}

	for (int nUI = 0; nUI < CONNECTUI_ONLY_MAX; nUI++)
	{
		if (m_pUIOnly[nUI])
		{
			if (m_UIInfoOnly[nUI].type == UITYPE_BUTTON)
				ButtonAnimOnly(nUI);

			m_pUIOnly[nUI]->Update();
		}
	}


	//フェードしてない時
	if (CManager::GetRenderer()->GetFade()->GetFadeState() == CFade::FADE_NONE)
	{
		if (m_flow == CONNECTFLOW_END)
		{
			//ステート変更
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_TUTORIAL);
		}
#ifdef _DEBUG
		if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT) && CManager::GetKeyboard()->GetPress(DIK_RETURN))
		{
			//ステート変更
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_TUTORIAL);
		}
#endif
	}
}

//=============================================================================
// ボタンのアニメーション
//=============================================================================
void CConnectUI::ButtonAnimBoth(int nPlayer, int type)
{
	switch (m_UIInfoBoth[nPlayer][type].buttonState)
	{
	case BUTTON_NORMAL: m_pUIBoth[nPlayer][type]->SetAnim(D3DXVECTOR2(0.0f, 0.0f + (1.0f / 3) * 0), UV_BUTTON); break;
	case BUTTON_PRESS:	m_pUIBoth[nPlayer][type]->SetAnim(D3DXVECTOR2(0.0f, 0.0f + (1.0f / 3) * 1), UV_BUTTON); break;
	case BUTTON_SELECT: m_pUIBoth[nPlayer][type]->SetAnim(D3DXVECTOR2(0.0f, 0.0f + (1.0f / 3) * 2), UV_BUTTON); break;
	}
}

void CConnectUI::ButtonAnimOnly(int type)
{
	switch (m_UIInfoOnly[type].buttonState)
	{
	case BUTTON_NORMAL: m_pUIOnly[type]->SetAnim(D3DXVECTOR2(0.0f, 0.0f + (1.0f / 3) * 0), UV_BUTTON); break;
	case BUTTON_PRESS:	m_pUIOnly[type]->SetAnim(D3DXVECTOR2(0.0f, 0.0f + (1.0f / 3) * 1), UV_BUTTON); break;
	case BUTTON_SELECT: m_pUIOnly[type]->SetAnim(D3DXVECTOR2(0.0f, 0.0f + (1.0f / 3) * 2), UV_BUTTON); break;
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

#ifdef _DEBUG
//=============================================================================
// デバッグコマンド
//=============================================================================
void CConnectUI::DebugCommand(void)
{
	if (m_flow == CONNECTFLOW_CONNECTING)
	{
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "1P接続完了 [左Shift + 1]\n");
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "2P接続完了 [左Shift + 2]\n");

		// 手動でゲストの接続完了
		if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT))
		{
			if (CManager::GetKeyboard()->GetTrigger(DIK_1))
				m_bConnect[PLAYER_ONE] = true;
			if (CManager::GetKeyboard()->GetTrigger(DIK_2))
				m_bConnect[PLAYER_TWO] = true;
		}
	}
	// モード選択
	if (m_flow == CONNECTFLOW_SELECT_MODE)
	{
		// 通常
		if (m_state == FLOWSTATE_NORMAL)
		{
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト解除選択 [左Shift + 1]\n");
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト解読選択 [左Shift + 2]\n");

			// 手動でゲストのモード設定
			if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT))
			{
				if (CManager::GetKeyboard()->GetTrigger(DIK_1))
					SetGuestMode(SELECTMODE_REMOVE);
				else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
					SetGuestMode(SELECTMODE_SOLVE);
			}
		}
		else if (m_state == FLOWSTATE_WAIT)
		{
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト解除選択 [左Shift + 1]\n");
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト解読選択 [左Shift + 2]\n");
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "次のモードへ　 [左Shift + 3]\n");

			// 手動でゲストのモード設定
			if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT))
			{
				if (CManager::GetKeyboard()->GetTrigger(DIK_1))
					SetGuestMode(SELECTMODE_REMOVE);
				else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
					SetGuestMode(SELECTMODE_SOLVE);
			}

			if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT) &&
				CManager::GetKeyboard()->GetTrigger(DIK_3))
			{
				// 強制書き換え
				m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE ?
					m_nSelectMode[PLAYER_TWO] = SELECTMODE_SOLVE :
					m_nSelectMode[PLAYER_TWO] = SELECTMODE_REMOVE;
				// 次へ
				m_state = FLOWSTATE_END;
			}
		}
	}
	else if (m_flow == CONNECTFLOW_SELECT_LEVEL)
	{
		if (m_state == FLOWSTATE_NORMAL)
		{
			if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE)
			{
				CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト選択EASY   [左Shift + 1]\n");
				CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト選択NORMAL [左Shift + 2]\n");
				CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト選択HARD   [左Shift + 3]\n");

				if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT))
				{
					// 手動でゲストのモード設定
					if (CManager::GetKeyboard()->GetTrigger(DIK_1))
						SetGuestLevel(SELECTLEVEL_EASY);
					else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
						SetGuestLevel(SELECTLEVEL_NORMAL);
					else if (CManager::GetKeyboard()->GetTrigger(DIK_3))
						SetGuestLevel(SELECTLEVEL_HARD);
				}
			}
			else if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
			{
				CDebugProc::Print(CDebugProc::PLACE_LEFT, "次のモードへ     [左Shift + 4]\n");

				// 手動でゲストが承認
				if (CManager::GetKeyboard()->GetTrigger(DIK_4))
					m_bGuestWait = true;
			}
		}
		else if (m_state == FLOWSTATE_WAIT)
		{
			if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE)
			{
				CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト選択EASY   [左Shift + 1]\n");
				CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト選択NORMAL [左Shift + 2]\n");
				CDebugProc::Print(CDebugProc::PLACE_LEFT, "ゲスト選択HARD   [左Shift + 3]\n");

				if (CManager::GetKeyboard()->GetPress(DIK_LSHIFT))
				{
					// 手動でゲストの難易度設定
					if (CManager::GetKeyboard()->GetTrigger(DIK_1))
						SetGuestLevel(SELECTLEVEL_EASY);
					else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
						SetGuestLevel(SELECTLEVEL_NORMAL);
					else if (CManager::GetKeyboard()->GetTrigger(DIK_3))
						SetGuestLevel(SELECTLEVEL_HARD);
					// 難易度が選択されていれば、手動でゲーム開始
					if (m_nSelectLevel[PLAYER_TWO] != SELECTLEVEL_NONE)
					{
						CDebugProc::Print(CDebugProc::PLACE_LEFT, "次のモードへ     [左Shift + 4]\n");

						if (CManager::GetKeyboard()->GetTrigger(DIK_4))
						{
							m_state = FLOWSTATE_END;
						}
					}
				}
			}
			else if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
			{
				CDebugProc::Print(CDebugProc::PLACE_LEFT, "次のモードへ     [左Shift + 4]\n");

				if (CManager::GetKeyboard()->GetTrigger(DIK_4))
					m_bGuestWait = true;
			}
		}
	}
}
#endif

//=============================================================================
// UI生成
//=============================================================================
void CConnectUI::CreateUI(void)
{
	for (int nPlayer = 0; nPlayer < 2; nPlayer++)
	{
		for (int nUI = 0; nUI < CONNECTUI_BOTH_MAX; nUI++)
		{
			// 存在せず、生成フラグ有効
			if (m_bCreateFlagBoth[nPlayer][nUI] && !m_pUIBoth[nPlayer][nUI])
			{
				// 生成
				m_pUIBoth[nPlayer][nUI] = CreateBothUI(nPlayer, (CONNECTUITYPE_BOTH)nUI);
				m_pUIBoth[nPlayer][nUI]->BindTexture(CTexture::GetTexture((CTexture::TEX_TYPE)GetTexNumberBoth(nPlayer, (CONNECTUITYPE_BOTH)nUI)));
			}
			// フラグ完了
			m_bCreateFlagBoth[nPlayer][nUI] = false;
		}
	}

	for (int nUI = 0; nUI < CONNECTUI_ONLY_MAX; nUI++)
	{
		// 存在せず、生成フラグ有効
		if (m_bCreateFlagOnly[nUI] && !m_pUIOnly[nUI])
		{
			// 生成
			m_pUIOnly[nUI] = CreateOnlyUI((CONNECTUITYPE_ONLY)nUI);
			m_pUIOnly[nUI]->BindTexture(CTexture::GetTexture((CTexture::TEX_TYPE)GetTexNumberOnly((CONNECTUITYPE_ONLY)nUI)));
		}
		// フラグ完了
		m_bCreateFlagOnly[nUI] = false;
	}
}

//=============================================================================
// UI破棄
//=============================================================================
void CConnectUI::DeleteUI(void)
{
	for (int nPlayer = 0; nPlayer < 2; nPlayer++)
	{
		for (int nUI = 0; nUI < CONNECTUI_BOTH_MAX; nUI++)
		{
			// 存在し、フラグ有効
			if (m_bDeleteFlagBoth[nPlayer][nUI] && m_pUIBoth[nPlayer][nUI])
			{
				// 破棄
				DeleteBothUI(nPlayer, (CONNECTUITYPE_BOTH)nUI);
			}
			// フラグ完了
			m_bDeleteFlagBoth[nPlayer][nUI] = false;
		}
	}

	for (int nUI = 0; nUI < CONNECTUI_ONLY_MAX; nUI++)
	{
		// 存在し、フラグ有効
		if (m_bDeleteFlagOnly[nUI] && m_pUIOnly[nUI])
		{
			// 破棄
			DeleteOnlyUI((CONNECTUITYPE_ONLY)nUI);
		}
		// フラグ完了
		m_bDeleteFlagOnly[nUI] = false;
	}
}

//=============================================================================
// UIのテクスチャ番号取得
//=============================================================================
int CConnectUI::GetTexNumberBoth(int nPlayer, CONNECTUITYPE_BOTH type)
{
	if (nPlayer == 0)
	{
		switch (type)
		{
		case CONNECTUI_BOTH_BACK:				return CTexture::TEX_CONNECT_BACK_00; break;			// 背景
		case CONNECTUI_BOTH_LOADICON:			return CTexture::TEX_CONNECT_LOAD; break;				// ロードアイコン
		case CONNECTUI_BOTH_STATE_CONNECTING:	return CTexture::TEX_CONNECT_CONNECTING; break;			// 接続中
		case CONNECTUI_BOTH_STATE_CONNECTED:	return CTexture::TEX_CONNECT_CONNECTED; break;			// 接続完了
		case CONNECTUI_BOTH_STATE_SELECTING:	return CTexture::TEX_CONNECT_GUEST_SELECTING; break;	// 選択中
		case CONNECTUI_BOTH_STATE_SELECTED:		return CTexture::TEX_CONNECT_GUEST_SELECTED; break;		// 選択完了
		case CONNECTUI_BOTH_SELECT_MODE:		return CTexture::TEX_CONNECT_SELECT_MODE; break;		// 選択 モード
		case CONNECTUI_BOTH_SELECT_LEVEL:		return CTexture::TEX_CONNECT_SELECT_LEVEL; break;		// 選択 レベル
		case CONNECTUI_BOTH_MODE_REMOVE:		return CTexture::TEX_CONNECT_SELECT_REMOVE; break;		// モード 解除
		case CONNECTUI_BOTH_MODE_SOLVE:			return CTexture::TEX_CONNECT_SELECT_SOLVE; break;		// モード 解読
		case CONNECTUI_BOTH_LEVEL_EASY:			return CTexture::TEX_CONNECT_SELECT_EASY; break;		// 難易度 イージー
		case CONNECTUI_BOTH_LEVEL_NORMAL:		return CTexture::TEX_CONNECT_SELECT_NORMAL; break;		// 難易度 ノーマル
		case CONNECTUI_BOTH_LEVEL_HARD:			return CTexture::TEX_CONNECT_SELECT_HARD; break;		// 難易度 ハード
		}
	}
	else if (nPlayer == 1)
	{
		switch (type)
		{
		case CONNECTUI_BOTH_BACK:				return CTexture::TEX_CONNECT_BACK_01; break;			// 背景
		case CONNECTUI_BOTH_LOADICON:			return CTexture::TEX_CONNECT_LOAD; break;				// ロードアイコン
		case CONNECTUI_BOTH_STATE_CONNECTING:	return CTexture::TEX_CONNECT_CONNECTING; break;			// 接続中
		case CONNECTUI_BOTH_STATE_CONNECTED:	return CTexture::TEX_CONNECT_CONNECTED; break;			// 接続完了
		case CONNECTUI_BOTH_STATE_SELECTING:	return CTexture::TEX_CONNECT_GUEST_SELECTING; break;	// 選択中
		case CONNECTUI_BOTH_STATE_SELECTED:		return CTexture::TEX_CONNECT_GUEST_SELECTED; break;		// 選択完了
		case CONNECTUI_BOTH_SELECT_MODE:		return CTexture::TEX_CONNECT_SELECT_MODE; break;		// 選択 モード
		case CONNECTUI_BOTH_SELECT_LEVEL:		return CTexture::TEX_CONNECT_SELECT_LEVEL; break;		// 選択 レベル
		case CONNECTUI_BOTH_MODE_REMOVE:		return CTexture::TEX_CONNECT_SELECT_REMOVE; break;		// モード 解除
		case CONNECTUI_BOTH_MODE_SOLVE:			return CTexture::TEX_CONNECT_SELECT_SOLVE; break;		// モード 解読
		case CONNECTUI_BOTH_LEVEL_EASY:			return CTexture::TEX_CONNECT_SELECT_EASY; break;		// 難易度 イージー
		case CONNECTUI_BOTH_LEVEL_NORMAL:		return CTexture::TEX_CONNECT_SELECT_NORMAL; break;		// 難易度 ノーマル
		case CONNECTUI_BOTH_LEVEL_HARD:			return CTexture::TEX_CONNECT_SELECT_HARD; break;		// 難易度 ハード
		}
	}

	return 0;
}

//=============================================================================
// UIのテクスチャ番号取得
//=============================================================================
int CConnectUI::GetTexNumberOnly(CONNECTUITYPE_ONLY type)
{
	switch (type)
	{
	case CONNECTUI_ONLY_BUTTON_DESIDE:	return CTexture::TEX_CONNECT_DECIDE; break;				// 決定ボタン
	case CONNECTUI_ONLY_COMMENT_REMOVE:	return CTexture::TEX_CONNECT_COMMENT_REMOVE; break;		// コメント 解除
	case CONNECTUI_ONLY_COMMENT_SOLVE:	return CTexture::TEX_CONNECT_COMMENT_SOLVE; break;		// コメント 解読
	case CONNECTUI_ONLY_COMMENT_EASY:	return CTexture::TEX_CONNECT_COMMENT_EASY; break;		// コメント イージー
	case CONNECTUI_ONLY_COMMENT_NORMAL:	return CTexture::TEX_CONNECT_COMMENT_NORMAL; break;		// コメント ノーマル
	case CONNECTUI_ONLY_COMMENT_HARD:	return CTexture::TEX_CONNECT_COMMENT_HARD; break;		// コメント ハード
	case CONNECTUI_ONLY_CAUTION_MODE:	return CTexture::TEX_CONNECT_CAUTION_MODE; break;		// モード選択の注意
	case CONNECTUI_ONLY_CAUTION_LEVEL:	return CTexture::TEX_CONNECT_CAUTION_LEVEL; break;		// レベル選択の注意
	}
	return 0;
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

	if (type == CONNECTUI_BOTH_MODE_REMOVE ||
		type == CONNECTUI_BOTH_MODE_SOLVE ||
		type == CONNECTUI_BOTH_LEVEL_EASY ||
		type == CONNECTUI_BOTH_LEVEL_NORMAL ||
		type == CONNECTUI_BOTH_LEVEL_HARD)
	{
		m_UIInfoBoth[nPlayer][type].type = UITYPE_BUTTON;
	}
	else if (type == CONNECTUI_BOTH_LOADICON)
	{
		m_UIInfoBoth[nPlayer][type].type = UITYPE_LOADICON;
	}

	// ボタンはUV値を変更
	if (m_UIInfoBoth[nPlayer][type].type == UITYPE_BUTTON)
		pPolygon->SetAnim(ZeroVector2, D3DXVECTOR2(1.0f, 1.0f / 3));

	// ロードアイコンはUV値を変更
	if (m_UIInfoBoth[nPlayer][type].type == UITYPE_LOADICON)
		pPolygon->SetAnim(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f / NUM_ANIMPATTER_LOADICON, 1.0f));

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

	if (type == CONNECTUI_ONLY_BUTTON_DESIDE)
		m_UIInfoOnly[type].type = UITYPE_BUTTON;

	if (m_UIInfoOnly[type].type == UITYPE_BUTTON)
		pPolygon->SetAnim(ZeroVector2, D3DXVECTOR2(1.0f, 1.0f / 3));

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
#ifdef _DEBUG
	DebugCommand();
#endif

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
		m_bDeleteFlagBoth[nPlayer][CONNECTUI_BOTH_LOADICON] = true;
		m_bDeleteFlagBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTING] = true;

		// 接続完了UIを生成
		m_bCreateFlagBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTED] = true;
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
#ifdef _DEBUG
	DebugCommand();
#endif
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

	// マウスのポインタ取得
	CMouse *pMouse = CManager::GetMouse();
	// マウス座標を取得
	D3DXVECTOR2 mousePos = pMouse->GetMousePos();

	if (m_nSelectMode[PLAYER_ONE] != SELECTMODE_NONE)
	{
		m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE - 1 + m_nSelectMode[PLAYER_ONE]].buttonState = BUTTON_SELECT;
	}

	// カーソル位置がUIと重なっている
	if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE]->ReturnHit(mousePos))
	{
		// 段々表示
		D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_REMOVE]->GetCol();
		if (col.a < 1.0f)
			m_pUIOnly[CONNECTUI_ONLY_COMMENT_REMOVE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a + (1.0f / TIME_ALPHA_COMMENT)));

		// クリックしてなければ、処理しない
		if (pMouse->GetTrigger(0))
		{
			m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE].buttonState = BUTTON_PRESS;
			m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE].buttonState = BUTTON_NORMAL;

			//音再生
			CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);

			if (m_nSelectMode[PLAYER_ONE] != SELECTMODE_REMOVE)
			{
				m_nSelectMode[PLAYER_ONE] = SELECTMODE_REMOVE;
				CClient::SendSelect(m_nSelectMode[PLAYER_ONE]);
			}
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
		if (pMouse->GetTrigger(0))
		{
			m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE].buttonState = BUTTON_PRESS;
			m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE].buttonState = BUTTON_NORMAL;
			//音再生
			CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);

			if (m_nSelectMode[PLAYER_ONE] != SELECTMODE_SOLVE)
			{
				m_nSelectMode[PLAYER_ONE] = SELECTMODE_SOLVE;
				CClient::SendSelect(m_nSelectMode[PLAYER_ONE]);
			}
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
#ifdef _DEBUG
	DebugCommand();
#endif

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
		if (m_nSelectLevel[PLAYER_ONE] != SELECTLEVEL_NONE)
		{
			m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY - 1 + m_nSelectLevel[PLAYER_ONE]].buttonState = BUTTON_SELECT;
		}

		// カーソル位置がUIと重なっている
		if (m_pUIBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY]->ReturnHit(mousePos))
		{
			// 段々表示
			D3DXCOLOR col = m_pUIOnly[CONNECTUI_ONLY_COMMENT_EASY]->GetCol();
			if (col.a < 1.0f)
				m_pUIOnly[CONNECTUI_ONLY_COMMENT_EASY]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, col.a + (1.0f / TIME_ALPHA_COMMENT)));

			// クリックしてなければ、処理しない
			if (pMouse->GetTrigger(0))
			{
				m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY].buttonState = BUTTON_PRESS;
				m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL].buttonState = BUTTON_NORMAL;
				m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD].buttonState = BUTTON_NORMAL;

				//音再生
				CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);

				if (m_nSelectLevel[PLAYER_ONE] != SELECTLEVEL_EASY)
				{
					m_nSelectLevel[PLAYER_ONE] = SELECTLEVEL_EASY;
					CClient::SendSelect(m_nSelectLevel[PLAYER_ONE]);
				}
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
			if (pMouse->GetTrigger(0))
			{
				m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL].buttonState = BUTTON_PRESS;
				m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY].buttonState = BUTTON_NORMAL;
				m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD].buttonState = BUTTON_NORMAL;

				//音再生
				CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);

				if (m_nSelectLevel[PLAYER_ONE] != SELECTLEVEL_NORMAL)
				{
					m_nSelectLevel[PLAYER_ONE] = SELECTLEVEL_NORMAL;
					CClient::SendSelect(m_nSelectLevel[PLAYER_ONE]);
				}
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
			if (pMouse->GetTrigger(0))
			{
				m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD].buttonState = BUTTON_PRESS;
				m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY].buttonState = BUTTON_NORMAL;
				m_UIInfoBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL].buttonState = BUTTON_NORMAL;

				//音再生
				CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);

				if (m_nSelectLevel[PLAYER_ONE] != SELECTLEVEL_HARD)
				{
					m_nSelectLevel[PLAYER_ONE] = SELECTLEVEL_HARD;
					CClient::SendSelect(m_nSelectLevel[PLAYER_ONE]);
				}
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
				m_bCreateFlagOnly[CONNECTUI_ONLY_BUTTON_DESIDE] = true;

				// 注意書きを破棄
				if (m_pUIOnly[CONNECTUI_ONLY_CAUTION_LEVEL])
					DeleteOnlyUI(CONNECTUI_ONLY_CAUTION_LEVEL);
			}
		}
	}
	else
	{
		if (!m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
		{
			// 決定ボタンを生成
			if (m_nSelectLevel[PLAYER_TWO] != SELECTLEVEL_NONE)
			{
				m_bCreateFlagOnly[CONNECTUI_ONLY_BUTTON_DESIDE] = true;

				// 決定ボタンを破棄
				if (m_pUIOnly[CONNECTUI_ONLY_CAUTION_LEVEL])
					DeleteOnlyUI(CONNECTUI_ONLY_CAUTION_LEVEL);
			}
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
		m_bCreateFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING] = true;
	}

	// 解除
	if (mode == SELECTMODE_REMOVE)
	{
		m_bCreateFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_REMOVE] = true;
		m_UIInfoBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_REMOVE].buttonState = BUTTON_SELECT;

		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_SOLVE] = true;
	}
	// 解読
	if (mode == SELECTMODE_SOLVE)
	{
		m_bCreateFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_SOLVE] = true;
		m_UIInfoBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_SOLVE].buttonState = BUTTON_SELECT;

		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_REMOVE] = true;
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
		m_bCreateFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING] = true;
	}

	// イージー
	if (level == SELECTLEVEL_EASY)
	{
		m_bCreateFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY] = true;
		m_UIInfoBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY].buttonState = BUTTON_SELECT;

		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD] = true;
	}
	// 解読
	if (level == SELECTLEVEL_NORMAL)
	{
		m_bCreateFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL] = true;
		m_UIInfoBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL].buttonState = BUTTON_SELECT;

		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD] = true;
	}
	if (level == SELECTLEVEL_HARD)
	{
		m_bCreateFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD] = true;
		m_UIInfoBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD].buttonState = BUTTON_SELECT;

		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY] = true;
	}
}

//=============================================================================
// ゲストの選択を処理
//=============================================================================
void CConnectUI::RecvGuestSelect(int nSelect)
{
	printf("ゲストから選択を受信\n");
	// モード別で選択
	if (m_flow == CONNECTFLOW_SELECT_MODE)
		SetGuestMode((SELECTMODE)nSelect);
	if (m_flow == CONNECTFLOW_SELECT_LEVEL)
		SetGuestLevel((SELECTLEVEL)nSelect);
}

//=============================================================================
// ゲストの待ち状態を処理
//=============================================================================
void CConnectUI::RecvGuestWait(void)
{
	printf("ゲストから待ち状態を受信\n");
	// 待ち状態をon
	m_bGuestWait = true;
}

//=============================================================================
// フローごとの開始時の処理
//=============================================================================
void CConnectUI::Begin(CONNECTFLOW_TYPE flow)
{
	m_bGuestWait = false;

	switch (flow)
	{
		// 接続中
	case CONNECTFLOW_CONNECTING:
		// 生成
		for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
		{
			m_bCreateFlagBoth[nPlayer][CONNECTUI_BOTH_BACK] = true;
			m_bCreateFlagBoth[nPlayer][CONNECTUI_BOTH_LOADICON] = true;
			m_bCreateFlagBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTING] = true;
		}
		break;

		// モード選択
	case CONNECTFLOW_SELECT_MODE:
		m_bCreateFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_MODE] = true;
		m_bCreateFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE] = true;
		m_bCreateFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE] = true;
		m_bCreateFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING] = true;
		m_bCreateFlagOnly[CONNECTUI_ONLY_CAUTION_MODE] = true;
		m_bCreateFlagOnly[CONNECTUI_ONLY_COMMENT_REMOVE] = true;
		m_bCreateFlagOnly[CONNECTUI_ONLY_COMMENT_SOLVE] = true;
		break;

		// レベル選択時
	case CONNECTFLOW_SELECT_LEVEL:
		m_UIInfoOnly[CONNECTUI_ONLY_BUTTON_DESIDE].buttonState = BUTTON_NORMAL;
		// プレイヤーのUI
		if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
		{
			m_bCreateFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_LEVEL] = true;
			m_bCreateFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY] = true;
			m_bCreateFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL] = true;
			m_bCreateFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD] = true;
		}
		// ゲストのUI
		else if (m_nSelectMode[PLAYER_TWO] == SELECTMODE_REMOVE)
		{
			m_bCreateFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING] = true;
		}

		m_bCreateFlagOnly[CONNECTUI_ONLY_CAUTION_LEVEL] = true;
		m_bCreateFlagOnly[CONNECTUI_ONLY_COMMENT_EASY] = true;
		m_bCreateFlagOnly[CONNECTUI_ONLY_COMMENT_NORMAL] = true;
		m_bCreateFlagOnly[CONNECTUI_ONLY_COMMENT_HARD] = true;
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
		m_bDeleteFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_STATE_CONNECTED] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_CONNECTED] = true;
		// カウンタをリセット
		m_nCntState = 0;
		// 次のフローへ
		m_flow = CONNECTFLOW_SELECT_MODE;
		break;

	case CONNECTFLOW_SELECT_MODE:
		m_bDeleteFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_MODE] = true;
		m_bDeleteFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_REMOVE] = true;
		m_bDeleteFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_MODE_SOLVE] = true;

		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTING] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_REMOVE] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_MODE_SOLVE] = true;
		
		m_bDeleteFlagOnly[CONNECTUI_ONLY_BUTTON_DESIDE] = true;
		m_bDeleteFlagOnly[CONNECTUI_ONLY_CAUTION_MODE] = true;
		m_bDeleteFlagOnly[CONNECTUI_ONLY_COMMENT_REMOVE] = true;
		m_bDeleteFlagOnly[CONNECTUI_ONLY_COMMENT_SOLVE] = true;
		// 次のフローへ
		m_flow = CONNECTFLOW_SELECT_LEVEL;
		break;
	case CONNECTFLOW_SELECT_LEVEL:
		m_bDeleteFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_SELECT_LEVEL] = true;
		m_bDeleteFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_EASY] = true;
		m_bDeleteFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_NORMAL] = true;
		m_bDeleteFlagBoth[PLAYER_ONE][CONNECTUI_BOTH_LEVEL_HARD] = true;

		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_EASY] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_NORMAL] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_LEVEL_HARD] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED] = true;
		m_bDeleteFlagBoth[PLAYER_TWO][CONNECTUI_BOTH_STATE_SELECTED] = true;

		m_bDeleteFlagOnly[CONNECTUI_ONLY_CAUTION_LEVEL] = true;
		m_bDeleteFlagOnly[CONNECTUI_ONLY_COMMENT_EASY] = true;
		m_bDeleteFlagOnly[CONNECTUI_ONLY_COMMENT_NORMAL] = true;
		m_bDeleteFlagOnly[CONNECTUI_ONLY_COMMENT_HARD] = true;
		m_bDeleteFlagOnly[CONNECTUI_ONLY_BUTTON_DESIDE] = true;

		m_flow = CONNECTFLOW_END;
		// 難易度を設定
		m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE ?
			CGame::SetDifficulty((CBomb::DIFFICULTY)m_nSelectLevel[PLAYER_ONE]) :
			CGame::SetDifficulty((CBomb::DIFFICULTY)m_nSelectLevel[PLAYER_TWO]);
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
		// 両者とも待ち状態だが、選択が重複した
		if ((m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE && m_nSelectMode[PLAYER_TWO] == SELECTMODE_REMOVE) ||
			(m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE && m_nSelectMode[PLAYER_TWO] == SELECTMODE_SOLVE))
		{
			// 選びなおし
			m_bGuestWait = false;
			m_state = FLOWSTATE_NORMAL;
		}
		// 両者とも待ち状態で、選択も重複しない
		if (m_bGuestWait &&
			((m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE && m_nSelectMode[PLAYER_TWO] == SELECTMODE_SOLVE) ||
			(m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE && m_nSelectMode[PLAYER_TWO] == SELECTMODE_REMOVE)))
		{
			// 次へ
			m_state = FLOWSTATE_END;
		}
		break;
	case CONNECTFLOW_SELECT_LEVEL:
		// 解除側
		if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_REMOVE)
		{
			// ゲストの承認を得られれば、ゲーム開始
			if (m_bGuestWait)
				m_state = FLOWSTATE_END;
		}
		// 解読側
		else if (m_nSelectMode[PLAYER_ONE] == SELECTMODE_SOLVE)
		{
			// ゲストがモードを選んでいれば、ゲーム開始
			if (m_bGuestWait && m_nSelectLevel[PLAYER_TWO] != SELECTLEVEL_NONE)
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
			m_bCreateFlagOnly[CONNECTUI_ONLY_BUTTON_DESIDE] = true;
		}

		// 決定ボタンを破棄
		if (m_pUIOnly[CONNECTUI_ONLY_CAUTION_MODE])
			m_bDeleteFlagOnly[CONNECTUI_ONLY_CAUTION_MODE] = true;
		return;
	}

	// 決定ボタンを破棄
	if (m_pUIOnly[CONNECTUI_ONLY_BUTTON_DESIDE])
		m_bDeleteFlagOnly[CONNECTUI_ONLY_BUTTON_DESIDE] = true;

	// 注意書きを生成
	if (!m_pUIOnly[CONNECTUI_ONLY_CAUTION_MODE])
		m_bCreateFlagOnly[CONNECTUI_ONLY_CAUTION_MODE] = true;
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
		//選択音
		CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);

		// ゲストを待つ状態へ
		m_UIInfoOnly[CONNECTUI_ONLY_BUTTON_DESIDE].buttonState = BUTTON_PRESS;
		// サーバーへ待ち状態を送信
		if (m_state != FLOWSTATE_WAIT)
		{
			CClient::SendWait();
		}
		// 待ち状態へ
		m_state = FLOWSTATE_WAIT;
	}
}
