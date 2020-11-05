//------------------------------------------------------------------------------
//
//モジュールの基底クラス  [module_base.h]
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
class CBomb;

class CModule_Base : public CSceneX
{
public:

	enum class MODULE_TYPE
	{
		NONE = 0,
		TIMER,
		BUTTON,
		NO1_SYMBOL,
		NO2_SHAPE,
		NO3_WIRE,
		MAX,

	};

	CModule_Base();
	virtual ~CModule_Base();

	virtual HRESULT Init() = 0;															//初期化
	virtual void Update() = 0;															//更新
	virtual void Draw() = 0;															//描画
	virtual void ShowDebugInfo() = 0;													//デバッグ情報表記
	virtual void Operation() {};														//モジュールの操作
	void CreateLamp();																	//ランプ生成
	void CameraApproach();																//カメラを近づける

	void SetCanModuleSelect(bool bSelect) { m_bCanSelect = bSelect; };					//モジュールを選択できるか
	void SetModuleType(MODULE_TYPE module) { m_ModuleType = module; };					//モジュールのタイプ設定
	void SetBombPtr(std::weak_ptr<CBomb> pBombPtr) { m_pBomb = pBombPtr; };				//ボムのポインタ設定

	CModule_Parts_Lamp *GetLampPtr() { return m_pLamp.get(); };							//ランプのポインタ取得
	MODULE_TYPE GetModuleType() { return m_ModuleType; };								//モジュールの種類
	bool GetModuleClearFlag() { return m_bModuleClearFlag; };							//モジュールのクリアフラグ取得
	bool GetCanModuleSelect() { return m_bCanSelect; };									//モジュール選択できるかどうか

	void Module_Clear();																//クリア
	void Module_Failed();																//失敗

	//Create関数
	//テンプレート関数
	//モジュールの初期配置用の関数
	//呼び出し時にクラス型を教えてあげる必要がある
	//例) CModule_Base::Create<CModule_Timer>(...)
	template <class Module> static S_ptr<Module> Create_Module(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot, D3DXMATRIX * const pBomb)
	{
		//メモリ確保
		S_ptr<Module> pPtr = std::make_shared<Module>();

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
	std::weak_ptr<CBomb> m_pBomb;								//爆弾のポインタ
	S_ptr<CModule_Parts_Lamp> m_pLamp;							//ランプのポインタ
	MODULE_TYPE m_ModuleType;									//モジュールの種類
	bool	m_bModuleClearFlag;									//モジュールのクリアフラグ
	bool	m_bCanSelect;										//選択できるモジュールか
};
#endif