//------------------------------------------------------------------------------
//
//モジュールのランプとワイヤーのやつの処理  [Module_No3_LampAndWire.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_NO3_LAMPANDWIRE_H_
#define _MODULE_NO3_LAMPANDWIRE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_No3_LampAndWire : public CModule_Base
{
public:
	CModule_No3_LampAndWire();
	virtual ~CModule_No3_LampAndWire();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
private:

};
#endif