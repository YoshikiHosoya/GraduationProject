//------------------------------------------------------------------------------
//
//シーン3D処理  [scene3D.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _SCENE3D_H_
#define _SCENE3D_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scenebase.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CScene3D : public CSceneBase
{
public:

	CScene3D();
	virtual ~CScene3D();

	virtual HRESULT Init();								//初期化
	virtual void Uninit();								//終了
	virtual void Update();								//更新
	virtual void Draw();								//描画
	virtual void ShowDebugInfo();						//デバッグ情報表記

	void DrawSettingMtx(D3DXMATRIX const &Mtx);				//渡す側で計算したワールドマトリックスを元に描画
	void DrawPolygon();										//ポリゴンの描画

	void SetAnimation(D3DXVECTOR2 const UV, D3DXVECTOR2 const size) override;		//テクスチャUV座標設定
	void SetBillBoard(bool bBillBoard)	{ m_bBillBoard = bBillBoard;};		//ビルボード化

	bool BetBillBoard() { return m_bBillBoard; };
	static int GetNumScene3D() { return m_nNumScene3D; };												//3Dポリゴン総数取得

	static std::shared_ptr<CScene3D> Create_Shared			(D3DXVECTOR3 const pos, D3DXVECTOR3 const size,D3DXCOLOR const col, OBJTYPE const objtype);		//生成処理（SceneとReturn先で共有管理
	static void Create_SceneManagement						(D3DXVECTOR3 const pos, D3DXVECTOR3 const size,D3DXCOLOR const col, OBJTYPE const objtype);		//生成処理（CSceneのみで管理
	static std::unique_ptr<CScene3D> Create_SelfManagement	(D3DXVECTOR3 const pos, D3DXVECTOR3 const size,D3DXCOLOR const col);							//生成処理（Return先で管理


protected:
	HRESULT MakeVertex(D3DXVECTOR3 const pos, D3DXVECTOR3 const size, D3DXCOLOR const col);		//頂点バッファ作成

private:
	static int m_nNumScene3D;

	bool m_bBillBoard;										//ビルボードかどうか
};
#endif