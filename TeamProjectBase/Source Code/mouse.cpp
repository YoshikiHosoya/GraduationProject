//*************************************************************************************************************
//
// マウス入力処理 [mouse.cpp]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "mouse.h"
#include "keyboard.h"
#include "manager.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CMouse::MOUSE_SCROLL CMouse::m_scroll = SCROLL_NONE;	// マウスのスクロールの状態

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CMouse::CMouse()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CMouse::~CMouse()
{
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL);

	m_pDevice->SetDataFormat(&c_dfDIMouse2); // マウス用のデータ・フォーマットを設定

	m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
	m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// 入力制御開始
	m_pDevice->Acquire();

	// 代入
	m_hWnd = hWnd;

	// カーソルを表示
	ShowCursor(TRUE);

	m_hCursor[CUR_PEN] = LoadCursorFromFile("data/TEXTURE/CUR/pen.cur"); 
	m_hCursor[CUR_ERASER] = LoadCursorFromFile("data/TEXTURE/CUR/eraser.cur");

	m_type = CURTYPE::CUR_NONE;

	m_bDisp = false;

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------------------------------------
void CMouse::Update(void)
{
	// ボタンの初期化
	m_mouse.state.rgbButtons[0] = 0;

	// マウスの座標を取得
	GetCursorPos(&m_mouse.point);
	ScreenToClient(m_hWnd, &m_mouse.point);

	// 値の更新
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_mouse.state)))
	{
		for (int nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
		{
			// 排他的論理和かどうか判断
			m_aButtonStateTrigger[nCntButton] = (m_aButtonState[nCntButton] ^ m_mouse.state.rgbButtons[nCntButton]) & m_mouse.state.rgbButtons[nCntButton];
			m_aButtonStateRelease[nCntButton] = (m_aButtonState[nCntButton] ^ m_mouse.state.rgbButtons[nCntButton]) & m_aButtonState[nCntButton];
			// キープレス情報保存
			m_aButtonState[nCntButton] = m_mouse.state.rgbButtons[nCntButton];
		}
	}
	else
	{
 		m_pDevice->Acquire();
	}

}

//-------------------------------------------------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------------------------------------------------
void CMouse::Draw(void)
{
	if (m_type == CURTYPE::CUR_NONE ||
		m_bDisp == false)
	{
		return;
	}
	// カーソルハンドルの設定
	SetCursor(m_hCursor[m_type]);
}

//-------------------------------------------------------------------------------------------------------------
// スクロールした値を取得する
//-------------------------------------------------------------------------------------------------------------
int CMouse::GetScrollValue(void)
{
	switch (m_scroll)
	{
		MLB_CASE(SCROLL_NONE) return 0;			// スクロールしていない
		MLB_CASE(SCROLL_UP)   return 1;			// 上にスクロール
		MLB_CASE(SCROLL_DOWN) return -1;		// 下にスクロール
		MLB_CASEEND;
	}
	return 0;
}
