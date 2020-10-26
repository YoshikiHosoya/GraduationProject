//------------------------------------------------------------------------------
//
//モジュールのボタン処理  [module_Button.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_KEYPAD_H_
#define _MODULE_KEYPAD_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_Key;

class CModule_KeyPad : public CModule_Base
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

	CModule_KeyPad();
	virtual ~CModule_KeyPad();

	virtual HRESULT Init()			override;			//初期化
	virtual void Uninit()			override;			//終了
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
	void Operation()				override;			//モジュール操作
	void CreateKeyPad(ANSWER_PATTERN answer);			//キーパッド生成
	void CheckClear();									//クリアしたか確認
private:
	std::vector<std::shared_ptr<CModule_Parts_Key>> m_pKeyPadList;				//キーパッドのポインタの配列
	ANSWER_PATTERN m_Answer;													//答えのパターン
	int m_nNextSymbolNum;														//次のシンボル番号
};
#endif