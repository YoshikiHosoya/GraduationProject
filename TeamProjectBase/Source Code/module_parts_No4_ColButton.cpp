//------------------------------------------------------------------------------
//
//モジュールパーツの色のボタン  [module_parts_No4_ColButton.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_parts_No4_ColButton.h"
#include "module_No4_4ColButton.h"
#include "manager.h"
#include "modelinfo.h"
#include "keyboard.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define KEYPAD_SYMBOL_OFFSET				(D3DXVECTOR3(0.0f,0.0f,-6.5f))
#define KEYPAD_SYMBOLPOLYGON_SIZE			(D3DXVECTOR3(12.0f,12.0f,0.0f))
#define KEYPAD_LIGHT_REDLIGHTING_TIME		(90)
#define LIGHT_FLASH_TIME					(30)
#define LIGHT_ADD_COLOR						(D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_Parts_No4_ColButton::CModule_Parts_No4_ColButton()
{
	m_ButtonCol = WhiteColor;
	m_bLight = false;
	m_nCntLight = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_Parts_No4_ColButton::~CModule_Parts_No4_ColButton()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_Parts_No4_ColButton::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULEPARTS_NO4_BUTTON));

	CSceneX::Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_Parts_No4_ColButton::Update()
{
	CSceneX::Update();

	//ライトが点灯しているとき
	if (m_bLight)
	{
		//カウントダウンして0以下になった時
		if (m_nCntLight-- <= 0)
		{
			//ライトOFF
			SetLighting(false);
		}
	}
}
//------------------------------------------------------------------------------
//描画処理
//CSceneXオーバーライド
//------------------------------------------------------------------------------
void CModule_Parts_No4_ColButton::Draw()
{
	//ハードエッジ描画
	//選択されているモデルのみ
	CSceneX::DrawHardEdgeStencil();

	//ワールドマトリックスの計算
	CSceneX::CalcMtx_IncludeParentMtx();

	if (m_bLight)
	{
		//メッシュ描画　マテリアル色を設定
		CSceneX::SetEmissive(true);
		CSceneX::DrawMesh_SetMaterial(m_ButtonCol + LIGHT_ADD_COLOR , true);

	}
	else
	{
		//メッシュ描画　マテリアル色を設定
		CSceneX::SetEmissive(false);
		CSceneX::DrawMesh_SetMaterial(m_ButtonCol, true);
	}

	//CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Parts_No4_ColButton::ShowDebugInfo()
{
#ifdef _DEBUG



#endif //DEBUG
}

//------------------------------------------------------------------------------
//ボタンの色設定
//------------------------------------------------------------------------------
void CModule_Parts_No4_ColButton::SetButtonCol(CModule_No4_4ColButton::BUTTON col)
{
	m_Button = col;

	switch (col)
	{
	case CModule_No4_4ColButton::BUTTON::RED:
		m_ButtonCol = RedColor;
		break;
	case CModule_No4_4ColButton::BUTTON::GREEN:
		m_ButtonCol = GreenColor;
		break;
	case CModule_No4_4ColButton::BUTTON::BLUE:
		m_ButtonCol = BlueColor;
		break;
	case CModule_No4_4ColButton::BUTTON::YELLOW:
		m_ButtonCol = YellowColor;
		break;

	}
}

//------------------------------------------------------------------------------
//ライトの設定
//------------------------------------------------------------------------------
void CModule_Parts_No4_ColButton::SetButtonLighting(bool bLight)
{
	m_bLight = bLight;
	m_nCntLight = LIGHT_FLASH_TIME;
}
