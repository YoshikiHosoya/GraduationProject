//------------------------------------------------------------------------------
//
//メッシュ処理  [mesh.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MESH_H_
#define _MESH_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
#include "Mylibrary.h"
#include "hossolibrary.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CMesh : public CScene
{
public:
	//メッシュの種類
	enum MESHTYPE
	{
		MESH_FIELD = 0,		//フィールド
		MESH_WALL,			//ウォール
		MESH_CYLINDER,		//シリンダー
		MESH_SPHERE			//スフィア
	};

	CMesh();
	~CMesh();

	virtual HRESULT Init();				//初期化
	virtual void Update();				//更新
	virtual void Draw();				//描画
	virtual void ShowDebugInfo();		//デバッグ情報表記
	virtual D3DXVECTOR3 SetVtx(INTEGER2 nCnt, INTEGER2 BlockNum) = 0;				//頂点設定処理
	virtual void SetSize(D3DXVECTOR3 size) = 0;				//サイズ設定　オーバーライド用
	void MakeVertex();										//頂点生成

	void SetTexSphere();									//テクスチャUV座標設定 球体用
	void SetNormal();										//法線設定
	void SetTex(bool bTex) { m_bTex = bTex; };				//テクスチャ設定

	void SetPos(D3DXVECTOR3 const& pos)				{ m_pos = pos; };				//座標設定
	void SetRot(D3DXVECTOR3 const& rot)				{ m_rot = rot; };				//回転設定
	void SetBlockNum(INTEGER2 const& blocknum)		{ m_BlockNum = blocknum; };		//ブロック数設定
	void BindTexture(LPDIRECT3DTEXTURE9 pTex)		{ m_pTexture = pTex; };			//テクスチャセット
	void SetCol(D3DXCOLOR const& col);								//色設定　色々するのでインラインじゃない
	void ResetVtx (D3DXVECTOR3 size);								//サイズ取得して頂点情報再設定

	D3DXVECTOR3 &GetPos()		{ return m_pos; };					//座標取得処理
	D3DXVECTOR3 &GetRot()		{ return m_rot; };					//回転量取得
	INTEGER2	&GetBlockNum()	{ return m_BlockNum; };				//ブロック数取得
	D3DXCOLOR	&GetCol()		{ return m_col; };					//色取得
	D3DXMATRIX	*GetMtx()		{ return &m_mtxWorld; };			//ワールドマトリックス取得
	static int GetNumMesh()		{ return m_nNumMesh; };				//数を取得
protected:
	std::vector<std::unique_ptr<D3DXVECTOR3>>	m_apNormalList;		//面の法線のポインタ
	std::vector<std::unique_ptr<VERTEX_3D>>		m_pVtxMeshList;		//頂点情報　保存用

private:
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;			//頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;			//インデックスバッファへのポインタ

	static int m_nNumMesh;			//総数
	D3DXVECTOR3 m_pos;				//座標
	D3DXVECTOR3 m_rot;				//回転
	D3DXCOLOR m_col;				//色
	MESHTYPE m_type;				//種類
	INTEGER2 m_BlockNum;			//ブロック数
	int m_nNumVertex;				//頂点数
	int m_nNumIdx;					//インデックス数
	int m_nNumPolygon;				//ポリゴン数
	bool m_bTex;					//テクスチャ使うかどうか
	D3DXMATRIX		m_mtxWorld;		//ワールドマトリックス
};
#endif