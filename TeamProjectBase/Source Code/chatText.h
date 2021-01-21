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

// ===================================================================
// クラス定義
// ===================================================================
class CPolygon2D;
class CKeyboard;

class CChatText
{
public:
	CChatText() {};		// コンストラクタ
	~CChatText() {};	// デストラクタ

	void Init(void);					// 初期化
	void Uninit(void);					// 終了
	void Update(void);			// 更新
	void Draw(void);				// 描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	static CChatText *Create(void);
	static HRESULT LoadFont(void);
	static void UnloadFont(void);

	// テキストの取得
	std::string &GetChatText(void) { return m_text; }
	// テキストの設定
	void SetChatText(char *cText) 
	{ 
		m_text.clear();
		m_text = cText; 
	}
	// 保持するテキストの位置設定
	void SetKeepRectBegin(D3DXVECTOR2 rectBegin)
	{ m_rect = { (LONG)rectBegin.x, (LONG)rectBegin.y, SCREEN_WIDTH, SCREEN_HEIGHT }; }
	RECT GetRect(void)
	{ return m_rect; }
	// 保持するテキストのカラー設定
	void SetKeepColor(D3DXCOLOR col)
	{ m_col = col; }

private:
	static LPD3DXFONT	m_pFont;	// フォント情報格納用
	int			m_nIndex;
	std::string m_text;
	RECT		m_rect;
	int			m_nLine;
	D3DXCOLOR	m_col;
};

#endif