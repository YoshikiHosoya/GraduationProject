//------------------------------------------------------------------------------
//
//モジュールのタイマー処理  [module_timer.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_TIMER_H_
#define _MODULE_TIMER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_base.h"
#include "sceneX.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CTimer;

class CModule_Timer : public CModule_Base,public CSceneX
{
public:
	CModule_Timer();
	virtual ~CModule_Timer();

	virtual HRESULT Init()			override;			//初期化
	virtual void Uninit()			override;			//終了
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
private:
	std::unique_ptr<CTimer> m_pTimer;


};
#endif