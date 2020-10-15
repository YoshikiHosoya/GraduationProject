//------------------------------------------------------------------------------
//
//シーン2D処理  [scene2D.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _SCENEBASE_H_
#define _SCENEBASE_H_

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CSceneBase :public CScene
{
public:
	CSceneBase();													//コンストラクタ
	virtual	~CSceneBase();											//デストラクタ
	virtual	HRESULT Init()			= 0;							//初期化
	virtual	void Uninit()			= 0;							//終了
	virtual	void Update()			= 0;							//更新
	virtual	void Draw()				= 0;							//描画
	virtual void ShowDebugInfo()	= 0;							//デバッグ情報表記

	//Set関数
	//仮想関数　頂点バッファの再調整とかオーバーライドでしてもらう
	virtual void SetPos(D3DXVECTOR3 const &pos)					{ m_pos = pos; };							//座標
	virtual void SetSize(D3DXVECTOR3 const &size)				{ m_size = size; };							//サイズ
	virtual void SetColor(D3DXCOLOR const &col)					{ m_col = col; };							//色
	virtual void SetRot(D3DXVECTOR3 const &rot)					{ m_rot = rot; };							//回転量
	virtual void SetDisp(bool bDisp)							{ m_bDisp = bDisp; };						//表示非表示
	virtual void SetAnimation(D3DXVECTOR2 const UV, D3DXVECTOR2 const UVsize) {};

	void BindTexture(LPDIRECT3DTEXTURE9 const tex)				{ m_pTexture = tex; };						//テクスチャ
	void BindVtxBuff(LPDIRECT3DVERTEXBUFFER9 const pVtxBuff)	{ m_pVtxBuff = pVtxBuff; };					//頂点バッファ
	void SetMtxWorld(D3DXMATRIX const mtxWorld)					{ m_mtxWorld = mtxWorld; };					//ワールドマトリックス
	void SetParentMtxPtr(D3DXMATRIX *pMtx)						{ m_pParentMtx = pMtx; };					//親のマトリックス設定

	//Get関数
	D3DXVECTOR3 &GetPos()										{ return m_pos; };							//座標
	D3DXVECTOR3 &GetSize()										{ return m_size; };							//サイズ
	D3DXCOLOR &GetColor()										{ return m_col; };							//色
	D3DXVECTOR3 &GetRot()										{ return m_rot; };							//回転量
	bool &GetDisp()												{ return m_bDisp; };						//表示非表示

	LPDIRECT3DTEXTURE9 GetTexture()								{ return m_pTexture; };						//テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff()						{ return m_pVtxBuff; };						//頂点バッファのポインタ
	D3DXVECTOR3 *GetPosPtr()									{ return &m_pos; };							//座標のポインタ　当たり判定とかはこれを使う
	D3DXMATRIX *GetMtxWorldPtr()								{ return &m_mtxWorld; };					//ワールドマトリックス
	D3DXMATRIX *GetParentMtxPtr()								{ return m_pParentMtx; };					//親のマトリックス

protected:

private:
	LPDIRECT3DTEXTURE9 m_pTexture;							//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						//頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;										//座標
	D3DXVECTOR3 m_size;										//サイズ
	D3DXCOLOR m_col;										//色
	D3DXVECTOR3 m_rot;										//回転量
	D3DXMATRIX m_mtxWorld;									//ワールドマトリックス
	D3DXMATRIX *m_pParentMtx;								//親のマトリックス
	bool m_bDisp;											//表示非表示の設定

};
#endif