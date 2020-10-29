//=============================================================================
//
// チャットのタブ処理 [ chatTab.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "chatTab.h"
#include "manager.h"
#include "renderer.h"
#include "mouse.h"
#include "keyboard.h"
#include "texture.h"
#include "polygon2D.h"
#include "chatText.h"
#include "ImGui/imgui.h"			// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"	// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"	// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================
#define POS_OPENTAB		(D3DXVECTOR2(980.0f, 720.0f))	// 開いたタブの座標
#define POS_CLOSETAB	(D3DXVECTOR2(1280.0f, 720.0f))	// 閉じたタブの座標
#define DIFPOS_X_TEXTBOX (10.0f)
#define DIFPOS_Y_TEXTBOX (110.0f)

#define SIZE_TABBACK	(D3DXVECTOR3(300.0f, 720.0f, 0.0f))	// タブ背景のサイズ
#define SIZE_TABBUTTON	(D3DXVECTOR3(40.0f, 100.0f, 0.0f))	// タブ開閉ボタンのサイズ
#define SIZE_X_TEXTBOX	(280.0f)

#define TIME_TABMOVE	(10)	// タブ開閉の時間

#define COLOR_TABCLICK	(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))	// タブクリック時のカラー

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
D3DXVECTOR2			CChatTab::m_TabPos		= ZeroVector2;
CChatTab::TABSTATE	CChatTab::m_tabState	= CChatTab::TABSTATE_CLOSED;
CChatText			* CChatTab::m_pChatText = nullptr;
std::vector<CPolygon2D*> CChatTab::m_pBoxBack = {};	// テキストの背景ポリゴン

//=============================================================================
// コンストラクタ
//=============================================================================
CChatTab::CChatTab()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CChatTab::~CChatTab()
{
	if (m_pPolyBack)
	{
		delete m_pPolyBack;
		m_pPolyBack = nullptr;
	}

	if (m_pPolyTab)
	{
		delete m_pPolyTab;
		m_pPolyTab = nullptr;
	}

	for (int nCnt = 0; nCnt < (int)m_pBoxBack.size(); nCnt++)
	{
		if (m_pBoxBack[nCnt])
		{
			delete m_pBoxBack[nCnt];
			m_pBoxBack[nCnt] = nullptr;
		}
	}

	// テキストの終了
	if (m_pChatText)
	{
		delete m_pChatText;
		m_pChatText = nullptr;
	}
}

//=============================================================================
// テキストボックスの追加
//=============================================================================
void CChatTab::AddTextBox(char * cText)
{
	// テキストボックスの背景を生成
	m_pBoxBack.push_back(CPolygon2D::Create());

	// 番号を取得
	int nNumBox = (int)m_pBoxBack.size() - 1;
	// 初期設定
	m_pBoxBack[nNumBox]->SetPos(D3DXVECTOR3(m_TabPos.x + DIFPOS_X_TEXTBOX, DIFPOS_Y_TEXTBOX, 0.0f));
	m_pBoxBack[nNumBox]->SetSize(D3DXVECTOR3(SIZE_X_TEXTBOX, 150.0f, 0.0f));
	m_pBoxBack[nNumBox]->SetPosStart(CPolygon2D::POSSTART_TOP_LEFT);
	m_pBoxBack[nNumBox]->SetCol(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
}

//=============================================================================
// チャットタブのクリック
//=============================================================================
void CChatTab::ClickTab(void)
{
	// スライドしている
	if (m_nCntState > 0)
		return;

	// 状態を変更
	switch (m_tabState)
	{
	case TABSTATE_OPENED:
		m_tabState = TABSTATE_CLOSING;
		break;
	case TABSTATE_CLOSED:
		m_tabState = TABSTATE_OPENNING;
		break;
	}
	// カラー更新
	m_pPolyTab->SetCol(COLOR_TABCLICK);
}

//=============================================================================
// チャットタブのスライド
//=============================================================================
void CChatTab::SlideTab(void)
{
	// 最初だけ通る
	if (m_nCntState == 0)
	{
		// タブの移動量を計算
		switch (m_tabState)
		{
		case TABSTATE_OPENNING:
			m_moveDest = (POS_OPENTAB - m_TabPos) / TIME_TABMOVE;
			break;
		case TABSTATE_CLOSING:
			m_moveDest = (POS_CLOSETAB - m_TabPos) / TIME_TABMOVE;
			break;
		}
	}

	// カラー更新
	if (m_nCntState == TIME_TABMOVE - 1)
		m_pPolyTab->SetCol(WhiteColor);

	// カウンタを加算
	m_nCntState++;
	// 時間でスライド終了
	if (m_nCntState > TIME_TABMOVE)
	{
		// カウンタをリセット
		m_nCntState = 0;
		// 状態を変更
		switch (m_tabState)
		{
		case TABSTATE_OPENNING:
			m_tabState = TABSTATE_OPENED;
			m_pPolyTab->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABCLOSE));
			break;
		case TABSTATE_CLOSING:
			m_tabState = TABSTATE_CLOSED;
			m_pPolyTab->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABOPEN));
			break;
		}
		// 処理の終了
		return;
	}

	// 移動値を加算し、座標更新
	m_TabPos += m_moveDest;
	m_pPolyBack->SetPos(D3DXVECTOR3(m_TabPos.x, m_TabPos.y, 0.0f));
	m_pPolyTab->SetPos(D3DXVECTOR3(m_TabPos.x, m_TabPos.y, 0.0f));
}

