//------------------------------------------------------------------------------
//
//モジュールの処理  [module.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_parts_No2_ShapeKey.h"
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
#define KEYPAD_SYMBOL_OFFSET				(D3DXVECTOR3(0.0f,0.0f,-7.0f))
#define KEYPAD_SYMBOLPOLYGON_SIZE			(D3DXVECTOR3(12.0f,12.0f,0.0f))
#define KEYPAD_LIGHT_REDLIGHTING_TIME		(90)

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_Parts_No2_ShapeKey::CModule_Parts_No2_ShapeKey()
{
	m_pShape.reset();
	m_Shape = CModule_No2_ShapeKeyPad::SHAPE::NONE;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_Parts_No2_ShapeKey::~CModule_Parts_No2_ShapeKey()
{
	m_pShape.reset();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_Parts_No2_ShapeKey::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULEPARTS_NO2_KEYPAD));

	//文字の生成
	m_pShape = CSceneBase::ScenePolygonCreateShared<CScene3D>(KEYPAD_SYMBOL_OFFSET, KEYPAD_SYMBOLPOLYGON_SIZE, WhiteColor,
		CTexture::GetSeparateTexture(CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01), CScene::OBJTYPE_MODULE_PARTS_SYMBOL);

	//親マトリックス設定
	m_pShape->SetParentMtxPtr(GetMtxWorldPtr());

	CSceneX::Init();

	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CModule_Parts_No2_ShapeKey::Uninit()
{
	CSceneX::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_Parts_No2_ShapeKey::Update()
{
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_Parts_No2_ShapeKey::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Parts_No2_ShapeKey::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//シンボルの設定
//------------------------------------------------------------------------------
void CModule_Parts_No2_ShapeKey::SetShape(CModule_No2_ShapeKeyPad::SHAPE shape)
{
	// Shape設定
	m_Shape = shape;

	//UV設定
	m_pShape->SetAnimation(CHossoLibrary::CalcUV_StaticFunc((int)shape, CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01),
							CTexture::GetSparateTex_UVSize(CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01));
}
