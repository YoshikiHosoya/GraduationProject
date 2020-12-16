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
#include "client.h"
#include "ImGui/imgui.h"			// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"	// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"	// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_KEEPTEXT			(20)	// 保持できるテキストの最大数

#define POS_OPENTAB				(D3DXVECTOR2(980.0f, 720.0f))	// 開いたタブの座標
#define POS_CLOSETAB			(D3DXVECTOR2(1280.0f, 720.0f))	// 閉じたタブの座標

#define SIZE_TABBACK			(D3DXVECTOR2(300.0f, 720.0f))	// タブ背景のサイズ
#define SIZE_TABTITLE			(D3DXVECTOR2(300.0f, 100.0f))	// タブタイトルのサイズ
#define SIZE_TABWRITEWINDOW		(D3DXVECTOR2(300.0f, 100.0f))	// タブ書き込みウィンドウのサイズ
#define SIZE_TABBUTTON			(D3DXVECTOR2(40.0f, 100.0f))	// タブ開閉ボタンのサイズ
#define SIZE_X_TEXTBOX			(280.0f)	// テキストボックスのサイズ
#define SIZE_Y_TEXTBOX			(100.0f)	// テキストボックスのサイズ
#define SIZE_X_TEXTBOX_PIC		(280.0f)	// テキストボックスのサイズ
#define SIZE_Y_TEXTBOX_PIC		(240.0f)	// テキストボックスのサイズ
#define SIZE_X_PICTURE			(260.0f)	// ピクチャのサイズ
#define SIZE_Y_PICTURE			(190.0f)	// ピクチャのサイズ

#define DIFPOS_X_TEXTBOX		(10.0f)		// テキストボックスの座標の差分
#define DIFPOS_Y_TEXTBOX		(100.0f)	// テキストボックスの座標の差分
#define DIFPOS_X_PICBOX			(10.0f)		// ピクチャボックスの座標の差分
#define DIFPOS_Y_PICBOX			(250.0f)	// ピクチャボックスの座標の差分
#define DIFPOS_X_PICTURE		(140.0f)	// ピクチャの座標の差分
#define DIFPOS_Y_PICTURE		(138.0f)	// ピクチャの座標の差分
#define DIFPOS_Y_TEXTBOX_UNDER	(85.0f)		// テキストボックスの座標の差分
#define DIFPOS_X_KEEPTEXT		(5.0f)		// 保持するテキストの座標の差分
#define DIFPOS_Y_KEEPTEXT		(40.0f)		// 保持するテキストの座標の差分
#define DIFPOS_X_SENDTEXT		(15.0f)		// 送信用テキストの座標の差分
#define DIFPOS_Y_SENDTEXT		(63.0f)		// 送信用テキストの座標の差分
#define DIFPOS_X_MAXCHAR		(190.0f)	// 文字数表示用テキストの座標の差分
#define DIFPOS_Y_MAXCHAR		(23.0f)		// 文字数表示用テキストの座標の差分

#define TIME_TABMOVE			(10)		// タブ開閉の時間
#define TIME_PRESSKEY			(40)		// キーの長押し時間

#define COLOR_TABCLICK			(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))	// タブクリック時のカラー

#define MOVE_SCROLL_TAB			(30.0f)	// マウススクロールによる座標移動

