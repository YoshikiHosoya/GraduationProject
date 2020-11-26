//------------------------------------------------------------------------------
//
//モジュールのランプとワイヤーのやつの処理  [Module_No2_LampAndWire.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_NO3_PASSWORD_H_
#define _MODULE_NO3_PASSWORD_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_No3_Button;
class CScene3D;

class CModule_No3_PassWord : public CModule_Base
{
public:

	CModule_No3_PassWord();
	virtual ~CModule_No3_PassWord();

	virtual HRESULT Init()				override;			//初期化
	virtual void Update()				override;			//更新
	virtual void Draw()					override;			//描画
	virtual void ShowDebugInfo()		override;			//デバッグ情報表記
	void Operation_Keyboard()			override;			//モジュール操作 キーボード
	void Operation_Mouse()				override;			//モジュール操作 マウス
	void ModuleAction()					override;			//モジュールのアクション
	void ModuleCancel()					override;			//モジュールの選択解除

private:
	Vec<S_ptr<CModule_Parts_No3_Button>> m_pButtonList;		//ボタンのリスト
	int m_nSelectPlace;										//現在選択している場所

	void CreateButton();									//ワイヤー生成

	void CheckClear();										//クリアしたかチェック
};
#endif