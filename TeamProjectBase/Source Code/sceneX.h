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

	virtual HRESULT Init()				override;				//初期化
	virtual void Update()				override;				//更新
	virtual void Draw()					override;				//描画
	virtual void ShowDebugInfo()		override;				//デバッグ情報表記
	static S_ptr<CSceneX> CreateShared(D3DXVECTOR3 const &pos, D3DXVECTOR3 const &rot, D3DXMATRIX *pParentMtx, CScene::OBJTYPE objtype, int nModelType);

	void DrawMesh();											//モデルのメッシュ描画
	void DrawStencil();											//ステンシル用の描画処理　マテリアルの設定無し
	void DrawShadow();											//影の描画
	void DrawHardEdgeStencil();									//ハードエッジ描画用の深度バッファ描画

	void DrawMesh_SetMaterial(D3DXCOLOR col, bool bAllCol, Vec<int> const&ChangeColMatNum = {});	//引数の色に応じたマテリアルで描画

	void SetTextureNum(Vec<int> const &TexNumVec) { m_TexNumber = TexNumVec; };			//テクスチャ番号設定

	void SetSelect(bool Select) { m_bSelecting = Select; };								//選択しているか設定
	void SetCanModuleSelect(bool bSelect) { m_bCanSelect = bSelect; };					//選択可能かどうか
	void SetEmissive(bool bEmissive) { m_bEmissive = bEmissive; };						//Emissive設定
	void BindModelInfo(S_ptr<CModelInfo> pSceneXInfo) { m_SceneXInfo = pSceneXInfo; };	//モデルセット

	bool GetSelect() { return m_bSelecting; };											//選択しているか
	bool GetCanModuleSelect() { return m_bCanSelect; };									//選択可能か
	bool GetEmissive() { return m_bEmissive; };											//Emissiveかどうか

	static int GetNumSceneX() { return m_nNumSceneX; };									//数取得
	CModelInfo* GetModelInfo(void) { return m_SceneXInfo.get(); }						//モデル情報取得

private:
	static int m_nNumSceneX;															//総数カウント
	S_ptr<CModelInfo>	m_SceneXInfo;													//モデル情報
	bool m_bSelecting;																	//選択されている
	bool m_bCanSelect;																	//選択可能かどうか

	bool m_bEmissive;																	//自発光がＯＮかどうか

	Vec<int> m_TexNumber;																//テクスチャ番号
};
#endif