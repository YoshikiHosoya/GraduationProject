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
//マクロ
//------------------------------------------------------------------------------
#define MODULE_INTERVAL (D3DXVECTOR3(120.0f,50.0f,60.0f))

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

	//テンプレート関数
	template<class Module> void CreateModuleOne(int nNumber)
	{
		int nX = nNumber % 3;
		int nY = nNumber / 3;
		int nZ = nNumber / 6;

		m_pModuleList.emplace_back(CModule_Base::Create<Module>
									(D3DXVECTOR3((-MODULE_INTERVAL.x + (MODULE_INTERVAL.x * nX)) * (1 - (nZ * 2)),
												MODULE_INTERVAL.y - (MODULE_INTERVAL.y * ((nY % 2) * 2)),
												-MODULE_INTERVAL.z + (MODULE_INTERVAL.z * nZ * 2)),
									D3DXVECTOR3(0.0f, (D3DX_PI)* nZ, 0.0f), GetMtxWorldPtr()));

	}
};
#endif