//------------------------------------------------------------------------------
//
//モジュールのキーパッド処理  [module_keypad.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_keypad.h"
#include "module_parts_key.h"
#include "renderer.h"
#include "manager.h"
#include "modelinfo.h"
#include "timer.h"
#include "keyboard.h"
#include "Mylibrary.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define MAX_KEYPAD (9)
#define KEYPAD_INTERVAL (D3DXVECTOR3(18.0f,18.0f,0.0f))
#define KEYPAD_OFFSET (D3DXVECTOR3(-10.0f,-10.0f,-15.0f))

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_KeyPad::CModule_KeyPad()
{
	m_pKeyPadList = {};
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_KeyPad::~CModule_KeyPad()
{
	m_pKeyPadList.clear();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_KeyPad::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_DEFAULT));

	//ランプ生成
	CModule_Base::CreateLamp();

	//キーパッド生成
	CreateKeyPad();

	CSceneX::Init();
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CModule_KeyPad::Uninit()
{
	CSceneX::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_KeyPad::Update()
{
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_KeyPad::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_KeyPad::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}
//------------------------------------------------------------------------------
//キーパッド操作
//------------------------------------------------------------------------------
void CModule_KeyPad::Operation()
{
	static int nSelectNum = 0;

	//入力が無かった時はbreak
	CHossoLibrary::Selecting(nSelectNum, 3, 3);

	for (int nCnt = 0; nCnt < m_pKeyPadList.size(); nCnt++)
	{
		//nullcheck
		if (m_pKeyPadList[nCnt].get())
		{
			//現在の選択番号と同じモノだけtrueにしておく
			nCnt == nSelectNum ?
				m_pKeyPadList[nCnt]->SetSelect(true) :
				m_pKeyPadList[nCnt]->SetSelect(false);
		}
	}
}
//------------------------------------------------------------------------------
//キーパッド生成
//------------------------------------------------------------------------------
void CModule_KeyPad::CreateKeyPad()
{
	for (int nCnt = 0; nCnt < MAX_KEYPAD; nCnt++)
	{
		INTEGER2 Value;

		Value.nX = nCnt % 3;
		Value.nY = nCnt / 3;

		m_pKeyPadList.emplace_back(CModule_Parts_Base::Create_ModuleParts<CModule_Parts_Key>
			(KEYPAD_OFFSET + D3DXVECTOR3(	-KEYPAD_INTERVAL.x + KEYPAD_INTERVAL.x * Value.nX,
											KEYPAD_INTERVAL.y - KEYPAD_INTERVAL.y * Value.nY, 0.0f), GetMtxWorldPtr()));

	}
}
