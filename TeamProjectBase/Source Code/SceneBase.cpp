//------------------------------------------------------------------------------
//
//Sceneのベースクラス [SceneBase.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//インクルード
//-----------------------------------------------------------------------------
#include "SceneBase.h"
//-----------------------------------------------------------------------------
//コンストラクタ
//-----------------------------------------------------------------------------
CSceneBase::CSceneBase()
{
	//初期化
	m_pos = ZeroVector3;
	m_size = ZeroVector3;
	m_rot = ZeroVector3;
	m_col = WhiteColor;
	m_scale = OneVector3;
	m_bLighting = true;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pParentMtx = nullptr;
	m_bDisp = true;
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
}

//-----------------------------------------------------------------------------
//デストラクタ
//-----------------------------------------------------------------------------
CSceneBase::~CSceneBase()
{
	//nullcheck
	if (m_pVtxBuff)
	{
		//頂点バッファ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//-----------------------------------------------------------------------------
//マトリックスの計算　親のマトリックスの合成も含む
//-----------------------------------------------------------------------------
void CSceneBase::CalcMtx_IncludeParentMtx()
{
	//マトリックス計算
	CHossoLibrary::CalcMatrix(GetMtxWorldPtr(), GetPos(), GetRot(), GetScale());

	//nullcheck
	if (GetParentMtxPtr())
	{
		//親のマトリックスを掛け合わせる
		*GetMtxWorldPtr() *= *GetParentMtxPtr();
	}

}
