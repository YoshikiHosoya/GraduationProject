//----------------------------------------------------------------------------
//
// テクスチャアニメーション用の基底クラス [TexAnimationBase.h]
// Author : Yoshiki Hosoya
//
//----------------------------------------------------------------------------
#ifndef _TEXANIMATION_BASE_H_
#define _TEXANIMATION_BASE_H_
//----------------------------------------------------------------------------
// インクルードファイル
//----------------------------------------------------------------------------
#include "main.h"

//----------------------------------------------------------------------------
// マクロ定義
//----------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CSceneBase;

class CTexAnimationBase
{
public:
	CTexAnimationBase();									//コンストラクタ
	~CTexAnimationBase();									//デストラクタ

	bool UpdateAnimation();			//更新
	D3DXVECTOR2 CalcUV();

	//Set関数
	void SetTex(CTexture::SEPARATE_TEX_TYPE Tex)		{ m_EffectTex = Tex; };				//テクスチャ設定
	void SetLife(int nLife)								{ m_nLife = nLife; };				//ライフ
	void SetCntSwitch(int nCntSwitch)					{ m_nCntSwitch = nCntSwitch; };		//切替のフレーム
	void SetLoop(bool bLoop)							{ m_bLoop = bLoop; };				//ループするか

	//Get関数
	CTexture::SEPARATE_TEX_TYPE GetEffectTex()			{ return m_EffectTex; };			//使ってるテクスチャ取得
	int GetLife()										{ return m_nLife; };				//ライフ
	int GetCntSwitch()									{ return m_nCntSwitch; };			//切替のフレーム
	int GetLoop()										{ return m_bLoop; };				//ループするか

	int GetCntAnim()									{ return m_nCntAnim; };				//アニメーションカウンタ
	int GetPatternAnim()								{ return m_nPatternAnim; };			//アニメーションパターン
	bool GetEndFlag()									{ return m_bEnd; };					//終了フラグ
private:
	CTexture::SEPARATE_TEX_TYPE m_EffectTex;				//エフェクトのタイプ
	int m_nLife;											//ライフ　アニメーション終了で消えないエフェクト（ループとか）用
	int m_nCntAnim;											//アニメーションカウンター
	int m_nPatternAnim;										//アニメーションパターン
	int m_nCntSwitch;										//アニメーション切替のフレーム
	bool m_bLoop;											//アニメーションループするか
	bool m_bEnd;											//終了フラグ

};

#endif