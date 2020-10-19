//-----------------------------------------------------------------------------
//
// ライト処理 [light.h]
// Author : Yoshiki Hosoya
//
//-----------------------------------------------------------------------------
#ifndef _LIGHT_H_
#define _LIGHT_H_

#define _CRT_SECURE_NO_WARNINGS

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define MAX_LIGHT (3)
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CLight
{
public:
	CLight();
	~CLight();
	void Init();								//初期化
	void Uninit();								//終了
	void Update();								//更新
	void ShowLightInfo();						//imgui ライト情報表示

	//Set関数
	void SetBlackLight();
	void SetDefaultLight();

	//Get関数
	D3DXVECTOR3 GetLightDir(int nLightNum)		{ return m_vecDir[nLightNum]; };				//ライトの方向取得　引数のライトを貰う
	D3DXCOLOR GetLightColor(int nLightNum)		{ return m_LightCol[nLightNum]; };		//ライトの方向取得　引数のライトを貰うけでいい

private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];					//ライト
	D3DXVECTOR3 m_vecDir[MAX_LIGHT];				//ライトの向き
	D3DXCOLOR m_LightCol[MAX_LIGHT];				//現在のライトの色
	D3DXCOLOR m_DefaultLightCol[MAX_LIGHT];			//ライトの初期色

};

#endif