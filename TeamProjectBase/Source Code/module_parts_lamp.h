//------------------------------------------------------------------------------
//
//モジュールのランプ  [module_parts_lamp.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_PARTS_LAMP_H_
#define _MODULE_PARTS_LAMP_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_parts_base.h"
#include "sceneX.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CTimer;

class CModule_Parts_Lamp : public CModule_Parts_Base,public CSceneX
{
public:
	enum class LAMP_STATE
	{
		OFF,
		RED,
		GREEN,
	};


	CModule_Parts_Lamp();
	virtual ~CModule_Parts_Lamp();

	virtual HRESULT Init()			override;			//初期化
	virtual void Uninit()			override;			//終了
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

	void SetLampState(LAMP_STATE lampstate)		{ m_LampState = lampstate; };
	LAMP_STATE GetLampState()					{ return m_LampState; };

	static std::shared_ptr<CModule_Parts_Lamp> Create(D3DXVECTOR3 const pos,D3DXMATRIX* const pModuleMtx);
private:
	int m_nCntLampCnt;					//ランプ用のカウント
	LAMP_STATE m_LampState;				//ランプの状態
};
#endif