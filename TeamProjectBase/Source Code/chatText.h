// ===================================================================
//
// チャット用テキスト処理 [ chatText.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CHATTEXT_H_
#define _CHATTEXT_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include <stdarg.h>
#include <assert.h>
#include "main.h"
#include "dinput.h"

// ===================================================================
// マクロ定義
// ===================================================================
#define MAX_CHARACTER		(2048)	// 文字の最大数
#define MAX_KEEPTEXT		(20)	// 保持できるテキストの最大数
#define SIZE_CHATTEXT		(64)	// チャットテキストの最大サイズ

// ===================================================================
// クラス定義
// ===================================================================
class CPolygon2D;
class CKeyboard;

class CChatText
{
public:
	typedef struct
	{	// 保持するテキストの内容
		int nIndex;					// 背景ポリゴンの番号
		char cText[SIZE_CHATTEXT];	// 文字列
		RECT rect;					// 描画範囲
		int nLine;					// 行数
	} CHATTEXT;

	CChatText() {};		// コンストラクタ
	~CChatText() {};	// デストラクタ

	void Init(void);					// 初期化
	void Uninit(void);					// 終了
	static void Draw(void);				// 描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	D3DXCOLOR &GetTextColor(void) { return m_textColor; } // テキストカラーの取得
	void SetTextColor(D3DXCOLOR &col) { m_textColor = col; } // テキストカラーの設定

	static void InputText(void);
	static void SetChatKeyInfo(int nKeyID);
	static void SetChatShiftKeyInfo(int nKeyID);
	static void PressKey(int nKeyID, bool bShift);
	static void SendChatText(void);

private:
	static void DrawWriteMessage(void);				// 記入中のメッセージの描画
	static void DrawLeftChar(void);					// 残り文字数の描画
	static LPD3DXFONT	m_pFont;					// フォント情報格納用
	static char			m_aStr[MAX_CHARACTER];		// テキストの格納用配列
	static CHATTEXT		m_keepText[MAX_KEEPTEXT];	// 保持できるテキスト
	static std::string	m_cSendText;				// 送るための文字列
	static D3DXCOLOR	m_textColor;				// テキストのカラー
	static int			m_nCntPress;				// 長押しのカウンタ
	static int			m_nPressKey;
};

#endif