#define LINK_SENDPICTURE		("data/SAVEDATA/PictureTextures/PicTex.txt")	// 送信用ピクチャのパス

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
D3DXVECTOR2			CChatTab::m_TabPos		= ZeroVector2;
float				CChatTab::m_fScrollPosY = 0.0f;
CChatTab::TABSTATE	CChatTab::m_tabState	= CChatTab::TABSTATE_CLOSED;
int					CChatTab::m_nCntPress = 0;
int					CChatTab::m_nPressKey = 0;
CChatText			*CChatTab::m_SendText = nullptr;
CChatText			*CChatTab::m_leftText = nullptr;
std::vector<CChatTab::CHATKEEP>	CChatTab::m_chatKeep = {};	// 保持できるテキスト
CChatTab::TABSTATE	CChatTab::m_tabletState = CChatTab::TABSTATE_CLOSED;
bool				CChatTab::m_bClickTab = false;

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
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_pChatPoly[nCnt])
		{
			delete m_pChatPoly[nCnt];
			m_pChatPoly[nCnt] = nullptr;
		}
	}

	if (m_SendText)
	{
		m_SendText->Uninit();
		delete m_SendText;
		m_SendText = nullptr;
	}

	if (m_leftText)
	{
		m_leftText->Uninit();
		delete m_leftText;
		m_leftText = nullptr;
	}

	for (int nCnt = 0; nCnt < (int)m_chatKeep.size(); nCnt++)
	{
		if (m_chatKeep[nCnt].pPolyBack)
		{
			delete m_chatKeep[nCnt].pPolyBack;
			m_chatKeep[nCnt].pPolyBack = nullptr;
		}
		if (m_chatKeep[nCnt].pPolyPic)
		{
			delete m_chatKeep[nCnt].pPolyPic;
			m_chatKeep[nCnt].pPolyPic = nullptr;
		}
		if (m_chatKeep[nCnt].pKeepText)
		{
			m_chatKeep[nCnt].pKeepText->Uninit();
			delete m_chatKeep[nCnt].pKeepText;
			m_chatKeep[nCnt].pKeepText = nullptr;
		}
	}
	m_chatKeep.clear();
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
	m_pChatPoly[POLY_TAB]->SetCol(COLOR_TABCLICK);
}

