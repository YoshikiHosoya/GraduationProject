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

//=============================================================================
// マクロ定義
//=============================================================================
#define INTERVAL_ANIM_LOADICON	(10)	// アニメーション処理の間隔
#define NUM_ANIMPATTER_LOADICON	(8)		// ロードアイコンのパターン数

#define TIME_FLOW_INTERVAL		(30)	// フロー切替時のインターバル

#define FILENAME_UIINFO			("data/TEXT/ConnectUIInfo.txt")	// UI情報のファイル

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
CPolygon2D *CConnectUI::m_pUIBoth[2][CONNECTUI_BOTH_MAX] = {};
CPolygon2D *CConnectUI::m_pUIOnly[CONNECTUI_ONLY_MAX] = {};

CConnectUI::CONNECTUI_INFO CConnectUI::m_UIInfoBoth[2][CONNECTUI_BOTH_MAX] = {};
CConnectUI::CONNECTUI_INFO CConnectUI::m_UIInfoOnly[CONNECTUI_ONLY_MAX] = {};

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

	// 生成
	for (int nPlayer = 0; nPlayer < 2; nPlayer++)
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
	m_pUIBoth[0][CONNECTUI_BOTH_BACK]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_BACK_00));
	m_pUIBoth[1][CONNECTUI_BOTH_BACK]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_BACK_01));
	m_pUIBoth[0][CONNECTUI_BOTH_LOADICON]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_LOAD));
	m_pUIBoth[1][CONNECTUI_BOTH_LOADICON]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_LOAD));
	m_pUIBoth[0][CONNECTUI_BOTH_STATE_CONNECTING]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_CONNECTING));
	m_pUIBoth[1][CONNECTUI_BOTH_STATE_CONNECTING]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_CONNECTING));

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
		break;
	}

	for (int nPlayer = 0; nPlayer < 2; nPlayer++)
	{
		for (int nUI = 0; nUI < CONNECTUI_BOTH_MAX; nUI++)
		{
			// 存在しないなら、次へ
			if (!m_pUIBoth[nPlayer][nUI])
				continue;

			// ロードアイコンのアニメーション
			if (nUI == CONNECTUI_BOTH_LOADICON)
				m_pUIBoth[nPlayer][nUI]->SetAnim(D3DXVECTOR2(0.0f + (1.0f / NUM_ANIMPATTER_LOADICON * m_nCntPattern), 0.0f), D3DXVECTOR2(1.0f / NUM_ANIMPATTER_LOADICON, 1.0f));

			// 更新
			m_pUIBoth[nPlayer][nUI]->Update();
		}
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

	// 値を返す
	return pPolygon;
}

//=============================================================================
// 接続中の処理
//=============================================================================
void CConnectUI::Connecting(void)
{
	// （仮） 接続完了
	if (CManager::GetKeyboard()->GetTrigger(DIK_1))
		ConnectPlayer(0);
	if (CManager::GetKeyboard()->GetTrigger(DIK_2))
		ConnectPlayer(1);

	// ロードアイコンのアニメーション
	Anim();
}

//=============================================================================
// プレイヤーが接続したときの処理
//=============================================================================
void CConnectUI::ConnectPlayer(int nPlayer)
{
	// 再度処理しないよう制限する
	if (m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTED])
		return;

	// 既存のUIの破棄
	if (m_pUIBoth[nPlayer][CONNECTUI_BOTH_LOADICON])
	{
		delete m_pUIBoth[nPlayer][CONNECTUI_BOTH_LOADICON];
		m_pUIBoth[nPlayer][CONNECTUI_BOTH_LOADICON] = nullptr;
	}
	if (m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTING])
	{
		delete m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTING];
		m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTING] = nullptr;
	}

	// 接続完了を生成
	m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTED] = CreateBothUI(nPlayer, CONNECTUI_BOTH_STATE_CONNECTED);
	m_pUIBoth[nPlayer][CONNECTUI_BOTH_STATE_CONNECTED]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_CONNECTED));

	// 両者とも接続出来たら、フローを次へ
	if (nPlayer == 0 && m_pUIBoth[1][CONNECTUI_BOTH_STATE_CONNECTED])
		m_flow = CONNECTFLOW_CONNECTED;
	else if (nPlayer == 1 && m_pUIBoth[0][CONNECTUI_BOTH_STATE_CONNECTED])
		m_flow = CONNECTFLOW_CONNECTED;
}

