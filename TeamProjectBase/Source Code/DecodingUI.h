//*************************************************************************************************************
//
// 解読書のUI[処理 [DecodingUI.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _DECODINGUI_H_
#define _DECODINGUI_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Mylibrary.h"
#include "DecodingTypes.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CDecodingUI : public CScene
{
public:
	/* 構造体定義 */
	// ポリゴンの頂点情報
	//typedef struct POLYVERTEXS
	//{
	//	union {
	//		struct {
	//			FLOAT3 pos0;
	//			FLOAT3 pos1;
	//			FLOAT3 pos2;
	//			FLOAT3 pos3;
	//		};
	//		FLOAT3 m[4];
	//	};

	//} POLYVERTEXS;

	/* メンバ関数 */
	CDecodingUI();																								// コンストラクタ
	~CDecodingUI();																								// デストラクタ

	static std::shared_ptr<CDecodingUI>Create(SETINGINFO Seting);												// 生成

	HRESULT                   Init();																			//初期化
	void                      Update();																			//更新
	void                      Draw();																			//描画
	void                      ShowDebugInfo() {}																//デバッグ情報表記

	void                      UpdateVertex(bool bPos, bool bCol);												// 頂点情報の更新
	void                      UpdateVertex(D3DXVECTOR3* pPos, D3DXVECTOR2* pSize, D3DXCOLOR *pCol);				// 頂点情報の更新

	// 設定関数
	void                      SetFromSetingInfo(CONST SETINGINFO & Seting);																// 設定用情報から設定する
	inline void               BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }										// テクスチャを結びつける
	inline void               SetPos(CONST D3DXVECTOR3 &pos) { m_pos = pos; }															// 位置の設定
	inline void               SetSize(CONST D3DXVECTOR2 &size) { m_size = size; }														// サイズの設定
	inline void               SetCol(CONST D3DXCOLOR &col) { m_col = col; }																// 色の設定
	inline void               SetAngle(const float &fAngle) { m_fAngle = fAngle; }														// 角度の設定
	inline void               SetOriginType(const ORIGINVERTEXTYPE nOriginType) { m_OriginType = nOriginType; }							// 原点タイプの設定
	inline void               SetDispFlag(const bool bDisp) { m_bDisp = bDisp; }														// 描画フラグの設定
	inline bool               SetWithPosPtr(D3DXVECTOR3 *pPos)   { if (pPos != NULL) { m_pos = *pPos; return true; } return false; }		// 位置ポインタで位置を設定
	inline bool               SetWithSizePtr(D3DXVECTOR2 *pSize) { if (pSize != NULL) { m_size = *pSize; return true; } return false; }	// 大きさポインタで大きさを設定
	inline bool               SetWithColPtr(D3DXCOLOR *pCol)     { if (pCol != NULL) { m_col = *pCol; return true; } return false; }		// 色ポインタで色を設定

	// 取得関数
	inline LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }											// テクスチャの取得
	inline FLOAT3&            GetPos(void) { return m_pos; }													// 位置の取得
	inline FLOAT2&            GetSize(void) { return m_size; }													// 大きさの取得
	inline D3DXCOLOR&         GetCol(void) { return m_col; }													// 色の取得
	inline float&             GetAngle(void) { return m_fAngle; }												// 角度の取得
	inline ORIGINVERTEXTYPE&  GetOriginType(void) { return m_OriginType; }										// 原点タイプの取得
	inline bool               GetDispFlag(void) { return m_bDisp; }												// 描画フラグの取得
	inline FLOAT3*            GetPosPtr(void) { return &m_pos; }												// 位置ポインタの取得
	inline FLOAT2*            GetSizePtr(void) { return &m_size; }												// 大きさポインタの取得
	inline D3DXCOLOR*         GetColPtr(void) { return &m_col; }												// 色ポインタの取得
	inline float*             GetAnglePtr(void) { return &m_fAngle; }											// 角度ポインタの取得
	inline ORIGINVERTEXTYPE*  GetOriginTypePtr(void) { return &m_OriginType; }									// 原点タイプポインタの取得
	inline bool*              GetDispFlagPtr(void) { return &m_bDisp; }											// 描画フラグポインタの取得
private:
	/* メンバ関数 */
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);																	// 頂点情報の作成
	void SetVatexPos(VERTEX_2D *pVtx, ORIGINVERTEXTYPE type, D3DXVECTOR3 * pPos, D3DXVECTOR2 * pSize, float fRotation);	// 頂点位置の設定
	void SetVatexCol(VERTEX_2D *pVtx);																			// 頂点カラーの設定



	/* メンバ変数 */
	static const float      m_fHalfPi;				// 円周率の半分の値
	static const float      m_fMinHalfPi;			// 円周率の半分の負の値
	LPDIRECT3DTEXTURE9      m_pTexture;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				//頂点バッファへのポインタ
	FLOAT3                  m_pos;					// 位置
	FLOAT2                  m_size;					// サイズ
	D3DXCOLOR               m_col;					// 色
	float                   m_fAngle;				// 角度
	ORIGINVERTEXTYPE        m_OriginType;			// 原点タイプ
	bool                    m_bDisp;				// 描画フラフラグ
};

#endif