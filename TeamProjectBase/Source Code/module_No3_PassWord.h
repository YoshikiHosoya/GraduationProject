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
	//答えのパターン
	enum ANSWER_PATTERN
	{
		ANSWER_1 = 0,		//ふろはいる 26816
		ANSWER_2,			//しばざくら 48390
		ANSWER_3,			//ごうひろみ 55163
		ANSWER_4,			//なつやすみ 72843
		ANSWER_5,			//やまざくら 80390
		ANSWER_6,			//くつわむし 92864
		ANSWER_MAX,
	};

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
	Vec<S_ptr<CScene3D>> m_pDisplayNum;						//ディスプレイに表記される数値

	S_ptr<CSceneX>	m_pDecitionButton;						//送信ボタン
	Vec<int> m_nDisplayNum;									//ディスプレイの番号
	int m_nSelectNum;										//現在選択している場所
	int m_nAnswerNum;										//答えの番号
	ANSWER_PATTERN m_AnswerPattern;							//答えのパターン

	void SetAnswer();										//答えパターン設定
	void CreateDisplayNum();								//ディスプレイに表記される数値
	void CreateButton();									//ワイヤー生成
	void CheckClear();										//クリアしたかチェック

	void ChangeDisplayNum();								//ディスプレイの番号変更


};
#endif