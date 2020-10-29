//------------------------------------------------------------------------------
//
//入力処理  [input.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "dinput.h"
#include <XInput.h>

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CInput
{
public:
	CInput();			//コンストラクタ
	virtual ~CInput();	//コンストラクタ

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	virtual void Update() = 0;								//更新
	LPDIRECTINPUT8 GetInput() { return m_pInput; };			//インプットデバイスの取得

protected:
	static LPDIRECTINPUT8 m_pInput;							//Input
};

#endif