//=============================================================================
// 生成
//=============================================================================
CChatTab * CChatTab::Create(void)
{
	// メモリ確保
	CChatTab *pTab = new CChatTab();

	// 確保失敗
	if (!pTab)
		return nullptr;

	// 初期化
	pTab->Init();
	// 値を返す
	return pTab;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CChatTab::Init(void)
{
	// 要素の初期化
	m_TabPos = POS_CLOSETAB;
	m_moveDest = ZeroVector2;
	m_tabState = TABSTATE_CLOSED;
	m_nCntState = 0;

	// 背景の生成
	m_pPolyBack = CPolygon2D::Create();
	m_pPolyBack->SetPos(D3DXVECTOR3(m_TabPos.x, m_TabPos.y, 0.0f));
	m_pPolyBack->SetSize(SIZE_TABBACK);
	m_pPolyBack->SetPosStart(CPolygon2D::POSSTART_BOTTOM_LEFT);
	m_pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BACK));

	// 開閉タブの生成
	m_pPolyTab = CPolygon2D::Create();
	m_pPolyTab->SetPos(D3DXVECTOR3(m_TabPos.x, m_TabPos.y, 0.0f));
	m_pPolyTab->SetSize(SIZE_TABBUTTON);
	m_pPolyTab->SetPosStart(CPolygon2D::POSSTART_BOTTOM_RIGHT);
	m_pPolyTab->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABOPEN));

	// チャットテキストの生成
	m_pChatText = new CChatText;
	m_pChatText->Init();

	return S_OK;
}


//=============================================================================
// 更新
//=============================================================================
void CChatTab::Update(void)
{
	CMouse *pMouse = CManager::GetMouse();	
	CKeyboard *pKey = CManager::GetKeyboard();
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)pMouse->GetMouseX(), (float)pMouse->GetMouseY());

	// タブのスライド
	if (m_tabState == TABSTATE_CLOSING || m_tabState == TABSTATE_OPENNING)
		SlideTab();

	// 左クリックかF5でタブの開閉処理
	if ((pMouse->GetTrigger(0) && m_pPolyTab->ReturnHit(mousePos)) ||
		pKey->GetTrigger(DIK_F5))
		ClickTab();

	if (m_pPolyBack)
		m_pPolyBack->Update();

	if (m_pPolyTab)
		m_pPolyTab->Update();
	
	for (int nCnt = 0; nCnt < (int)m_pBoxBack.size(); nCnt++)
	{
		if (m_pBoxBack[nCnt])
			m_pBoxBack[nCnt]->Update();
	}

	// タブが開いているときのみ、文字を入力できる
	if (m_tabState == TABSTATE_OPENED)
		CChatText::InputText();
}

//=============================================================================
// 描画
//=============================================================================
void CChatTab::Draw(void)
{
	if (m_pPolyBack)
		m_pPolyBack->Draw();

	if (m_pPolyTab)
		m_pPolyTab->Draw();

	for (int nCnt = 0; nCnt < (int)m_pBoxBack.size(); nCnt++)
	{
		if (m_pBoxBack[nCnt])
			m_pBoxBack[nCnt]->Draw();
	}

	CChatText::Draw();
}
