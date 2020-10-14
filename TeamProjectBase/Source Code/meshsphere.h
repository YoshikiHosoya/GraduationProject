//------------------------------------------------------------------------------
//
//メッシュスフィア処理  [meshsphere.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "mesh.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CMeshSphere : public CMesh
{
public:
	CMeshSphere();
	~CMeshSphere();

	virtual HRESULT Init();												//初期化
	virtual void Uninit();												//終了
	virtual void Update();												//更新
	virtual void Draw();												//描画
	virtual void ShowDebugInfo();										//デバッグ情報表記
	static std::shared_ptr<CMeshSphere> Create(D3DXVECTOR3 const pos, float const fRadius, INTEGER2 const BlockNum);	//生成処理

	D3DXVECTOR3 SetVtx(INTEGER2 nCnt, INTEGER2 BlockNum);				//頂点設定
	void SetSize(D3DXVECTOR3 size) { m_fRadius = size.x; };				//サイズ設定
	float &GetRadius() { return m_fRadius; };							//半径取得

protected:

private:
	float m_fRadius;			//半径
};
#endif