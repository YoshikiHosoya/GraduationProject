//------------------------------------------------------------------------------
//
//メッシュシリンダー処理  [meshcylinder.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "mesh.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CMeshCylinder : public CMesh
{
public:
	CMeshCylinder();
	~CMeshCylinder();

	HRESULT Init();				//初期化
	void Uninit();				//終了
	void Update();				//更新
	void Draw();				//描画
	void ShowDebugInfo();		//デバッグ情報表記
	D3DXVECTOR3 SetVtx(INTEGER2 nCnt, INTEGER2 BlockNum);		//頂点設定
	void SetSize(D3DXVECTOR3 size) { m_size = size; };			//サイズ設定
	static std::shared_ptr<CMeshCylinder> Create(D3DXVECTOR3 const pos, D3DXVECTOR3 const onesize, INTEGER2 const BlockNum, D3DXMATRIX *pMtx);	//生成処理

protected:

private:
	D3DXMATRIX *m_pParentMtx;			//親のマトリックスのポインタ
	D3DXVECTOR3 m_size;					//サイズ

};
#endif