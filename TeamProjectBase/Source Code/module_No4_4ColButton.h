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

	//ボタンの色
	enum BUTTON
	{
		RED = 0,			//赤
		GREEN,				//緑
		BLUE,				//青
		YELLOW				//黄
	};

	enum class STATE
	{
		START,				//開始
		LIGHTING,			//点灯
		INTERVAL,			//次のボタン点滅までのインターバル
		PLAYER_INPUT,		//プレイヤー入力中
		RESET_INTERVAL,		//次の周回へのインターバル
		END,

	};


	CModule_No4_4ColButton();
	virtual ~CModule_No4_4ColButton();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
	void Operation()				override;			//モジュール操作

	void SetButtonState(STATE state);


	void NextButtonSet();
	void PlayerInputReset();


	void ButtonPushSuccess();
	bool CheckModuleClear();


private:
	Vec<S_ptr<CModule_Parts_No4_ColButton>> m_pColButtonList;
	BUTTON m_nNowSelectButton;										//現在の洗濯番号
	STATE m_buttonState;											//ボタンのステート
	Vec<BUTTON> m_QuestionButtonList;								//答えのパターン

	int m_nNowFlashNumber;											//現在の点灯してるバターン
	int m_nClearNum;												//クリアした数
	int m_nButtonLightingCnt;										//ボタンのライト点滅用のカウント

	int m_nPlayerPushNum;											//プレイヤーの入力した回数



};
#endif