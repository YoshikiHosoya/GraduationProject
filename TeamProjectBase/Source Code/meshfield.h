//------------------------------------------------------------------------------
//
//メッシュフィールド処理  [meshfield.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "mesh.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CMeshField : public CMesh
{
public:
	CMeshField();
	~CMeshField();

	HRESULT Init();												//初期化
	void Update();												//更新
	void Draw();												//描画
	void ShowDebugInfo();										//デバッグ情報表記
	D3DXVECTOR3 SetVtx(INTEGER2 nCnt, INTEGER2 BlockNum);		//頂点設定

	//Set
	void SetSize(D3DXVECTOR3 size) { m_size = size; };			//サイズ設定

	//Get
	D3DXVECTOR3 GetSize() { return m_size; };					//サイズ取得

	static std::shared_ptr<CMeshField> Create(D3DXVECTOR3 const pos, D3DXVECTOR3 const onesize, INTEGER2 const BlockNum);	//生成処理
	bool GetHeight(D3DXVECTOR3 &pos);					//高さ取得

protected:

private:
	D3DXVECTOR3 m_size;		//サイズ


};
#endif