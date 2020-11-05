//------------------------------------------------------------------------------
//
//モジュールのランプ  [module_parts_lamp.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_parts_lamp.h"
#include "renderer.h"
#include "manager.h"
#include "modelinfo.h"
#include "particle.h"
#include "timer.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define LAMP_OFFSET (D3DXVECTOR3(0.0f,0.0f,-15.0f))

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_Parts_Lamp::CModule_Parts_Lamp()
{
	m_LampState = LAMP_STATE::OFF;
	m_nCntLampCnt = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_Parts_Lamp::~CModule_Parts_Lamp()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_Parts_Lamp::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULEPARTS_CLEARLAMP));

	CSceneX::Init();
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_Parts_Lamp::Update()
{
	D3DXVECTOR3 vec;

	switch (m_LampState)
	{
	case CModule_Parts_Lamp::LAMP_STATE::OFF:
		break;
	case CModule_Parts_Lamp::LAMP_STATE::RED:
		m_nCntLampCnt--;

		//赤いパーティクル生成
		CParticle::CreateFromText(*D3DXVec3TransformCoord(&vec,&LAMP_OFFSET, GetMtxWorldPtr()), ZeroVector3, CParticleParam::EFFECT_LED_LIGHT, RedColor);

		if (m_nCntLampCnt < 0)
		{
			//クリア済なら緑、そうでないなら消す
			GetClearFlag() ?
				SetLampState(LAMP_STATE::GREEN) :
				SetLampState(LAMP_STATE::OFF);
		}
		break;
	case CModule_Parts_Lamp::LAMP_STATE::GREEN:

		//緑のパーティクル生成
 		CParticle::CreateFromText(*D3DXVec3TransformCoord(&vec, &LAMP_OFFSET, GetMtxWorldPtr()), ZeroVector3, CParticleParam::EFFECT_LED_LIGHT, GreenColor);
		break;
	}
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_Parts_Lamp::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Parts_Lamp::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//ランプの状態切り替え
//------------------------------------------------------------------------------
void CModule_Parts_Lamp::SetLampState(LAMP_STATE lampstate)
{
	//同じステートだった時はreturn
	if (m_LampState == lampstate)
	{
		return;
	}

	//ステート切り替え
	m_LampState = lampstate;

	//切り替わったステートに応じて処理
	switch (lampstate)
	{
	case CModule_Parts_Lamp::LAMP_STATE::OFF:
		break;
	case CModule_Parts_Lamp::LAMP_STATE::RED:
		m_nCntLampCnt = 90;
		break;
	case CModule_Parts_Lamp::LAMP_STATE::GREEN:
		SetClearFlag(true);
		break;

	}
}
