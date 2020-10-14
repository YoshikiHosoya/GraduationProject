//------------------------------------------------------------------------------
//
//モジュールの処理  [module.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_H_
#define _MODULE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "SceneX.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule : public CSceneX
{
public:
	CModule();
	virtual ~CModule();

	virtual HRESULT Init()			override;			//初期化
	virtual void Uninit()			override;			//終了
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

	static std::shared_ptr<CModule> Create(D3DXVECTOR3 const pos,D3DXMATRIX* const pBombMtx);
	void SetParentMtx(D3DXMATRIX* const pBombMtx) { m_pBombMatrix = pBombMtx; };
private:
	D3DXMATRIX *m_pBombMatrix;


};
#endif