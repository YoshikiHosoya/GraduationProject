//------------------------------------------------------------------------------
//
//モジュールの基底クラス  [module_base.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_BASE_H_
#define _MODULE_BASE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "sceneX.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_Lamp;

class CModule_Base : public CSceneX
{
public:
	CModule_Base();
	virtual ~CModule_Base();

	virtual HRESULT Init()			= 0;										//初期化
	virtual void Uninit()			= 0;										//終了
	virtual void Update()			= 0;										//更新
	virtual void Draw()				= 0;										//描画
	virtual void ShowDebugInfo()	= 0;										//デバッグ情報表記
	void CreateLamp(D3DXMATRIX *pModuleMtx);									//ランプ生成

	CModule_Parts_Lamp *GetLampPtr() { return m_pLamp.get(); };					//ランプのポインタ取得
	bool GetModuleClearFlag() { return m_bModuleClearFlag; };					//モジュールのクリアフラグ取得

	void Module_Clear();															//モジュールクリア
	void Module_Failed();															//モジュールクリア

	//テンプレート関数
	//モジュールの初期配置用の関数
	//呼び出し時にクラス型を教えてあげる必要がある
	//例) CModule_Base::Create<CModule_Timer>(...)
	template <class T> static std::shared_ptr<T> Create(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot, D3DXMATRIX * const pBomb)
	{
		//メモリ確保
		std::shared_ptr<T> pPtr = std::make_shared<T>();

		//初期化
		pPtr->Init();

		//座標とサイズ設定
		pPtr->SetPos(pos);
		pPtr->SetRot(rot);
		pPtr->SetParentMtxPtr(pBomb);

		//Scene側で管理
		pPtr->SetObjType(CScene::OBJTYPE_MODULE);
		pPtr->AddSharedList(pPtr);

		return pPtr;
	}


private:
	std::shared_ptr<CModule_Parts_Lamp> m_pLamp;				//ランプのポインタ
	bool	m_bModuleClearFlag;									//モジュールのクリアフラグ
};
#endif