//=============================================================================
// タブレット開閉タブのクリック
//=============================================================================
void CChatTab::ClickTabletTab(void)
{
	if (CManager::GetMouse()->GetTrigger(0))
	{
		// カラー更新
		m_pChatPoly[POLY_TABLET]->SetCol(COLOR_TABCLICK);

		m_tabletState == TABSTATE_OPENED ?
			m_tabState = TABSTATE_CLOSED :
			m_tabState = TABSTATE_OPENED;

		// フラグon
		m_bClickTab = true;
	}
	else if (!CManager::GetMouse()->GetTrigger(0))
	{
		// カラー更新
		if (m_pChatPoly[POLY_TABLET]->GetCol() != WhiteColor)
			m_pChatPoly[POLY_TABLET]->SetCol(WhiteColor);
		//	フラグ解除
		if (m_bClickTab)
			m_bClickTab = false;
	}
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
		m_pChatPoly[POLY_TAB]->SetCol(WhiteColor);

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
			m_pChatPoly[POLY_TAB]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABCLOSE));
			break;
		case TABSTATE_CLOSING:
			m_tabState = TABSTATE_CLOSED;
			m_pChatPoly[POLY_TAB]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABOPEN));
			break;
		}
		// 処理の終了
		return;
	}

	// 移動値を加算し、座標更新
	m_TabPos += m_moveDest;
	m_pChatPoly[POLY_BACK]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_TAB]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_TABLET]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y - 100.0f));
	m_pChatPoly[POLY_TITLE]->SetPos(D3DXVECTOR2(m_TabPos.x, 0.0f));
	m_pChatPoly[POLY_WRITEWINDOW]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));

	m_SendText->SetKeepRectBegin(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_SENDTEXT, m_TabPos.y - DIFPOS_Y_SENDTEXT));
	m_leftText->SetKeepRectBegin(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_MAXCHAR, m_TabPos.y - DIFPOS_Y_MAXCHAR));
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
	m_fScrollPosY = 0.0f;
	m_moveDest = ZeroVector2;
	m_tabState = TABSTATE_CLOSED;
	m_tabletState = TABSTATE_CLOSED;
	m_nCntState = 0;
	m_bClickTab = false;

	// 背景の生成
	m_pChatPoly[POLY_BACK] = CPolygon2D::Create();
	m_pChatPoly[POLY_BACK]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_BACK]->SetSize(SIZE_TABBACK);
	m_pChatPoly[POLY_BACK]->SetPosStart(CPolygon2D::POSSTART_BOTTOM_LEFT);
	m_pChatPoly[POLY_BACK]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BACK));

	m_pChatPoly[POLY_TITLE] = CPolygon2D::Create();
	m_pChatPoly[POLY_TITLE]->SetPos(D3DXVECTOR2(m_TabPos.x, 0.0f));
	m_pChatPoly[POLY_TITLE]->SetSize(SIZE_TABTITLE);
	m_pChatPoly[POLY_TITLE]->SetPosStart(CPolygon2D::POSSTART_TOP_LEFT);
	m_pChatPoly[POLY_TITLE]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TITLE));

	m_pChatPoly[POLY_WRITEWINDOW] = CPolygon2D::Create();
	m_pChatPoly[POLY_WRITEWINDOW]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_WRITEWINDOW]->SetSize(SIZE_TABWRITEWINDOW);
	m_pChatPoly[POLY_WRITEWINDOW]->SetPosStart(CPolygon2D::POSSTART_BOTTOM_LEFT);
	m_pChatPoly[POLY_WRITEWINDOW]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_WRITEWINDOW));

	// 開閉タブの生成
	m_pChatPoly[POLY_TAB] = CPolygon2D::Create();
	m_pChatPoly[POLY_TAB]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y));
	m_pChatPoly[POLY_TAB]->SetSize(SIZE_TABBUTTON);
	m_pChatPoly[POLY_TAB]->SetPosStart(CPolygon2D::POSSTART_BOTTOM_RIGHT);
	m_pChatPoly[POLY_TAB]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABOPEN));
	
	// タブレット開閉の生成
	m_pChatPoly[POLY_TABLET] = CPolygon2D::Create();
	m_pChatPoly[POLY_TABLET]->SetPos(D3DXVECTOR2(m_TabPos.x, m_TabPos.y - 100.0f));
	m_pChatPoly[POLY_TABLET]->SetSize(SIZE_TABBUTTON);
	m_pChatPoly[POLY_TABLET]->SetPosStart(CPolygon2D::POSSTART_BOTTOM_RIGHT);
	m_pChatPoly[POLY_TABLET]->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_TABLETOPEN));

	// 送信用テキストの生成
	m_SendText = CChatText::Create();
	m_SendText->SetKeepRectBegin(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_SENDTEXT, m_TabPos.y - DIFPOS_Y_SENDTEXT));

	// 残り文字数テキストの生成
	m_leftText = CChatText::Create();
	m_leftText->SetKeepRectBegin(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_MAXCHAR, m_TabPos.y - DIFPOS_Y_MAXCHAR));

	m_nCntPress = 0;

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
	if ((pMouse->GetTrigger(0) && m_pChatPoly[POLY_TAB]->ReturnHit(mousePos)) ||
		CManager::GetKeyboard()->GetTrigger(DIK_F5))
		ClickTab();

	if (m_pChatPoly[POLY_TABLET]->ReturnHit(mousePos))
		ClickTabletTab();

	// マウスによるタブスクロール
	CMouse::MOUSE_SCROLL scroll = CMouse::GetScroll();
	if (scroll == CMouse::SCROLL_UP)
		ScrollUp();
	else if (scroll == CMouse::SCROLL_DOWN)
		ScrollDown();

	// タブポリゴンの更新
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_pChatPoly[nCnt])
			m_pChatPoly[nCnt]->Update();
	}
	
	float fAllChatSize = DIFPOS_Y_TEXTBOX;

	// チャット履歴の更新
	for (int nCnt = (int)m_chatKeep.size() - 1; nCnt > -1; nCnt--)
	{
		if (!m_chatKeep[nCnt].pPolyBack)
			continue;

		D3DXVECTOR2 pos;

		if (m_chatKeep[nCnt].pKeepText)
		{
			// 座標設定
			pos = D3DXVECTOR2(m_TabPos.x + DIFPOS_X_TEXTBOX,
				fAllChatSize + m_fScrollPosY);
			// 背景ポリゴンの座標を決定
			m_chatKeep[nCnt].pPolyBack->SetPos(pos);
			m_chatKeep[nCnt].pPolyBack->Update();
			// 背景座標をもとに、テキスト座標を決定
			m_chatKeep[nCnt].pKeepText->SetKeepRectBegin(D3DXVECTOR2(pos.x + DIFPOS_X_KEEPTEXT, pos.y + DIFPOS_Y_KEEPTEXT));

			fAllChatSize += DIFPOS_Y_TEXTBOX;
		}
		else if (m_chatKeep[nCnt].pPolyPic)
		{
			// 座標設定
			pos = D3DXVECTOR2(m_TabPos.x + DIFPOS_X_PICBOX,
				fAllChatSize + m_fScrollPosY);

			// 背景ポリゴンの座標を決定
			m_chatKeep[nCnt].pPolyBack->SetPos(pos);
			m_chatKeep[nCnt].pPolyBack->Update();
			// 背景座標をもとに、テキスト座標を決定
			m_chatKeep[nCnt].pPolyPic->SetPos(D3DXVECTOR2(pos.x + DIFPOS_X_PICTURE, pos.y + DIFPOS_Y_PICTURE));
			m_chatKeep[nCnt].pPolyPic->Update();

			fAllChatSize += DIFPOS_Y_PICBOX;
		}
	}

	// タブが開いているときのみ、文字を入力できる
	if (m_tabState == TABSTATE_OPENED)
		InputText();

	// 残り時数を決定
	char cText[32];
	sprintf(cText, "Left %d/%d", SIZE_CHATTEXT - (int)m_SendText->GetChatText().size(), SIZE_CHATTEXT);
	m_leftText->SetChatText(cText);
}

