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
		MODE_NONE = - 1,											// 無し
		MODE_BRUSH,													// ブラシ
		MODE_ERASER,												// 消しゴム
		MODE_MAX													// 最大
	}MODE;

	CPaintingPen() {};												// コンストラクタ
	~CPaintingPen() {};												// デストラクタ

	void Init(void);												// 初期化
	void Uninit(void);												// 終了
	void Update(void);												// 更新

	void PaintCol(D3DXCOLOR *pCol);									// 色を塗る

	void PosCalculation(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPlaneNor);	// 位置を算出

	static CPaintingPen *Create(void);								// 生成

	// 設定関数
	void SetCapsule(void);											// カプセルの設定
	inline void SetPosOld(void) { m_posOld = m_pos; }				// 前回位置の設定
	inline void SetMode(MODE mode) { m_mode = mode; }				// モードの設定

	// 取得関数
	inline FLOAT2 *GetPos(void) { return &m_pos; }					// 位置の取得
	inline FLOAT2 *GetPosOld(void) { return &m_posOld; }			// 前回位置の取得
	inline CAPSULE_2D *GetCapsule(void) { return &m_Capsule; }		// カプセルの取得
	inline MODE &GetMode(void) { return m_mode; }					// モードの取得

private:
	/* メンバ関数 */
	void PaintBrush(D3DXCOLOR *pCol);								// ブラシで塗る
	void PaintEraser(D3DXCOLOR *pCol);								// 消しゴムで塗る

	/* メンバ変数 */
	D3DXMATRIX*  m_pMtxView;										// マトリックスヴュー
	D3DXMATRIX*  m_pMtxProj;										// プロジェクションマトリックス
	bool         m_bPaint;											// 塗るフラグ
	MODE         m_mode;											// モード
	FLOAT2       m_pos;												// 位置
	FLOAT2       m_posOld;											// 前回の位置
	CAPSULE_2D   m_Capsule;											// カプセル情報
};


#endif