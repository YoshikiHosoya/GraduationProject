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

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------------------------------------
void CMouse::Update(void)
{
	// ボタンの初期化
	m_mouse.mouseState.rgbButtons[0] = 0;

	// マウスの座標を取得
	GetCursorPos(&m_mouse.point);
	ScreenToClient(m_hWnd, &m_mouse.point);

	// 値の更新 
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_mouse.mouseState)))
	{
		for (int nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
		{
			// 排他的論理和かどうか判断
			m_aButtonStateTrigger[nCntButton] = (m_aButtonState[nCntButton] ^ m_mouse.mouseState.rgbButtons[nCntButton]) & m_mouse.mouseState.rgbButtons[nCntButton];
			m_aButtonStateRelease[nCntButton] = (m_aButtonState[nCntButton] ^ m_mouse.mouseState.rgbButtons[nCntButton]) & m_aButtonState[nCntButton];
			// キープレス情報保存
			m_aButtonState[nCntButton] = m_mouse.mouseState.rgbButtons[nCntButton];
		}
	}
	else
	{
 		m_pDevice->Acquire();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------------------------------------------------
void CMouse::Uninit(void)
{
	CInput::Uninit();
}