//=============================================================================
// 描画
//=============================================================================
void CChatTab::Draw(void)
{
	if (m_pChatPoly[POLY_TAB])
		m_pChatPoly[POLY_TAB]->Draw();

	if (m_pChatPoly[POLY_TABLET])
		m_pChatPoly[POLY_TABLET]->Draw();

	if (m_pChatPoly[POLY_BACK])
		m_pChatPoly[POLY_BACK]->Draw();

	for (int nCnt = 0; nCnt < (int)m_chatKeep.size(); nCnt++)
	{
		if (m_chatKeep[nCnt].pPolyBack)
			m_chatKeep[nCnt].pPolyBack->Draw();
		if (m_chatKeep[nCnt].pKeepText)
			m_chatKeep[nCnt].pKeepText->Draw();
		if (m_chatKeep[nCnt].pPolyPic)
			m_chatKeep[nCnt].pPolyPic->Draw();
	}

	if (m_pChatPoly[POLY_TITLE])
		m_pChatPoly[POLY_TITLE]->Draw();

	if (m_pChatPoly[POLY_WRITEWINDOW])
		m_pChatPoly[POLY_WRITEWINDOW]->Draw();

	if (m_SendText)
		m_SendText->Draw();
	if (m_leftText)
		m_leftText->Draw();
}

//==========================================================================================================================================================
// キーボード入力
//==========================================================================================================================================================
void CChatTab::InputText(void)
{
	bool bPress = false;

	// キーボードの取得
	CKeyboard *pKey = CManager::GetKeyboard();

	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{
		// 押されていないならば、終了
		if (pKey->GetPress(nCnt) != true ||
			nCnt == DIK_LSHIFT || nCnt == DIK_RSHIFT)
			continue;

		// 入力した
		bPress = true;

		// 他のキーを入力していたら、カウンタリセット
		if (m_nPressKey != nCnt)
			m_nCntPress = 0;
		// キー番号を保存
		m_nPressKey = nCnt;

		// キー入力
		PressKey(nCnt, (pKey->GetPress(DIK_LSHIFT) || pKey->GetPress(DIK_RSHIFT)));

		// 長押しカウンタ加算
		if (m_nCntPress < TIME_PRESSKEY)
			m_nCntPress++;
	}

	if (!bPress)
	{
		m_nCntPress = 0;
		m_nPressKey = 0;
	}

	// 文字列の送信
	if ((pKey->GetTrigger(DIK_RETURN) || pKey->GetTrigger(DIK_NUMPADENTER)) && 
		m_SendText->GetChatText().size() > 0)
	{
		SendChatText();
	}
}

//==========================================================================================================================================================
// キー入力
//==========================================================================================================================================================
void CChatTab::PressKey(int nKeyID, bool bShift)
{
	// シフトキーが有効
	if (bShift)
	{
		// 単発入力
		if (m_nCntPress == 1)
			SetChatShiftKeyInfo(nKeyID);
		// 連続入力
		if (m_nCntPress >= TIME_PRESSKEY)
			SetChatShiftKeyInfo(nKeyID);
		// 処理を終える
		return;
	}

	if (nKeyID != DIK_BACKSPACE)
	{
		// 単発入力
		if (m_nCntPress == 1)
			SetChatKeyInfo(nKeyID);
		// 連続入力
		if (m_nCntPress >= TIME_PRESSKEY)
			SetChatKeyInfo(nKeyID);
	}
	else
	{
		// 単発入力
		if (m_nCntPress == 1 && (int)m_SendText->GetChatText().size() > 0)
			m_SendText->GetChatText().pop_back();
		// 連続入力
		if (m_nCntPress >= TIME_PRESSKEY && (int)m_SendText->GetChatText().size() > 0)
			m_SendText->GetChatText().pop_back();
	}
}

