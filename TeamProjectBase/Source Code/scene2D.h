//------------------------------------------------------------------------------
//
//シーン2D処理  [scene2D.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _SCENE2D_H_
#define _SCENE2D_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
#include "SceneBase.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CScene2D : public CSceneBase
{
public:
	CScene2D();
	virtual ~CScene2D();

	virtual HRESULT Init()												override;						//初期化
	virtual void Uninit()												override;						//終了
	virtual void Update()												override;						//更新
	virtual void Draw()													override;						//描画
	virtual void ShowDebugInfo()										override;						//デバッグ情報表記
	void SetAnimation(D3DXVECTOR2 const UV, D3DXVECTOR2 const size)		override;						//テクスチャUV座標設定
	void SetSize(D3DXVECTOR3 const size)								override;						//サイズ設定

	static std::shared_ptr<CScene2D> Create_Shared			(D3DXVECTOR3 const pos, D3DXVECTOR3 const size,D3DXCOLOR const col, OBJTYPE const objtype);		//生成処理（SceneとReturn先で共有管理
	static void Create_SceneManagement						(D3DXVECTOR3 const pos, D3DXVECTOR3 const size,D3DXCOLOR const col, OBJTYPE const objtype);		//生成処理（CSceneのみで管理
	static std::unique_ptr<CScene2D> Create_SelfManagement	(D3DXVECTOR3 const pos, D3DXVECTOR3 const size,D3DXCOLOR const col);							//生成処理（Return先で管理

	static int GetNumScene2D()	{ return m_nNumScene2D; };		//2Dの総数取得

protected:
	HRESULT MakeVertex(D3DXVECTOR3 const pos, D3DXVECTOR3 const size, D3DXCOLOR const col);		//頂点バッファ作成

private:
	static int m_nNumScene2D;								//総数
	float m_fAngle;											//角度　2Dポリゴンを回転する時に使う
	float m_fLength;										//長さ

};

#endif