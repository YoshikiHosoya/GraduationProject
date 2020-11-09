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

class CChatTab : public CChatBase
{
public:
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

	static D3DXVECTOR2 &GetTabPos(void)			{ return m_TabPos; }			// タブ座標の取得
	static TABSTATE &GetTabState(void)			{ return m_tabState; }			// タブの状態の取得
	static CHATKEEP *GetChatKeep(int nIndex)	{ return &m_chatKeep[nIndex]; }	// テキストの背景ポリゴン取得
	static void SetTabPos(D3DXVECTOR2 &pos)		{ m_TabPos = pos; }				// タブ座標の設定
	static int AddTextBox(int nIndex);											// テキストボックス追加
	static void InputText(void);
	static void PressKey(int nKeyID, bool bShift);
	static void SetChatKeyInfo(int nKeyID);
	static void SetChatShiftKeyInfo(int nKeyID);
	static void SendChatText(void);
	static void RecvChatText(void);

private:
	void ClickTab(void);	// タブクリック
	void SlideTab(void);	// タブスライド
	static void CreateKeep(TEXTOWNER owner);	// チャットキープの生成

	CPolygon2D	*m_pPolyBack;		// 背景タブのポリゴン
	CPolygon2D	*m_pPolyTab;		// 開閉タブのポリゴン
	static CChatText	*m_pChatText;		// テキスト
	static TABSTATE m_tabState;		// タブの状態
	int m_nCntState;				// 状態管理のカウンタ
	static D3DXVECTOR2 m_TabPos;	// タブの親座標
	D3DXVECTOR2 m_moveDest;			// タブ移動の量
	static int			m_nCntPress;				// 長押しのカウンタ
	static int			m_nPressKey;
	static std::vector<CHATKEEP>	m_chatKeep;	// 保持できるテキスト
	static CChatText	*m_leftText;					// 残り時数を表示するテキスト
	static CChatText	*m_SendText;				// 送るテキスト
};

#endif
