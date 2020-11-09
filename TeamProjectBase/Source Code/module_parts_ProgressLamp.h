//------------------------------------------------------------------------------
//
//モジュールの進捗度のランプ  [module_parts_Progresslamp.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_PARTS_PROGRESSLAMP_H_
#define _MODULE_PARTS_PROGRESSLAMP_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_parts_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CTimer;

class CModule_Parts_ProgressLamp : public CModule_Parts_Base
{
public:

	CModule_Parts_ProgressLamp();
	virtual ~CModule_Parts_ProgressLamp();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

private:

};
#endif