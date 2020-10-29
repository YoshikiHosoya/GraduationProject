//------------------------------------------------------------------------------
//
//複数桁の数字の処理  [multinumber.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MULTINUMBER_H_
#define _MULTINUMBER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scenebase.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CSceneBase;

class CMultiNumber : public CSceneBase
{
public:

	//ナンバーのエフェクト
	enum class NUMBER_TYPE
	{
		NUMBER_2D,							//2D
		NUMBER_3D,							//3D

	};
	//ナンバーのエフェクト
	enum class NUMBER_EFFECT
	{
		NONE,								//エフェクト無し
		FLASHING,							//点滅 表示非表示
		FLASHING_RED_YELLOW,				//点滅 赤黄色

	};

	CMultiNumber();
	~CMultiNumber();
	virtual HRESULT Init()						override;				//初期化
	virtual void Uninit()						override;				//終了
	virtual void Update()						override;				//更新
	virtual void Draw()							override;				//描画
	virtual void ShowDebugInfo()				override;				//デバッグ情報表記
	void SetPos(D3DXVECTOR3 const &pos)			override;				//座標設定
	void SetSize(D3DXVECTOR3 const &size)		override;				//サイズ設定
	void SetColor(D3DXCOLOR const &col)			override;				//色設定

	static S_ptr<CMultiNumber> Create(D3DXVECTOR3 const &pos, D3DXVECTOR3 const &onesize,
		int const nValue, int const  nDigits, NUMBER_TYPE const type, CScene::OBJTYPE const objtype);	//生成処理

	//Set関数
	void SetMultiNumber(int nValue);										//スコア設定処理
	void Settype(NUMBER_EFFECT const effect);								//種類設定

	const NUMBER_EFFECT GetNumberType() { return m_NumberEffect; };			//種類取得

private:
	Vec<U_ptr<CSceneBase>> m_pNumberList;					//ナンバーのポインタのリスト
	NUMBER_EFFECT m_NumberEffect;											//種類
	int m_nValue;															//値
	int m_nCnt;																//点滅カウント

};

#endif