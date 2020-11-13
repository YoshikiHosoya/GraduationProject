//------------------------------------------------------------------------------
//
//モジュールパーツのキーパッド  [module_parts_No1_ShapeKey.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_parts_No2_Wire.h"
#include "module_No2_LampAndWire.h"
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

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_Parts_No2_Wire::CModule_Parts_No2_Wire()
{
	m_Wire = CModule_No2_LampAndWire::WIRE::NONE;
	m_WireColor = MagentaColor;
	m_bCut = false;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_Parts_No2_Wire::~CModule_Parts_No2_Wire()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_Parts_No2_Wire::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULEPARTS_NO3_WIRE));

	CSceneX::Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_Parts_No2_Wire::Update()
{
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_Parts_No2_Wire::Draw()
{
	//ハードエッジ描画
	//選択されているモデルのみ
	CSceneX::DrawHardEdgeStencil();

	//ワールドマトリックスの計算
	CSceneX::CalcMtx_IncludeParentMtx();

	//メッシュ描画　マテリアル色を設定
	CSceneX::DrawMesh_SetMaterial(m_WireColor, false, { 2 });

	//CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Parts_No2_Wire::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}
//------------------------------------------------------------------------------
//ワイヤーカラー設定
//------------------------------------------------------------------------------
void CModule_Parts_No2_Wire::SetWire(CModule_No2_LampAndWire::WIRE wire)
{
	m_Wire = wire;

	switch (m_Wire)
	{

	case CModule_No2_LampAndWire::RED:
		m_WireColor = RedColor;
		break;
	case CModule_No2_LampAndWire::BLUE:
		m_WireColor = BlueColor;
		break;
	case CModule_No2_LampAndWire::GREEN:
		m_WireColor = GreenColor;
		break;
	case CModule_No2_LampAndWire::YELLOW:
		m_WireColor = YellowColor;
		break;
	case CModule_No2_LampAndWire::WHITE:
		m_WireColor = WhiteColor;
		break;
	case CModule_No2_LampAndWire::BLACK:
		m_WireColor = BlackColor;
		break;


	}


}
