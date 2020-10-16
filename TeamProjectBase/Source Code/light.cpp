//-----------------------------------------------------------------------------
//
// ライト処理 [light.cpp]
// Author : Yosihiki Hosoya
//
//-----------------------------------------------------------------------------
#include "light.h"
#include "manager.h"
#include "renderer.h"
//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CLight::CLight()
{
}
//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CLight::~CLight()
{
}
//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------

void CLight::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	m_vecDir[0] = D3DXVECTOR3(0.20f, -0.80f, 0.40f);
	m_vecDir[1] = D3DXVECTOR3(-0.20f, 0.80f, -0.40f);
	m_vecDir[2] = D3DXVECTOR3(0.10f, -0.10f, -0.40f);


	m_LightCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightCol[1] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	m_LightCol[2] = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);


	//---------------------------------------------
	// ライト0の設定
	//---------------------------------------------

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ライトをクリアする
		ZeroMemory(&m_aLight[nCnt], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_aLight[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		m_aLight[nCnt].Diffuse = m_LightCol[nCnt];
		m_aLight[nCnt].Specular = m_LightCol[nCnt];		//スペキュラー(反射光)

		// ライトの方向の設定
		D3DXVec3Normalize(&m_vecDir[nCnt], &m_vecDir[nCnt]);		//正規化
		m_aLight[nCnt].Direction = m_vecDir[nCnt];

		// ライトを設定する
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);
		// ライトを有効にする
		pDevice->LightEnable(nCnt, TRUE);
	}
}
//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CLight::Uninit()
{
}
//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CLight::Update()
{

}
//-----------------------------------------------------------------------------
// ライトの情報
//-----------------------------------------------------------------------------
void CLight::ShowLightInfo()
{


	if (ImGui::CollapsingHeader("LightInfo"))
	{
		char aName[MAX_TEXT] = {};
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

		for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
		{

			sprintf(aName, "Light [%d]", nCnt);

			if (ImGui::TreeNode(aName))
			{
				//文字列改良
				sprintf(aName, "Direction [%d]", nCnt);
				ImGui::SliderFloat3(aName, m_vecDir[nCnt], -1.0f, 1.0f);

				//色のポインタ
				float *rCol = m_LightCol[nCnt];

				//色の設定
				sprintf(aName, "Color [%d]", nCnt);
				ImGui::ColorEdit4(aName, rCol);

				//正規化
				D3DXVec3Normalize(&m_vecDir[nCnt], &m_vecDir[nCnt]);

				//ライトの変数に代入
				m_aLight[nCnt].Direction = m_vecDir[nCnt];

				//ライト設定
				pDevice->SetLight(nCnt, &m_aLight[nCnt]);
				ImGui::TreePop();
			}
		}
	}
}
//-----------------------------------------------------------------------------
// ライトの向き取得
//-----------------------------------------------------------------------------
D3DXVECTOR3 CLight::GetLightDir(int nLightNum)
{
	return m_vecDir[nLightNum];
}
//-----------------------------------------------------------------------------
// ライト色取得
//-----------------------------------------------------------------------------
D3DXCOLOR CLight::GetLightColor(int nLightNum)
{
	return m_LightCol[nLightNum];
}
