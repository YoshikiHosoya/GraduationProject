//------------------------------------------------------------------------------
//
//Xファイルモデル処理  [SceneX.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "SceneBase.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModelInfo;

class CSceneX : public CSceneBase
{
public:

	CSceneX();
	virtual ~CSceneX();

	virtual HRESULT Init();						//初期化
	virtual void Update();						//更新
	virtual void Draw();						//描画
	virtual void ShowDebugInfo();				//デバッグ情報表記

	void DrawMesh();							//モデルのメッシュ描画
	void DrawStencil();							//ステンシル用の描画処理　マテリアルの設定無し
	void DrawShadow();							//影の描画
	void DrawHardEdgeStencil();					//ハードエッジ描画用の深度バッファ描画

	void DrawMesh_SetMaterial(D3DXCOLOR col, bool bAllCol, Vec<int> const&ChangeColMatNum = {});	//引数の色に応じたマテリアルで描画

	void SetSelect(bool Select) { m_bSelecting = Select; };
	void SetEmissive(bool bEmissive) { m_bEmissive = bEmissive; };

	bool GetSelect() { return m_bSelecting; };
	bool GetEmissive() { return m_bEmissive; };

	static int GetNumSceneX() { return m_nNumSceneX; };			//数取得


	void BindModelInfo(S_ptr<CModelInfo> pSceneXInfo) { m_SceneXInfo = pSceneXInfo; };	//モデルセット

	CModelInfo* GetModelInfo(void) { return m_SceneXInfo.get(); }

private:
	static int m_nNumSceneX;									//総数カウント
	S_ptr<CModelInfo>	m_SceneXInfo;							//モデル情報
	bool m_bSelecting;											//選択されている

	bool m_bEmissive;											//自発光がＯＮかどうか
};
#endif