//==========================================================================================================================================================
// テキストの送信
//==========================================================================================================================================================
void CChatTab::SendChatText(void)
{
	// 記入したテキストを送信
	std::thread t2(CClient::SendText);
	t2.detach();
}

//==========================================================================================================================================================
// ピクチャの追加
//==========================================================================================================================================================
void CChatTab::AddPicture(CChatBase::TEXTOWNER owner, LPDIRECT3DTEXTURE9 pTexture)
{
	// テキストを末尾に追加
	CHATKEEP keep;
	m_chatKeep.push_back(keep);
	// 末尾の番号を取得
	int nNumber = (int)m_chatKeep.size() - 1;

	// 背景ポリゴンの生成
	m_chatKeep[nNumber].pPolyBack = CPolygon2D::Create();
	m_chatKeep[nNumber].pPolyBack->SetPos(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_TEXTBOX, DIFPOS_Y_TEXTBOX));
	m_chatKeep[nNumber].pPolyBack->SetSize(D3DXVECTOR2(SIZE_X_TEXTBOX_PIC, SIZE_Y_TEXTBOX_PIC));
	m_chatKeep[nNumber].pPolyBack->SetPosStart(CPolygon2D::POSSTART_TOP_LEFT);
	owner == CChatBase::OWNER_OWN ?
		m_chatKeep[nNumber].pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BOX_02)) :
		m_chatKeep[nNumber].pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BOX_03));

	// 文字列の生成
	D3DXVECTOR2 BackPos = m_chatKeep[nNumber].pPolyBack->GetPos();
	m_chatKeep[nNumber].pPolyPic = CPolygon2D::Create();
	m_chatKeep[nNumber].pPolyPic->SetPos(BackPos);
	m_chatKeep[nNumber].pPolyPic->SetSize(D3DXVECTOR2(SIZE_X_PICTURE, SIZE_Y_PICTURE));
	m_chatKeep[nNumber].pPolyPic->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
	m_chatKeep[nNumber].pPolyPic->BindTexture(pTexture);

	m_chatKeep[nNumber].pKeepText = nullptr;

	// 古いほうから削除
	if (nNumber >= MAX_KEEPTEXT)
		m_chatKeep.erase(m_chatKeep.begin());
}

//==========================================================================================================================================================
// テキストの受信
//==========================================================================================================================================================
void CChatTab::RecvChatText(char *cText)
{
	// チャットキープの生成
	CreateKeep(CChatBase::OWNER_GUEST, cText);
}

//==========================================================================================================================================================
// マウススクロールによる座標上昇
//==========================================================================================================================================================
void CChatTab::ScrollUp(void)
{
	// ゲーム中のみ
	if (CManager::GetMode() != CManager::MODE_GAME || m_tabState != TABSTATE_OPENED)
		return;
	// 上昇
	m_fScrollPosY += MOVE_SCROLL_TAB;

	if (m_fScrollPosY > -DIFPOS_Y_TEXTBOX_UNDER + (int)m_chatKeep.size() * SIZE_Y_TEXTBOX)
		m_fScrollPosY = -DIFPOS_Y_TEXTBOX_UNDER + (int)m_chatKeep.size() * SIZE_Y_TEXTBOX;
}

//==========================================================================================================================================================
// マウススクロールによる座標下降
//==========================================================================================================================================================
void CChatTab::ScrollDown(void)
{
	// ゲーム中のみ
	if (CManager::GetMode() != CManager::MODE_GAME || m_tabState != TABSTATE_OPENED)
		return;
	// 下降
	m_fScrollPosY -= MOVE_SCROLL_TAB;

	if (m_fScrollPosY < DIFPOS_Y_TEXTBOX + (int)m_chatKeep.size() * -SIZE_Y_TEXTBOX)
		m_fScrollPosY = DIFPOS_Y_TEXTBOX + (int)m_chatKeep.size() * -SIZE_Y_TEXTBOX;
}

