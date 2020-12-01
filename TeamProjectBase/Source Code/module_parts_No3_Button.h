//------------------------------------------------------------------------------
//
//モジュールパーツのボタン  [module_parts_No3_Button.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_PARTS_NO3_WIRE_H_
#define _MODULE_PARTS_NO3_WIRE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_parts_base.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_No3_Button : public CModule_Parts_Base
{
public:

	//ボタンの種類
	enum BUTTONTYPE
	{
		NONE = -1,		//無し
		UP,				//↑
		DOWN,			//↓
	};


	CModule_Parts_No3_Button();
	virtual ~CModule_Parts_No3_Button();

	virtual HRESULT Init()			override;						//初期化
	virtual void Update()			override;						//更新
	virtual void Draw()				override;						//描画
	virtual void ShowDebugInfo()	override;						//デバッグ情報表記

	void SetButtonType(BUTTONTYPE type);							//ボタンタイプ設定

	BUTTONTYPE GetButtonType() { return m_ButtonType; };			//ボタンタイプ取得

protected:

private:
	BUTTONTYPE m_ButtonType;										//ボタンタイプ
	int m_nNumber_To_Left;											//左から何番目のボタンか
};
#endif