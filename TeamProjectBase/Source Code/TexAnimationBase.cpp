//------------------------------------------------------------------------------
//
//テクスチャアニメーションの基底クラス  [TexAnimationBase.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "manager.h"
#include "TexAnimationBase.h"
#include "SceneBase.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CTexAnimationBase::CTexAnimationBase()
{
	//初期化
	m_nPatternAnim = 0;
	m_nCntAnim = 0;
	m_nCntSwitch = 1;
	m_nLife = 120;
	m_bLoop = false;
	m_EffectTex = CTexture::SEPARATE_TEX_EFFECT_EXPLOSION00;
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CTexAnimationBase::~CTexAnimationBase()
{
}

//------------------------------------------------------------------------------
//アニメーションの更新
//パターンに変更があった時はtrueを返す
//返した先でSetAnimation関数読んで
//------------------------------------------------------------------------------
bool CTexAnimationBase::UpdateAnimation()
{
	if (m_nLife-- <= 0 || m_nCntSwitch == 0)
	{
		//終了フラグtrue
		m_bEnd = true;

		//終了
		return false;
	}
	//カウンターを進める
	m_nCntAnim++;

	//2フレームごとに
	if (m_nCntAnim % m_nCntSwitch == 0)
	{
		//パターンを進める
		m_nPatternAnim++;

		//最後のパターンまで進んだ時
		if (m_nPatternAnim >= CTexture::GetSparateTex_UVCnt(m_EffectTex).x *  CTexture::GetSparateTex_UVCnt(m_EffectTex).y)
		{
			m_bLoop ?
				m_nPatternAnim = 0 :		//ループする時		パターン最初に戻す
				m_bEnd = true;				//ループしない時　	フラグを立てる
		}

		//アニメーション更新
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
//UV計算
//------------------------------------------------------------------------------
D3DXVECTOR2 CTexAnimationBase::CalcUV()
{
	//UV座標計算
	D3DXVECTOR2 UV;
	UV.x = m_nPatternAnim % (int)CTexture::GetSparateTex_UVCnt(m_EffectTex).x * CTexture::GetSparateTex_UVSize(m_EffectTex).x;
	UV.y = m_nPatternAnim / (int)CTexture::GetSparateTex_UVCnt(m_EffectTex).x * CTexture::GetSparateTex_UVSize(m_EffectTex).y;

	//return
	return UV;
}

