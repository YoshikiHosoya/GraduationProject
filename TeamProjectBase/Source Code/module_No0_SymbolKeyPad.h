//------------------------------------------------------------------------------
//
//モジュールのボタン処理  [Module_No0_SymbolKeypad.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_NO1_SYMBOLKEYPAD_H_
#define _MODULE_NO1_SYMBOLKEYPAD_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_No0_SymbolKey;

class CModule_No0_SymbolKeyPad : public CModule_Base
{
public:
	enum ANSWER_PATTERN
	{
		ANSWER_1 = 0,
		ANSWER_2,
		ANSWER_3,
		ANSWER_4,
		ANSWER_5,
		ANSWER_6,
		ANSWER_MAX,
	};

	CModule_No0_SymbolKeyPad();
	virtual ~CModule_No0_SymbolKeyPad();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
	void Operation_Keyboard()		override;			//モジュール操作 キーボード
	void Operation_Mouse()			override;			//モジュール操作 マウス
	void ModuleAction()				override;			//モジュールのアクション
	void ModuleCancel()				override;			//モジュールの選択解除

	void CreateKeyPad(ANSWER_PATTERN answer);			//キーパッド生成
	void CheckClear();									//クリアしたか確認
private:
	Vec<S_ptr<CModule_Parts_No0_SymbolKey>> m_pKeyPadList;				//キーパッドのポインタの配列
	ANSWER_PATTERN m_Answer;											//答えのパターン
	int m_nNextSymbolNum;												//次のシンボル番号
	int m_nSelectNum;													//現在の選択番号
};
#endif