//=============================================================================
// 接続完了の処理
//=============================================================================
void CConnectUI::Connected(void)
{
	// 状態カウンタを加算
	m_nCntState++;

	// 時間でなければ、処理を終える
	if (m_nCntState < TIME_FLOW_INTERVAL)
		return;

	// 接続完了のUIを破棄
	if (m_pUIBoth[0][CONNECTUI_BOTH_STATE_CONNECTED])
	{
		delete m_pUIBoth[0][CONNECTUI_BOTH_STATE_CONNECTED];
		m_pUIBoth[0][CONNECTUI_BOTH_STATE_CONNECTED] = nullptr;
	}
	if (m_pUIBoth[1][CONNECTUI_BOTH_STATE_CONNECTED])
	{
		delete m_pUIBoth[1][CONNECTUI_BOTH_STATE_CONNECTED];
		m_pUIBoth[1][CONNECTUI_BOTH_STATE_CONNECTED] = nullptr;
	}

	// 次のフローへ
	m_flow = CONNECTFLOW_SELECT_MODE;
	m_nCntState = 0;
}

//=============================================================================
// モードの選択処理
//=============================================================================
void CConnectUI::SelectMode(void)
{
	// UIの生成
	if (!m_pUIBoth[0][CONNECTUI_BOTH_SELECT_MODE])
	{
		m_pUIBoth[0][CONNECTUI_BOTH_SELECT_MODE] = CreateBothUI(0, CONNECTUI_BOTH_SELECT_MODE);
		m_pUIBoth[0][CONNECTUI_BOTH_SELECT_MODE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_MODE));
	}
	if (!m_pUIBoth[0][CONNECTUI_BOTH_MODE_REMOVE])
	{
		m_pUIBoth[0][CONNECTUI_BOTH_MODE_REMOVE] = CreateBothUI(0, CONNECTUI_BOTH_MODE_REMOVE);
		m_pUIBoth[0][CONNECTUI_BOTH_MODE_REMOVE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SELECT_REMOVE));
	}
	if (!m_pUIBoth[0][CONNECTUI_BOTH_MODE_SOLVE])
	{
		m_pUIBoth[0][CONNECTUI_BOTH_MODE_SOLVE] = CreateBothUI(0, CONNECTUI_BOTH_MODE_SOLVE);
		m_pUIBoth[0][CONNECTUI_BOTH_MODE_SOLVE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_SLECT_SOLVE));
	}

	// ゲストのUI
	if (!m_pUIBoth[1][CONNECTUI_BOTH_STATE_SELECTING])
	{
		m_pUIBoth[1][CONNECTUI_BOTH_STATE_SELECTING] = CreateBothUI(1, CONNECTUI_BOTH_STATE_SELECTING);
		m_pUIBoth[1][CONNECTUI_BOTH_STATE_SELECTING]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_GUEST_SELECTING));
	}
}

//=============================================================================
// アニメーション
//=============================================================================
void CConnectUI::Anim(void)
{
	if (m_pUIBoth[0][CONNECTUI_BOTH_LOADICON])
		UpdateCntAnim();
	else if (m_pUIBoth[1][CONNECTUI_BOTH_LOADICON])
		UpdateCntAnim();
}

//=============================================================================
// アニメーションカウンタの更新
//=============================================================================
void CConnectUI::UpdateCntAnim(void)
{
	// 時間でないなら、処理を終える
	m_nCntAnim++;
	if (m_nCntAnim < INTERVAL_ANIM_LOADICON)
		return;

	// 一定フレームおきにアニメーション
	m_nCntPattern++;
	if (m_nCntPattern >= NUM_ANIMPATTER_LOADICON)
		m_nCntPattern = 0;
	m_nCntAnim = 0;
}
