//------------------------------------------------------------------------------
//
//メッシュフィールド処理  [meshfield.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MESHCIRCLE_H_
#define _MESHCIRCLE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "mesh.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CMeshCircle : public CMesh
{
public:
	CMeshCircle();
	~CMeshCircle();

	HRESULT Init();				//初期化
	void Uninit();				//終了
	void Update();				//更新
	void Draw();				//描画
	void ShowDebugInfo();		//デバッグ情報表記
	D3DXVECTOR3 SetVtx(INTEGER2 nCnt, INTEGER2 BlockNum);				//頂点設定

	//Set
	void SetSize(D3DXVECTOR3 size) {  };						//サイズ
	void SetRadius(float fRadius) { m_fRadius = fRadius; };		//半径
	void SetWidth(float fWidth) { m_fWidth = fWidth; };			//幅


	//Get
	D3DXVECTOR3 GetSize() { return D3DXVECTOR3(m_fRadius,0.0f, m_fRadius); };	//サイズ
	float &GetRadius() { return m_fRadius; };									//半径
	float &GetWidth() { return m_fWidth; };										//幅

	static std::shared_ptr<CMeshCircle> Create(D3DXVECTOR3 const pos, float fRadius, float fWidth, INTEGER2 const BlockNum);	//生成処理

protected:

private:
	float m_fRadius;	//半径
	float m_fWidth;		//幅

};
#endif