//==========================================================================================================================================================
// チャットキープの生成
//==========================================================================================================================================================
void CChatTab::CreateKeep(CChatBase::TEXTOWNER owner, char *cText)
{
	char *cNewText = new char[strlen(cText) + 1];
	strcpy(cNewText, cText);

	// テキストを末尾に追加
	CHATKEEP keep;
	m_chatKeep.push_back(keep);
	// 末尾の番号を取得
	int nNumber = (int)m_chatKeep.size() - 1;

	// 背景ポリゴンの生成
	m_chatKeep[nNumber].pPolyBack = CPolygon2D::Create();
	m_chatKeep[nNumber].pPolyBack->SetPos(D3DXVECTOR2(m_TabPos.x + DIFPOS_X_TEXTBOX, DIFPOS_Y_TEXTBOX));
	m_chatKeep[nNumber].pPolyBack->SetSize(D3DXVECTOR2(SIZE_X_TEXTBOX, SIZE_Y_TEXTBOX));
	m_chatKeep[nNumber].pPolyBack->SetPosStart(CPolygon2D::POSSTART_TOP_LEFT);
	owner == CChatBase::OWNER_OWN ?
		m_chatKeep[nNumber].pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BOX_00)) :
		m_chatKeep[nNumber].pPolyBack->BindTexture(CTexture::GetTexture(CTexture::TEX_CHAT_BOX_01));

	// 文字列の生成
	D3DXVECTOR3 BackPos = m_chatKeep[nNumber].pPolyBack->GetPos();
	m_chatKeep[nNumber].pKeepText = CChatText::Create();
	m_chatKeep[nNumber].pKeepText->SetKeepColor(BlackColor);
	m_chatKeep[nNumber].pKeepText->SetKeepRectBegin(D3DXVECTOR2(BackPos.x + DIFPOS_X_KEEPTEXT, BackPos.y + DIFPOS_Y_KEEPTEXT));

	for (int nCnt = nNumber - 1; nCnt > -1; nCnt--)
	{
		if (!m_chatKeep[nCnt].pKeepText)
			continue;

		D3DXVECTOR2 pos = m_chatKeep[nCnt + 1].pPolyBack->GetPos();
		m_chatKeep[nCnt].pPolyBack->SetPos(D3DXVECTOR2(pos.x, pos.y + SIZE_Y_TEXTBOX));
		pos = m_chatKeep[nCnt].pPolyBack->GetPos();
		m_chatKeep[nCnt].pKeepText->SetKeepRectBegin(D3DXVECTOR2(pos.x + DIFPOS_X_KEEPTEXT, pos.y + DIFPOS_Y_KEEPTEXT));
	}

	// チャットに保存
	m_chatKeep[nNumber].pKeepText->SetChatText(cNewText);

	delete[] cNewText;
	m_chatKeep[nNumber].pPolyPic = nullptr;

	// 古いほうから削除
	if (nNumber >= MAX_KEEPTEXT)
		m_chatKeep.erase(m_chatKeep.begin());
}

