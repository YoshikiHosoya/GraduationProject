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
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	//ライトの向き設定
	m_vecDir[0] = D3DXVECTOR3(0.20f, -0.80f, 0.40f);
	m_vecDir[1] = D3DXVECTOR3(-0.20f, 0.80f, -0.40f);
	m_vecDir[2] = D3DXVECTOR3(0.10f, -0.10f, -0.40f);

	//ライトの色設定
	m_DefaultLightCol[0] = m_LightCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_DefaultLightCol[1] = m_LightCol[1] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	m_DefaultLightCol[2] = m_LightCol[2] = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ライトの設定
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		//ライトをクリアする
		ZeroMemory(&m_aLight[nCnt], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		m_aLight[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		m_aLight[nCnt].Diffuse = m_LightCol[nCnt];		//ディフューズ拡散光
		m_aLight[nCnt].Specular = m_LightCol[nCnt];		//スペキュラー(反射光)

		//正規化
		D3DXVec3Normalize(&m_vecDir[nCnt], &m_vecDir[nCnt]);

		//ライトの方向の設定
		m_aLight[nCnt].Direction = m_vecDir[nCnt];

		//ライトの情報を設定する
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);

		//ライトを有効にする
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
// ライトのデバッグ用の情報表記
//-----------------------------------------------------------------------------
void CLight::ShowLightInfo()
{
	if (ImGui::CollapsingHeader("LightInfo"))
	{
		char aName[MAX_TEXT] = {};
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

		//ライトの個数分
		for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
		{
			//文字列生成
			//同じ文字列だとImGuiがうまく動かない
			sprintf(aName, "Light [%d]", nCnt);

			//ツリー
			if (ImGui::TreeNode(aName))
			{
				//ライトの向き設定
				sprintf(aName, "Direction [%d]", nCnt);
				if (ImGui::SliderFloat3(aName, m_vecDir[nCnt], -1.0f, 1.0f))
				{
					//正規化
					D3DXVec3Normalize(&m_vecDir[nCnt], &m_vecDir[nCnt]);

					//ライトの変数に代入
					m_aLight[nCnt].Direction = m_vecDir[nCnt];
				}

				//色のポインタ
				float *rCol = m_LightCol[nCnt];

				//色の設定
				sprintf(aName, "Color [%d]", nCnt);
				if (ImGui::ColorEdit4(aName, rCol))
				{
					//ライトの変数に代入
					m_aLight[nCnt].Diffuse = m_LightCol[nCnt];
					m_aLight[nCnt].Specular = m_LightCol[nCnt];		//スペキュラー(反射光)
				}

				//ライト設定
				pDevice->SetLight(nCnt, &m_aLight[nCnt]);
				ImGui::TreePop();
			}
		}
	}
}
//-----------------------------------------------------------------------------
// ライトを全部黒く設定
//-----------------------------------------------------------------------------
void CLight::SetBlackLight()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライトの設定
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		//ライトの色を上書き
		m_aLight[nCnt].Specular = m_aLight[nCnt].Diffuse = m_LightCol[nCnt] = BlackColor;

		//ライトの情報を設定する
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);
	}
}
//-----------------------------------------------------------------------------
// ライトを全部デフォルトに戻す
//-----------------------------------------------------------------------------
void CLight::SetDefaultLight()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライトの設定
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		//ライトの色を上書き
		m_aLight[nCnt].Specular = m_aLight[nCnt].Diffuse = m_LightCol[nCnt] = m_DefaultLightCol[nCnt];

		//ライトの情報を設定する
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);
	}
}
