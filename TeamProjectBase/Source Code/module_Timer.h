//------------------------------------------------------------------------------
//
//モジュールのタイマー処理  [module_timer.h]
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
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CTimer;
class CScene3D;

class CModule_Timer : public CModule_Base
{
public:
	CModule_Timer();
	virtual ~CModule_Timer();

	virtual HRESULT Init()			override;							//初期化
	virtual void Update()			override;							//更新
	virtual void Draw()				override;							//描画
	virtual void ShowDebugInfo()	override;							//デバッグ情報表記

	bool MissCountUp();										//ミスカウンターカウントアップ

private:
	U_ptr<CTimer> m_pTimer;									//タイマーのポインタ
	Vec<S_ptr<CScene3D>> m_pMissCounter;				//ミスカウンターのポインタ

	int m_nMissCount;

};
#endif