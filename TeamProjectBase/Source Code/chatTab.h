// ===================================================================
//
// チャットのタブ処理 [ chatTab.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CHATTAB_H_
#define _CHATTAB_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "chatBase.h"

// ===================================================================
// マクロ定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CPolygon2D;
class CChatText;

class CChatTab
{
public:
	typedef enum
	{	// 常に表示するチャットタブの種類
		POLY_BACK,			// 背景
		POLY_TITLE,			// タイトル
		POLY_WRITEWINDOW,	// 書き込みウィンドウ
		POLY_TAB,			// 開閉タブ
		POLY_MAX
	} POLYTYPE;

	typedef struct
	{	// チャットキープの情報
		CChatText *pKeepText;
		CPolygon2D *pPolyBack;
	} CHATKEEP;

	typedef enum
	{	// チャットタブのテクスチャタイプ
		CHATTAB_BACK,	// 背景
		CHATTAB_OPEN,	// 開く
		CHATTAB_CLOSE,	// 閉じる
		CHATTAB_MAX
	} CHATTAB_TEXTYPE;

	typedef enum
	{	// タブの状態
		TABSTATE_NONE,		// 何もない
		TABSTATE_CLOSING,	// 閉じる状態
		TABSTATE_CLOSED,	// 閉じた状態
		TABSTATE_OPENNING,	// 開く状態
		TABSTATE_OPENED,	// 開いた状態
		TABSTATE_MAX
	} TABSTATE;

	CChatTab();
	~CChatTab();

	HRESULT Init(void);			// 初期化
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void ShowDebugInfo() {};		//デバッグ情報表記

	static CChatTab * Create(void);	// 生成

	static D3DXVECTOR2	&GetTabPos(void)			{ return m_TabPos; }			// タブ座標の取得
	static TABSTATE		&GetTabState(void)			{ return m_tabState; }			// タブの状態の取得
	static CHATKEEP		*GetChatKeep(int nIndex)	{ return &m_chatKeep[nIndex]; }	// テキストの背景ポリゴン取得
	static void			SetTabPos(D3DXVECTOR2 &pos)	{ m_TabPos = pos; }				// タブ座標の設定
	static void			SendChatText(void);											// テキストの送信
	static void			SendPicture(void);											// ピクチャの送信
	static void			RecvChatText(char *cText);									// テキストの受信
	static void			ScrollUp(void);												// チャット履歴の上スクロール
	static void			ScrollDown(void);											// チャット履歴の下スクロール

private:
	void		ClickTab(void);											// タブクリック
	void		SlideTab(void);											// タブスライド
	void		InputText(void);										// キー入力の総括
	void		PressKey(int nKeyID, bool bShift);						// キー入力
	void		SetChatKeyInfo(int nKeyID);								// 通常の入力
	void		SetChatShiftKeyInfo(int nKeyID);						// シフトキーを用いた入力
	static void CreateKeep(CChatBase::TEXTOWNER owner, char *cText);	// チャットキープの生成

	static std::vector<CHATKEEP>	m_chatKeep;							// 保持できるテキスト
	static TABSTATE					m_tabState;							// タブの状態
	static D3DXVECTOR2				m_TabPos;							// タブの親座標
	static float					m_fScrollPosY;						// マウススクロールの座標
	static int						m_nCntPress;						// 長押しのカウンタ
	static int						m_nPressKey;						// 入力されたキーを格納する
	static CChatText				*m_leftText;						// 残り時数を表示するテキスト
	static CChatText				*m_SendText;						// 送るテキスト
	CPolygon2D						*m_pChatPoly[POLY_MAX];				// チャット用画像のポリゴン
	int								m_nCntState;						// 状態管理のカウンタ
	D3DXVECTOR2						m_moveDest;							// タブ移動の量
};

#endif
