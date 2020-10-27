//------------------------------------------------------------------------------
//
//モジュールの処理  [module.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_parts_No1_SymbolKey.h"
#include "renderer.h"
#include "manager.h"
#include "modelinfo.h"
#include "particle.h"
#include "timer.h"
#include "scene3D.h"
#include "keyboard.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define KEYPAD_LIGHT_OFFSET					(D3DXVECTOR3(0.0f,1.0f,-5.0f))
#define KEYPAD_LIGHTPOLYGON_SIZE			(D3DXVECTOR3(10.0f,10.0f,0.0f))
#define KEYPAD_SYMBOL_OFFSET				(D3DXVECTOR3(0.0f,-2.0f,-7.0f))
#define KEYPAD_SYMBOLPOLYGON_SIZE			(D3DXVECTOR3(10.0f,10.0f,0.0f))
#define KEYPAD_LIGHT_REDLIGHTING_TIME		(90)

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_Parts_No1_SymbolKey::CModule_Parts_No1_SymbolKey()
{
	m_KeyPadState = KEYPAD_STATE::NORMAL;
	m_nCntLampCnt = 0;
	m_pLight.reset();
	m_pSymbol.reset();
	m_nSymbolNum = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_Parts_No1_SymbolKey::~CModule_Parts_No1_SymbolKey()
{
	m_pLight.reset();
	m_pSymbol.reset();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_Parts_No1_SymbolKey::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULEPARTS_NO1_KEYPAD));

	//パッドのライト生成
	m_pLight = CSceneBase::ScenePolygonCreateSelfManagement<CScene3D>(KEYPAD_LIGHT_OFFSET, KEYPAD_LIGHTPOLYGON_SIZE, BlackColor, nullptr);

	//マトリックス設定
	m_pLight->SetParentMtxPtr(GetMtxWorldPtr());

	//ライティングoff
	m_pLight->SetLighting(false);

	//文字の生成
	m_pSymbol = CSceneBase::ScenePolygonCreateShared<CScene3D>(KEYPAD_SYMBOL_OFFSET, KEYPAD_SYMBOLPOLYGON_SIZE, WhiteColor, CTexture::GetSeparateTexture(CTexture::SEPARATE_TEX_MODULEPARTS_MODULE00), CScene::OBJTYPE_MODULE_PARTS_SYMBOL);
	m_pSymbol->SetParentMtxPtr(GetMtxWorldPtr());

	CSceneX::Init();

	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CModule_Parts_No1_SymbolKey::Uninit()
{
	CSceneX::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_Parts_No1_SymbolKey::Update()
{
	D3DXVECTOR3 vec;

	switch (m_KeyPadState)
	{
	case CModule_Parts_No1_SymbolKey::KEYPAD_STATE::NORMAL:
		break;
	case CModule_Parts_No1_SymbolKey::KEYPAD_STATE::FAILED:
		m_nCntLampCnt--;

		if (m_nCntLampCnt < 0)
		{
			//クリア済なら緑、そうでないなら消す
			GetClearFlag() ?
				SetKeypadState(KEYPAD_STATE::CLEAR) :
				SetKeypadState(KEYPAD_STATE::NORMAL);
		}
		break;
	}

	//nullcheck
	if (m_pLight)
	{
		//ライトの更新
		m_pLight->Update();
	}

	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_Parts_No1_SymbolKey::Draw()
{
	if (m_pLight)
	{
		m_pLight->Draw();
	}
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Parts_No1_SymbolKey::ShowDebugInfo()
{
#ifdef _DEBUG


#endif //DEBUG
}

//------------------------------------------------------------------------------
//シンボルの設定
//------------------------------------------------------------------------------
void CModule_Parts_No1_SymbolKey::SetSymbol(int nSymbol)
{
	m_nSymbolNum = nSymbol;

	m_pSymbol->SetAnimation(CHossoLibrary::CalcUV_StaticFunc(nSymbol, CTexture::SEPARATE_TEX_MODULEPARTS_MODULE00), CTexture::GetSparateTex_UVSize(CTexture::SEPARATE_TEX_MODULEPARTS_MODULE00));
}

//------------------------------------------------------------------------------
//ランプの状態切り替え
//------------------------------------------------------------------------------
void CModule_Parts_No1_SymbolKey::SetKeypadState(KEYPAD_STATE keypadstate)
{
	//同じステートだった時はreturn
	if (m_KeyPadState == keypadstate)
	{
		return;
	}

	//ステート切り替え
	m_KeyPadState = keypadstate;

	//切り替わったステートに応じて処理
	switch (m_KeyPadState)
	{
	case CModule_Parts_No1_SymbolKey::KEYPAD_STATE::NORMAL:
		m_pLight->SetColor(BlackColor);
		break;
	case CModule_Parts_No1_SymbolKey::KEYPAD_STATE::FAILED:
		m_pLight->SetColor(RedColor);
		m_nCntLampCnt = KEYPAD_LIGHT_REDLIGHTING_TIME;
		break;
	case CModule_Parts_No1_SymbolKey::KEYPAD_STATE::CLEAR:
		m_pLight->SetColor(GreenColor);
		SetClearFlag(true);
		break;

	}
}
