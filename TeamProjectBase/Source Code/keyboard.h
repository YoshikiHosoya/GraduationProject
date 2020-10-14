//------------------------------------------------------------------------------
//
//キーボード処理  [keyboard.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "input.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define	NUM_KEY_MAX		(256)	// キーの最大数
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CKeyboard : public CInput
{
public:
	CKeyboard();
	~CKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	void Uninit();									//終了
	void Update();									//更新
	bool GetPress(int nKey);						//プレス処理
	bool GetTrigger(int nKey);						//トリガー処理

private:
	LPDIRECTINPUTDEVICE8	m_pDevKeyboard = NULL;
	BYTE					m_aKeyState[NUM_KEY_MAX];
	BYTE					m_aKeyStateTrigger[NUM_KEY_MAX];

};

#endif