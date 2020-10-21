//------------------------------------------------------------------------------
//
//モジュールのボタン処理  [module_Button.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_BUTTON_H_
#define _MODULE_BUTTON_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Button : public CModule_Base
{
public:
	CModule_Button();
	virtual ~CModule_Button();

	virtual HRESULT Init()			override;			//初期化
	virtual void Uninit()			override;			//終了
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
private:

};
#endif