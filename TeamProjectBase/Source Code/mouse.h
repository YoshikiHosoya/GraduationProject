//*************************************************************************************************************
//
// マウス処理 [mouse.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _MOUSE_H_
#define _MOUSE_H_

#define _CRT_SECURE_NO_WARNINGS

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "input.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define	NUM_BUTTON_MAX			(8)		// ボタン(マウス)の最大数

//-------------------------------------------------------------------------------------------------------------
// クラスの定義
//-------------------------------------------------------------------------------------------------------------
class CMouse :public CInput
{
public:
	/* 列挙型定義 */
	typedef enum
	{
		CUR_NONE = -1,	// 無し
		CUR_PEN,		// ペン
		CUR_ERASER,		// 消しゴム
		CUR_MAX			// 最大数
	}CURTYPE;

	typedef enum
	{
		SCROLL_NONE,	// スクロールしていない
		SCROLL_UP,		// 上にスクロール
		SCROLL_DOWN,	// 下にスクロール
		SCROLL_MAX
	} MOUSE_SCROLL;

	/* 構造体定義 */
	typedef struct
	{
		DIMOUSESTATE2 state;	// 状態
		POINT         point;		// 位置
	} MOUSE;

	/* メンバ関数 */
	CMouse();
	~CMouse();
	HRESULT              Init(HINSTANCE hInstance, HWND hWnd);
	void                 Update(void);
	void                 Draw(void);

	// 設定関数
	inline void          SetCursorType(CONST CURTYPE type) { if (m_type != type)m_type = type; }					// カーソルタイプの設定
	inline void          SetDisp(bool bDisp) { if (m_bDisp != bDisp)m_bDisp = bDisp; }								// 描画フラグの設定
	static void			 SetScroll(MOUSE_SCROLL scroll) { m_scroll = scroll; }										// スクロールの状態の設定

	// 取得関数
	inline bool          GetPress(int nButton) { return (m_aButtonState[nButton] & 0x80) ? true : false; }			// 押しているとき
	inline bool          GetTrigger(int nButton) { return (m_aButtonStateTrigger[nButton] & 0x80) ? true : false; }	// 押した時
	inline bool          GetRelease(int nButton) { return (m_aButtonStateRelease[nButton] & 0x80) ? true : false; }	// 離した時
	inline LONG          GetMouseX(void) { return m_mouse.point.x; }												// マウスX軸
	inline LONG          GetMouseY(void) { return m_mouse.point.y; }												// マウスY軸
	inline POINT         GetMousePoint(void) { return m_mouse.point; }												// マウスの位置
	inline D3DXVECTOR2   GetMousePos(void) { return D3DXVECTOR2((float)m_mouse.point.x, (float)m_mouse.point.y); }				// マウスの位置の取得
	inline DIMOUSESTATE2 GetMouseState(void) { return m_mouse.state; }												// マウスの状態の取得
	static MOUSE_SCROLL  GetScroll(void) { return m_scroll; }														// スクロールの状態の取得
	static int           GetScrollValue(void);																		// スクロールした値を取得する
private:
	/* メンバ変数 */
	LPDIRECTINPUTDEVICE8 m_pDevice;								// デバイス
	MOUSE                m_mouse;								// マウスの情報
	BYTE                 m_aButtonState[NUM_BUTTON_MAX];		// マウスの入力情報ワーク
	BYTE                 m_aButtonStateTrigger[NUM_BUTTON_MAX];	// マウスのトリガー
	BYTE                 m_aButtonStateRelease[NUM_BUTTON_MAX];	// 離した時
	HWND                 m_hWnd;								// ウィンドウハンドル
	HCURSOR              m_hCursor[CUR_MAX];					// カーソルハンドル
	CURTYPE              m_type;								// カーソルタイプ
	bool                 m_bDisp;								// 描画したフラグ
	static MOUSE_SCROLL	 m_scroll;								// マウスのスクロールの状態
};

#endif
