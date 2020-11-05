//------------------------------------------------------------------------------
//
//Sceneのベースクラス  [SceneBase.cpp]
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
	virtual	void Update()			= 0;							//更新
	virtual	void Draw()				= 0;							//描画
	virtual void ShowDebugInfo()	= 0;							//デバッグ情報表記

	virtual HRESULT MakeVertex() { return E_FAIL; };	//頂点バッファ生成関数 ポリゴンの場合はこの関数をオーバーライド

	//Set関数
	//仮想関数　頂点バッファの再調整とかオーバーライドでしてもらう
	virtual void SetPos(D3DXVECTOR3 const &pos)					{ m_pos = pos; };							//座標
	virtual void SetSize(D3DXVECTOR3 const &size)				{ m_size = size; };							//サイズ
	virtual void SetColor(D3DXCOLOR const &col)					{ m_col = col; };							//色
	virtual void SetRot(D3DXVECTOR3 const &rot)					{ m_rot = rot; };							//回転量
	virtual void SetScale(D3DXVECTOR3 const &scale)				{ m_scale = scale; };						//拡大率
	virtual void SetAnimation(D3DXVECTOR2 const UV, D3DXVECTOR2 const UVsize) {};

	void BindTexture(LPDIRECT3DTEXTURE9 const tex)				{ m_pTexture = tex; };						//テクスチャ
	void BindVtxBuff(LPDIRECT3DVERTEXBUFFER9 const pVtxBuff)	{ m_pVtxBuff = pVtxBuff; };					//頂点バッファ
	void SetMtxWorld(D3DXMATRIX const mtxWorld)					{ m_mtxWorld = mtxWorld; };					//ワールドマトリックス
	void SetParentMtxPtr(D3DXMATRIX *pMtx)						{ m_pParentMtx = pMtx; };					//親のマトリックス設定
	void SetDisp(bool bDisp)									{ m_bDisp = bDisp; };						//表示非表示
	void SetLighting(bool bLighting)							{ m_bLighting = bLighting; };				//ライティング設定

	//Get関数
	D3DXVECTOR3 &GetPos()										{ return m_pos; };							//座標
	D3DXVECTOR3 &GetSize()										{ return m_size; };							//サイズ
	D3DXCOLOR &GetColor()										{ return m_col; };							//色
	D3DXVECTOR3 &GetRot()										{ return m_rot; };							//回転量
	D3DXVECTOR3 &GetScale()										{ return m_scale; };						//拡大率
	bool &GetDisp()												{ return m_bDisp; };						//表示非表示
	bool &GetLighting()											{ return m_bLighting; };					//ライティングしているかどうか

	LPDIRECT3DTEXTURE9 GetTexture()								{ return m_pTexture; };						//テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff()						{ return m_pVtxBuff; };						//頂点バッファのポインタ
	D3DXVECTOR3 *GetPosPtr()									{ return &m_pos; };							//座標のポインタ　当たり判定とかはこれを使う
	D3DXMATRIX *GetMtxWorldPtr()								{ return &m_mtxWorld; };					//ワールドマトリックス
	D3DXMATRIX *GetParentMtxPtr()								{ return m_pParentMtx; };					//親のマトリックス

	void CalcMtx_IncludeParentMtx();


	//テンプレート関数
	//ポリゴン生成関数
	//2D3D両方可能
	//SharedPtrを利用して共用管理するポインタ
	template<class T>
	static S_ptr<T> ScenePolygonCreateShared
	(D3DXVECTOR3 const & pos, D3DXVECTOR3 const & size, D3DXCOLOR const & col,
		LPDIRECT3DTEXTURE9 const tex, CScene::OBJTYPE const objtype)
	{
		//メモリ確保
		S_ptr<T> ptr = std::make_shared<T>();

		//情報設定
		ptr->SetPos(pos);
		ptr->SetSize(size);
		ptr->SetColor(col);
		ptr->SetObjType(objtype);
		ptr->BindTexture(tex);

		//初期化処理
		ptr->Init();

		//Sceneにポインタを渡す
		ptr->SetObjType(objtype);
		ptr->AddSharedList(ptr);
		return ptr;
	}

	//テンプレート関数
	//ポリゴン生成関数
	//2D3D両方可能
	//UniquePtrで呼び出した側が管理
	template<class T>
	static U_ptr<T> ScenePolygonCreateSelfManagement
	(D3DXVECTOR3 const & pos, D3DXVECTOR3 const & size, D3DXCOLOR const & col,
		LPDIRECT3DTEXTURE9 const tex)
	{
		//メモリ確保
		U_ptr<T> ptr = std::make_unique<T>();

		//情報設定
		ptr->SetPos(pos);
		ptr->SetSize(size);
		ptr->SetColor(col);
		ptr->BindTexture(tex);

		//初期化処理
		ptr->Init();

		//return
		return std::move(ptr);
	}

	//テンプレート関数
	//ポリゴン生成関数
	//2D3D両方可能
	//UniquePtrでSceneが管理
	template<class T>
	static void ScenePolygonCreateSceneManagement
	(D3DXVECTOR3 const & pos, D3DXVECTOR3 const & size, D3DXCOLOR const & col,
		LPDIRECT3DTEXTURE9 const tex, CScene::OBJTYPE const objtype)
	{
		//メモリ確保
		U_ptr<T> ptr = std::make_unique<T>();

		//情報設定
		ptr->SetPos(pos);
		ptr->SetSize(size);
		ptr->SetColor(col);
		ptr->BindTexture(tex);

		//初期化処理
		ptr->Init();

		//Sceneにポインタを渡す
		ptr->SetObjType(objtype);
		ptr->AddUniqueList(std::move(ptr));
	}



private:
	LPDIRECT3DTEXTURE9 m_pTexture;							//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						//頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;										//座標
	D3DXVECTOR3 m_size;										//サイズ
	D3DXCOLOR m_col;										//色
	D3DXVECTOR3 m_rot;										//回転量
	D3DXVECTOR3 m_scale;									//拡大率
	D3DXMATRIX m_mtxWorld;									//ワールドマトリックス
	D3DXMATRIX *m_pParentMtx;								//親のマトリックス
	bool m_bDisp;											//表示非表示の設定
	bool m_bLighting;										//ライティングするかどうか

};
#endif