//==========================================================================================================================================================
// チャットのキー情報の設定
//==========================================================================================================================================================
void CChatTab::SetChatKeyInfo(int nKeyID)
{
	// 文字数を超えていたら、処理しない
	if ((int)m_SendText->GetChatText().size() >= SIZE_CHATTEXT)
		return;

	// 文字を追加
	switch (nKeyID)
	{
	case DIK_1:				m_SendText->GetChatText().push_back('1'); break;
	case DIK_2:				m_SendText->GetChatText().push_back('2'); break;
	case DIK_3:				m_SendText->GetChatText().push_back('3'); break;
	case DIK_4:				m_SendText->GetChatText().push_back('4'); break;
	case DIK_5:				m_SendText->GetChatText().push_back('5'); break;
	case DIK_6:				m_SendText->GetChatText().push_back('6'); break;
	case DIK_7:				m_SendText->GetChatText().push_back('7'); break;
	case DIK_8:				m_SendText->GetChatText().push_back('8'); break;
	case DIK_9:				m_SendText->GetChatText().push_back('9'); break;
	case DIK_0:				m_SendText->GetChatText().push_back('0'); break;
	case DIK_A:				m_SendText->GetChatText().push_back('a'); break;
	case DIK_B:				m_SendText->GetChatText().push_back('b'); break;
	case DIK_C:				m_SendText->GetChatText().push_back('c'); break;
	case DIK_D:				m_SendText->GetChatText().push_back('d'); break;
	case DIK_E:				m_SendText->GetChatText().push_back('e'); break;
	case DIK_F:				m_SendText->GetChatText().push_back('f'); break;
	case DIK_G:				m_SendText->GetChatText().push_back('g'); break;
	case DIK_H:				m_SendText->GetChatText().push_back('h'); break;
	case DIK_I:				m_SendText->GetChatText().push_back('i'); break;
	case DIK_J:				m_SendText->GetChatText().push_back('j'); break;
	case DIK_K:				m_SendText->GetChatText().push_back('k'); break;
	case DIK_L:				m_SendText->GetChatText().push_back('l'); break;
	case DIK_M:				m_SendText->GetChatText().push_back('m'); break;
	case DIK_N:				m_SendText->GetChatText().push_back('n'); break;
	case DIK_O:				m_SendText->GetChatText().push_back('o'); break;
	case DIK_P:				m_SendText->GetChatText().push_back('p'); break;
	case DIK_Q:				m_SendText->GetChatText().push_back('q'); break;
	case DIK_R:				m_SendText->GetChatText().push_back('r'); break;
	case DIK_S:				m_SendText->GetChatText().push_back('s'); break;
	case DIK_T:				m_SendText->GetChatText().push_back('t'); break;
	case DIK_U:				m_SendText->GetChatText().push_back('u'); break;
	case DIK_V:				m_SendText->GetChatText().push_back('v'); break;
	case DIK_W:				m_SendText->GetChatText().push_back('w'); break;
	case DIK_X:				m_SendText->GetChatText().push_back('x'); break;
	case DIK_Y:				m_SendText->GetChatText().push_back('y'); break;
	case DIK_Z:				m_SendText->GetChatText().push_back('z'); break;
	case DIK_MINUS:			m_SendText->GetChatText().push_back('-'); break;
	case DIK_CIRCUMFLEX:	m_SendText->GetChatText().push_back('^'); break;
	case DIK_YEN:			m_SendText->GetChatText().push_back('\\'); break;
	case DIK_AT:			m_SendText->GetChatText().push_back('@'); break;
	case DIK_LBRACKET:		m_SendText->GetChatText().push_back('['); break;
	case DIK_SEMICOLON:		m_SendText->GetChatText().push_back(';'); break;
	case DIK_COLON:			m_SendText->GetChatText().push_back(':'); break;
	case DIK_RBRACKET:		m_SendText->GetChatText().push_back(']'); break;
	case DIK_COMMA:			m_SendText->GetChatText().push_back(','); break;
	case DIK_PERIOD:		m_SendText->GetChatText().push_back('.'); break;
	case DIK_SLASH:			m_SendText->GetChatText().push_back('/'); break;
	case DIK_BACKSLASH:		m_SendText->GetChatText().push_back('\\'); break;
	case DIK_SPACE:			m_SendText->GetChatText().push_back(' '); break;
	case DIK_NUMPAD0:		m_SendText->GetChatText().push_back('0'); break;
	case DIK_NUMPAD1:		m_SendText->GetChatText().push_back('1'); break;
	case DIK_NUMPAD2:		m_SendText->GetChatText().push_back('2'); break;
	case DIK_NUMPAD3:		m_SendText->GetChatText().push_back('3'); break;
	case DIK_NUMPAD4:		m_SendText->GetChatText().push_back('4'); break;
	case DIK_NUMPAD5:		m_SendText->GetChatText().push_back('5'); break;
	case DIK_NUMPAD6:		m_SendText->GetChatText().push_back('6'); break;
	case DIK_NUMPAD7:		m_SendText->GetChatText().push_back('7'); break;
	case DIK_NUMPAD8:		m_SendText->GetChatText().push_back('8'); break;
	case DIK_NUMPAD9:		m_SendText->GetChatText().push_back('9'); break;
	case DIK_DECIMAL:		m_SendText->GetChatText().push_back('.'); break;
	case DIK_ADD:			m_SendText->GetChatText().push_back('+'); break;
	case DIK_SUBTRACT:		m_SendText->GetChatText().push_back('-'); break;
	case DIK_MULTIPLY:		m_SendText->GetChatText().push_back('*'); break;
	case DIK_DIVIDE:		m_SendText->GetChatText().push_back('/'); break;
	}
}

