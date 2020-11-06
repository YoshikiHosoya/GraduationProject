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
#define MODULE_INTERVAL (D3DXVECTOR3(120.0f,50.0f,40.0f))
#define MAX_MODULE_NUM	 (12)
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Base;

class CBomb : public CSceneX , public std::enable_shared_from_this<CBomb>
{
public:

	CBomb();
	virtual ~CBomb();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

	static S_ptr<CBomb> CreateBomb(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot,int const nModuleNum);
	void Operator();							//操作
	void ModuleClearCheck();
	void ModuleMiss();							//モジュールミスった

private:
	int m_nModuleNum;																	//モジュール数
	int m_nSelectModuleNum;																//選択しているモジュール数
	Vec<S_ptr<CModule_Base>> m_pModuleList;												//モジュールのリスト


	void CreateModule(int const nModuleNum);											//モジュール生成
	void CreateModule_Random(int const nModuleNum);

#ifdef _DEBUG
	void CreateModuleDebug();
	static bool m_bCanExplosion;
#endif

	//------------------------------------------------------------------------------
	//モジュール生成関数
	//------------------------------------------------------------------------------
	template<class Module> bool CreateModuleOne()
	{
		//モジュールが最大数に達したとき
		if (m_pModuleList.size() >= MAX_MODULE_NUM)
		{
			return false;
		}

		//変数宣言
		int nX = m_pModuleList.size() % 3;
		int nY = m_pModuleList.size() / 3;
		int nZ = m_pModuleList.size() / 6;

		m_pModuleList.emplace_back(CModule_Base::Create_Module<Module>
									(D3DXVECTOR3((-MODULE_INTERVAL.x + (MODULE_INTERVAL.x * nX)) * (1 - (nZ * 2)),
												MODULE_INTERVAL.y - (MODULE_INTERVAL.y * ((nY % 2) * 2)),
												-MODULE_INTERVAL.z + (MODULE_INTERVAL.z * nZ * 2)),
									D3DXVECTOR3(0.0f, (D3DX_PI)* nZ, 0.0f), GetMtxWorldPtr()));

		return true;
	}
};
#endif