//==========================================================
// SceneBase処理：NORI
//==========================================================
//==========================================================
// インクルードファイル
//==========================================================
#include "SceneBase.h"
#include "renderer.h"
#include "manager.h"

//==========================================================
// コンストラクタ
//==========================================================
CSceneBase::CSceneBase()
{
	m_pos = ZeroVector3;
	m_size = ZeroVector3;
	m_col = WhiteColor;
	m_rot = ZeroVector3;
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_bDisp = true;
	D3DXMatrixIdentity(&m_mtxWorld);

}

//==========================================================
// デストラクタ
//==========================================================
CSceneBase::~CSceneBase()
{
	//nullcheck
	if (m_pVtxBuff)
	{
		//頂点バッファの開放
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}
