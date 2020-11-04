//------------------------------------------------------------------------------
//
//モジュールの図形のキーパッド処理  [module_ShapeKeypad.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_NO4_4COLBUTTON_H_
#define _MODULE_NO4_4COLBUTTON_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_No4_ColButton;

class CModule_No4_4ColButton : public CModule_Base
{
public:

	CModule_No4_4ColButton();
	virtual ~CModule_No4_4ColButton();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
	void Operation()				override;			//モジュール操作


private:
	Vec<S_ptr<CModule_Parts_No4_ColButton>> m_pColButtonList;
};
#endif