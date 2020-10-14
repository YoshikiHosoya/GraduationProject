//------------------------------------------------------------------------------
//
//キーボード処理  [keyboard.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "keyboard.h"
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CKeyboard::CKeyboard()
{
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CKeyboard::~CKeyboard()
{
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance ,hWnd);

	// 入力デバイス（キーボード）の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		//失敗
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		//失敗
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		//失敗
		return E_FAIL;
	}
	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevKeyboard->Acquire();

	//成功
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CKeyboard::Uninit()
{
	CInput::Uninit();

	// 入力デバイス(キーボード)の開放
	if (m_pDevKeyboard != NULL)
	{
		m_pDevKeyboard->Unacquire();
		m_pDevKeyboard->Release();
		m_pDevKeyboard = NULL;
	}
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CKeyboard::Update()
{
	//変数宣言
	BYTE aKeyState[NUM_KEY_MAX];	//キー数分

	// デバイスからデータを取得
	if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCntkey = 0; nCntkey < NUM_KEY_MAX; nCntkey++)
		{
			//トリガー処理
			m_aKeyStateTrigger[nCntkey] = (aKeyState[nCntkey] ^ m_aKeyState[nCntkey]) & aKeyState[nCntkey];

			m_aKeyState[nCntkey] = aKeyState[nCntkey];
		}
	}
	else
	{
		m_pDevKeyboard->Acquire();
	}
}
//------------------------------------------------------------------------------
//プレス取得処理
//------------------------------------------------------------------------------
bool CKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}
//------------------------------------------------------------------------------
//トリガー取得処理
//------------------------------------------------------------------------------
bool CKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
