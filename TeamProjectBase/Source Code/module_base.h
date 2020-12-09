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
		NONE = 0,				//モジュールなし
		TIMER,					//タイマー
		NO0_SYMBOL,				//記号
		NO1_SHAPE,				//図形
		NO2_WIRE,				//ワイヤー
		NO3_PASSWORD,			//パスワード
		NO4_4COLBUTTON,			//4色ボタン
		MAX,
	};

	CModule_Base();																		//コンストラクタ
	virtual ~CModule_Base();															//デストラクタ

	virtual HRESULT Init() = 0;															//初期化
	virtual void Update() = 0;															//更新
	virtual void Draw() = 0;															//描画
	virtual void ShowDebugInfo();													//デバッグ情報表記
	virtual void Operation_Keyboard();													//モジュール操作　キーボード
	virtual void Operation_Mouse();														//モジュール操作　マウス
	virtual void ModuleAction(){};														//モジュールに対して何かアクションをする
	virtual void ModuleCancel(){};														//モジュールの選択を解除

	void CreateLamp();																	//ランプ生成
	void CameraApproach();																//カメラを近づける

	void SetModuleType(MODULE_TYPE module) { m_ModuleType = module; };					//モジュールのタイプ設定
	void SetBombPtr(W_ptr<CBomb> pBombPtr) { m_pBomb = pBombPtr; };						//ボムのポインタ設定
	void SetOperating(bool bOperating) { m_bOperating = bOperating; };					//操作してる

	W_ptr<CBomb> GetBombWeakPtr() { return m_pBomb; };									//ボムのweakポインタ取得

	CModule_Parts_Lamp *GetLampPtr() { return m_pLamp.get(); };							//ランプのポインタ取得
	MODULE_TYPE GetModuleType() { return m_ModuleType; };								//モジュールの種類
	bool GetModuleClearFlag() { return m_bModuleClearFlag; };							//モジュールのクリアフラグ取得
	bool GetOperatin() { return m_bOperating; };										//操作してる

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

	//テンプレート関数
	//モジュールの選択状態を設定する関数
	//全部解除する場合はnSelectNumに-1を入れて使う
	template <class ModuleParts> static void ModuleParts_Select(Vec<S_ptr<ModuleParts>> &rList,int nSelectNum)
	{
		//配列が空かどうかチェック
		if (rList.empty())
		{
			return;
		}

		for (int nCnt = 0; nCnt < (int)rList.size(); nCnt++)
		{
			//現在の選択番号と同じモノだけtrueにしておく
			nCnt == nSelectNum ?
				rList[nCnt]->SetSelect(true) :
				rList[nCnt]->SetSelect(false);

		}
	}

private:
	std::weak_ptr<CBomb> m_pBomb;								//爆弾のポインタ
	S_ptr<CModule_Parts_Lamp> m_pLamp;							//ランプのポインタ
	MODULE_TYPE m_ModuleType;									//モジュールの種類
	bool	m_bModuleClearFlag;									//モジュールのクリアフラグ
	bool	m_bOperating;										//モジュール操作中

};
#endif