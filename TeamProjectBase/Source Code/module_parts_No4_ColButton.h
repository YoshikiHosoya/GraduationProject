//------------------------------------------------------------------------------
//
//モジュールパーツの色のボタン  [module_parts_No4_ColButton.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_PARTS_NO4_COLBUTTON_H_
#define _MODULE_PARTS_NO4_COLBUTTON_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_parts_base.h"
#include "module_No4_4ColButton.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_No4_ColButton : public CModule_Parts_Base
{
public:

	CModule_Parts_No4_ColButton();
	virtual ~CModule_Parts_No4_ColButton();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

	void SetButtonCol(CModule_No4_4ColButton::BUTTON button);
	void SetButtonLighting(bool bLight);

	CModule_No4_4ColButton::BUTTON GetButton()				{ return m_Button; };
	bool GetButtonLighting()								{ return m_bLight; };

protected:

private:
	CModule_No4_4ColButton::BUTTON m_Button;	//ボタン
	D3DXCOLOR m_ButtonCol;						//ボタンの色
	bool m_bLight;								//ライトが点灯しているかどうか
	int m_nCntLight;							//ライトの点灯のカウント
};
#endif