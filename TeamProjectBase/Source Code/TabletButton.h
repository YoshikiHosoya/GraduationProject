//*************************************************************************************************************
//
// タブレットボタンの基底処理 [TabletButton.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _TABLETBUTTON_H_
#define _TABLETBUTTON_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "sceneX.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CScene3D; 
class CTabletButton : public CSceneX
{
public:
	typedef enum
	{
		TYPE_NOEN = -1,	// 無し
		TYPE_PEN,		// ペン
		TYPE_ERASER,	// 消しゴム
		TYPE_SEND,		// 送信
		TYPE_MAX,		// 最大
	} TYPE;
	CTabletButton();															// コンストラクタ
	~CTabletButton();															// デストラクタ

	static void Load(void);														// 読み込み
	static void Unload(void);													// 開放
	HRESULT             Init();													// 初期化
	void                Update();												// 更新
	void                Draw();													// 描画
	inline void         ShowDebugInfo() {};										// デバッグ情報表記

	// 設定関数
	inline void SetType(TYPE type) { m_Type = type; }							// タイプの設定

	// 取得関数
	inline TYPE &GetType(void) { return m_Type; }								// タイプの取得

	// 生成
	static std::shared_ptr<CTabletButton> Create(D3DXMATRIX *pMtxParent,CONST D3DXVECTOR3 &pos, TYPE type);	// 生成
private:
	static LPDIRECT3DTEXTURE9 m_aTexture[TYPE_MAX];

	std::shared_ptr<CScene3D> m_pImage;
	TYPE m_Type;
};

#endif