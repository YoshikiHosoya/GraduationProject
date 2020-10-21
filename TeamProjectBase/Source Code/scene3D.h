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

	virtual HRESULT Init()													override;			//初期化
	virtual void Uninit()													override;			//終了
	virtual void Update()													override;			//更新
	virtual void Draw()														override;			//描画
	virtual void ShowDebugInfo()											override;			//デバッグ情報表記
	void SetAnimation(D3DXVECTOR2 const UV, D3DXVECTOR2 const size)			override;			//テクスチャUV座標設定
	HRESULT MakeVertex()														override;			//頂点バッファ作成

	void DrawSettingMtx(D3DXMATRIX const &Mtx);													//渡す側で計算したワールドマトリックスを元に描画
	void DrawPolygon();																			//ポリゴンの描画

	void SetBillBoard(bool bBillBoard)	{ m_bBillBoard = bBillBoard;};							//ビルボード化
	void SetLighting(bool bLighting) { m_bLighting = bLighting; };								//ライティング設定

	bool GetBillBoard() { return m_bBillBoard; };												//ビルボードかどうか取得
	bool GetLighting() { return m_bLighting; };													//ライティングしているかどうか
	static int GetNumScene3D() { return m_nNumScene3D; };										//3Dポリゴン総数取得

protected:
private:
	static int m_nNumScene3D;		//3Dポリゴンの総数
	bool m_bBillBoard;				//ビルボードかどうか
	bool m_bLighting;				//ライティングするかどうか
};
#endif