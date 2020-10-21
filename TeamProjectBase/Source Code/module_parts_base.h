//------------------------------------------------------------------------------
//
//モジュールのパーツの基底クラス  [module_parts_base.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_PARTS_BASE_H_
#define _MODULE_PARTS_BASE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "sceneX.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_Base : public CSceneX
{
public:
	CModule_Parts_Base()
	{
	};
	virtual ~CModule_Parts_Base() {};

	virtual HRESULT Init() = 0;					//初期化
	virtual void Uninit() = 0;					//終了
	virtual void Update() = 0;					//更新
	virtual void Draw() = 0;					//描画
	virtual void ShowDebugInfo() = 0;			//デバッグ情報表記

	//Create関数
	template<class Module> static std::shared_ptr<Module> Create_ModuleParts(D3DXVECTOR3 const pos, D3DXMATRIX * const pModuleMtxPtr);

private:



};
#endif

//テンプレート関数
//モジュールの初期配置用の関数
//呼び出し時にクラス型を教えてあげる必要がある
//例) CModule_Base::Create<CModule_Timer>(...)
template<class Module>
inline std::shared_ptr<Module> static CModule_Parts_Base::Create_ModuleParts(D3DXVECTOR3 const pos, D3DXMATRIX * const pModuleMtxPtr)
{
	//メモリ確保
	std::shared_ptr<Module> pPtr = std::make_shared<Module>();

	//初期化
	pPtr->Init();

	//座標とサイズ設定
	pPtr->SetPos(pos);
	pPtr->SetParentMtxPtr(pModuleMtxPtr);

	//Scene側で管理
	pPtr->SetObjType(CScene::OBJTYPE_MODULE_PARTS);
	pPtr->AddSharedList(pPtr);

	return pPtr;
}