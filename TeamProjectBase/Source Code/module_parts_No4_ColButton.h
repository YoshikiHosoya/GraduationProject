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
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_No4_ColButton : public CModule_Parts_Base
{
public:

	enum class BUTTON_COL
	{
		RED = 0,
		GREEN,
		BLUE,
		YELLOW
	};

	CModule_Parts_No4_ColButton();
	virtual ~CModule_Parts_No4_ColButton();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

	void SetButtonCol(BUTTON_COL col);

protected:

private:
	BUTTON_COL m_buttoncol;
};
#endif