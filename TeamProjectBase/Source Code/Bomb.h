//------------------------------------------------------------------------------
//
//爆弾の処理  [Bomb.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _BOMB_H_
#define _BOMB_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "SceneX.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Base;

class CBomb : public CSceneX
{
public:
	CBomb();
	virtual ~CBomb();

	virtual HRESULT Init()			override;			//初期化
	virtual void Uninit()			override;			//終了
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

	static std::shared_ptr<CBomb> Create(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot,int const nModuleNum);

private:
	int m_nModuleNum;																		//モジュール数
	std::vector<std::shared_ptr<CModule_Base>> m_pModuleList;							//モジュールのリスト

	void CreateModule(int const nModuleNum);											//モジュール生成
};
#endif