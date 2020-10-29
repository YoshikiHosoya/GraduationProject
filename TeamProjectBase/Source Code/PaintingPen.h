//*************************************************************************************************************
//
// 絵用のペン処理[PaintingPen.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _PAINTINGPEN_H_
#define _PAINTINGPEN_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CMouse;
class CPaintingPen
{
public:
	typedef enum
	{
		MODE_NONE = - 1,										// 無し
		MODE_BRUSH,										// ブラシ
		MODE_ERASER,									// 消しゴム
		MODE_MAX										// 最大
	}MODE;

	CPaintingPen() {};									// コンストラクタ
	~CPaintingPen() {};									// デストラクタ

	void Init(void);									// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新

	void PaintCol(D3DXCOLOR *pCol);						// 色を塗る

	void RayCalculation(CMouse *pMouse);				// レイの算出
	void PosCalculation(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPlaneNor);// 位置を算出

	static CPaintingPen *Create(void);					// 生成

	// 設定関数
	void SetCapsule(void);
	inline void SetPosOld(void) { m_posOld = m_pos; }
	inline void SetMode(MODE mode) { m_mode = mode; }
	// 取得関数
	inline FLOAT2 *GetPos(void) { return &m_pos; }				// 位置の取得
	inline FLOAT2 *GetPosOld(void) { return &m_posOld; }		// 前回位置の取得
	inline CAPSULE_2D *GetCapsule(void) { return &m_Capsule; }	// カプセルの取得

private:
	void PaintBrush(D3DXCOLOR *pCol);					// ブラシで塗る
	void PaintEraser(D3DXCOLOR *pCol);					// 消しゴムで塗る
	D3DXMATRIX*  m_pMtxView;							// マトリックスヴュー
	D3DXMATRIX*  m_pMtxProj;							// プロジェクションマトリックス
	INTEGER2     m_ScreenPos;							// スクリーン座標
	bool         m_bPaint;								// 塗るフラグ
	MODE         m_mode;								// モード
	FLOAT3       m_NearPos;								// 近い位置
	FLOAT3       m_FarPos;								// 遠い位置
	VEC3         m_Ray;									// レイ
	FLOAT2       m_pos;									// 位置
	FLOAT2       m_posOld;								// 前回の位置
	CAPSULE_2D   m_Capsule;								// カプセル情報

};


#endif