//==========================================================================================================================================================
// チャットのシフトキー情報の設定
//==========================================================================================================================================================
void CChatTab::SetChatShiftKeyInfo(int nKeyID)
{
	// 文字数を超えていたら、処理しない
	if ((int)m_SendText->GetChatText().size() >= SIZE_CHATTEXT)
		return;

	// 文字を追加
	switch (nKeyID)
	{
	case DIK_1:				m_SendText->GetChatText().push_back('!'); break;
	case DIK_2:				m_SendText->GetChatText().push_back('"'); break;
	case DIK_3:				m_SendText->GetChatText().push_back('#'); break;
	case DIK_4:				m_SendText->GetChatText().push_back('$'); break;
	case DIK_5:				break;
	case DIK_6:				m_SendText->GetChatText().push_back('&'); break;
	case DIK_7:				break;
	case DIK_8:				m_SendText->GetChatText().push_back('('); break;
	case DIK_9:				m_SendText->GetChatText().push_back(')'); break;
	case DIK_0:				break;
	case DIK_A:				m_SendText->GetChatText().push_back('A'); break;
	case DIK_B:				m_SendText->GetChatText().push_back('B'); break;
	case DIK_C:				m_SendText->GetChatText().push_back('C'); break;
	case DIK_D:				m_SendText->GetChatText().push_back('D'); break;
	case DIK_E:				m_SendText->GetChatText().push_back('E'); break;
	case DIK_F:				m_SendText->GetChatText().push_back('F'); break;
	case DIK_G:				m_SendText->GetChatText().push_back('G'); break;
	case DIK_H:				m_SendText->GetChatText().push_back('H'); break;
	case DIK_I:				m_SendText->GetChatText().push_back('I'); break;
	case DIK_J:				m_SendText->GetChatText().push_back('J'); break;
	case DIK_K:				m_SendText->GetChatText().push_back('K'); break;
	case DIK_L:				m_SendText->GetChatText().push_back('L'); break;
	case DIK_M:				m_SendText->GetChatText().push_back('M'); break;
	case DIK_N:				m_SendText->GetChatText().push_back('N'); break;
	case DIK_O:				m_SendText->GetChatText().push_back('O'); break;
	case DIK_P:				m_SendText->GetChatText().push_back('P'); break;
	case DIK_Q:				m_SendText->GetChatText().push_back('Q'); break;
	case DIK_R:				m_SendText->GetChatText().push_back('R'); break;
	case DIK_S:				m_SendText->GetChatText().push_back('S'); break;
	case DIK_T:				m_SendText->GetChatText().push_back('T'); break;
	case DIK_U:				m_SendText->GetChatText().push_back('U'); break;
	case DIK_V:				m_SendText->GetChatText().push_back('V'); break;
	case DIK_W:				m_SendText->GetChatText().push_back('W'); break;
	case DIK_X:				m_SendText->GetChatText().push_back('X'); break;
	case DIK_Y:				m_SendText->GetChatText().push_back('Y'); break;
	case DIK_Z:				m_SendText->GetChatText().push_back('Z'); break;
	case DIK_MINUS:			m_SendText->GetChatText().push_back('='); break;
	case DIK_CIRCUMFLEX:	m_SendText->GetChatText().push_back('~'); break;
	case DIK_YEN:			m_SendText->GetChatText().push_back('|'); break;
	case DIK_AT:			m_SendText->GetChatText().push_back('`'); break;
	case DIK_LBRACKET:		m_SendText->GetChatText().push_back('{'); break;
	case DIK_SEMICOLON:		m_SendText->GetChatText().push_back('+'); break;
	case DIK_COLON:			m_SendText->GetChatText().push_back('*'); break;
	case DIK_RBRACKET:		m_SendText->GetChatText().push_back('}'); break;
	case DIK_COMMA:			m_SendText->GetChatText().push_back('<'); break;
	case DIK_PERIOD:		m_SendText->GetChatText().push_back('>'); break;
	case DIK_SLASH:			m_SendText->GetChatText().push_back('?'); break;
	case DIK_BACKSLASH:		m_SendText->GetChatText().push_back('_'); break;
	case DIK_SPACE:			break;
	case DIK_NUMPAD0:		break;
	case DIK_NUMPAD1:		break;
	case DIK_NUMPAD2:		break;
	case DIK_NUMPAD3:		break;
	case DIK_NUMPAD4:		break;
	case DIK_NUMPAD5:		break;
	case DIK_NUMPAD6:		break;
	case DIK_NUMPAD7:		break;
	case DIK_NUMPAD8:		break;
	case DIK_NUMPAD9:		break;
	case DIK_DECIMAL:		break;
	case DIK_ADD:			break;
	case DIK_SUBTRACT:		break;
	case DIK_MULTIPLY:		break;
	case DIK_DIVIDE:		break;
	}
}