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

// ===================================================================
// マクロ定義
// ===================================================================
#define MAX_CHARACTER		(2048)	// 文字の最大数
#define MAX_KEEPTEXT		(20)	// 保持できるテキストの最大数
#define SIZE_CHATTEXT		(256)	// チャットテキストの最大サイズ

// ===================================================================
// クラス定義
// ===================================================================
class CPolygon2D;
class CChatText
{
public:
	typedef enum
	{	// キー
		// 数字
		CHATKEY_0, CHATKEY_1, CHATKEY_2, CHATKEY_3, CHATKEY_4,
		CHATKEY_5, CHATKEY_6, CHATKEY_7, CHATKEY_8, CHATKEY_9,
		// アルファベット
		CHATKEY_A, CHATKEY_B, CHATKEY_C, CHATKEY_D, CHATKEY_E,
		CHATKEY_F, CHATKEY_G, CHATKEY_H, CHATKEY_I, CHATKEY_J,
		CHATKEY_K, CHATKEY_L, CHATKEY_M, CHATKEY_N, CHATKEY_O,
		CHATKEY_P, CHATKEY_Q, CHATKEY_R, CHATKEY_S, CHATKEY_T,
		CHATKEY_U, CHATKEY_V, CHATKEY_W, CHATKEY_X, CHATKEY_Y,
		CHATKEY_Z,
		// 記号
		CHATEKY_DASH,					// -
		CHATEKY_CIRCUMFLEX_ACCENT,		// ^
		CHATEKY_YEN,					// \マーク
		CHATEKY_AT,						// @
		CHATEKY_LEFT_SQUARE_BRACKET,	// [
		CHATEKY_SEMICOLON,				// ;
		CHATEKY_COLON,					// :
		CHATEKY_RIGHT_SQUARE_BRACKET,	// ]
		CHATEKY_COMMA,					// ,
		CHATEKY_FULL_STOP,				// .
		CHATEKY_SOLIDUS,				// /
		CHATEKY_BACK_SOLIDUS,			// \マーク
		CHATKEY_SPACE,					// 空欄 スペースキー
		// テンキー
		CHATKEY_NUMKEY_0, CHATKEY_NUMKEY_1, CHATKEY_NUMKEY_2, CHATKEY_NUMKEY_3, CHATKEY_NUMKEY_4,
		CHATKEY_NUMKEY_5, CHATKEY_NUMKEY_6, CHATKEY_NUMKEY_7, CHATKEY_NUMKEY_8, CHATKEY_NUMKEY_9,
		CHATKEY_NUMKEY_FULL_STOP,		// . (テンキー)
		CHATKEY_NUMKEY_SOLIDUS,			// / (テンキー)
		CHATKEY_NUMKEY_ASTERISK,		// * (テンキー)
		CHATKEY_NUMKEY_DASH,			// - (テンキー)
		CHATKEY_NUMKEY_PLUS,			// + (テンキー)
		CHATKEY_MAX
	} CHATKEY;

	typedef enum
	{	// キー + Shift
		// 数字
		CHATKEY_SHIFT_EXCLAMATION,			// !
		CHATKEY_SHIFT_BOUBLE_QUOTATION,		// "
		CHATKEY_SHIFT_HASH,					// #
		CHATKEY_SHIFT_DOLLAR,				// $
		CHATKEY_SHIFT_PERCENT,				// %
		CHATKEY_SHIFT_AMPERSAND,			// &
		CHATKEY_SHIFT_QUOTATION,			// '
		CHATKEY_SHIFT_LEFT_PARENTHESIS,		// (
		CHATKEY_SHIFT_RIGHT_PARENTHESIS,	// )
		// アルファベット
		CHATKEY_SHIFT_A, CHATKEY_SHIFT_B, CHATKEY_SHIFT_C, CHATKEY_SHIFT_D, CHATKEY_SHIFT_E,
		CHATKEY_SHIFT_F, CHATKEY_SHIFT_G, CHATKEY_SHIFT_H, CHATKEY_SHIFT_I, CHATKEY_SHIFT_J,
		CHATKEY_SHIFT_K, CHATKEY_SHIFT_L, CHATKEY_SHIFT_M, CHATKEY_SHIFT_N, CHATKEY_SHIFT_O,
		CHATKEY_SHIFT_P, CHATKEY_SHIFT_Q, CHATKEY_SHIFT_R, CHATKEY_SHIFT_S, CHATKEY_SHIFT_T,
		CHATKEY_SHIFT_U, CHATKEY_SHIFT_V, CHATKEY_SHIFT_W, CHATKEY_SHIFT_X, CHATKEY_SHIFT_Y,
		CHATKEY_SHIFT_Z,
		// 記号
		CHATEKY_SHIFT_EQUAL,				// =
		CHATEKY_SHIFT_TILDE,				// ~
		CHATEKY_SHIFT_VERTICAL_LINE,		// |
		CHATEKY_SHIFT_GRAVE_ACCENT,			// `
		CHATEKY_SHIFT_LEFT_CURLY_BRACKET,	// {
		CHATEKY_SHIFT_PLUS,					// +
		CHATEKY_SHIFT_ASTERISK,				// *
		CHATEKY_SHIFT_RIGHT_CURLY_BRACKET,	// }
		CHATEKY_SHIFT_LESS_THAN,			// <
		CHATEKY_SHIFT_GREATER_THAN,			// >
		CHATEKY_SHIFT_QUESTION,				// ?
		CHATEKY_SHIFT_UNDER_LINE,			// _
		CHATKEY_SHIFT_MAX
	} CHATKEY_SHIFT;

	CChatText() {};		// コンストラクタ
	~CChatText() {};	// デストラクタ

	void Init(void);					// 初期化
	void Uninit(void);					// 終了
	void static Print(char* fmt, ...);	// テキストの設定 (引数を...にすれば何個でも受け取れる)
	void Draw(void);				// 描画
	void ShowDebugInfo() {};		//デバッグ情報表記

	D3DXCOLOR &GetTextColor(void) { return m_textColor; } // テキストカラーの取得
	void SetTextColor(D3DXCOLOR &col) { m_textColor = col; } // テキストカラーの設定

private:
	static LPD3DXFONT	m_pFont;				// フォント情報格納用
	static char			m_aStr[MAX_CHARACTER];	// テキストの格納用配列
	char				m_cKeepText[MAX_KEEPTEXT][SIZE_CHATTEXT];	// 保持できるテキスト
	D3DXCOLOR			m_textColor;			// テキストのカラー
};

#endif