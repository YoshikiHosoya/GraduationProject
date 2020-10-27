//------------------------------------------------------------------------------
//
//モジュールの図形のキーパッド処理  [module_ShapeKeypad.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_NO2_SHAPEKEYPAD_H_
#define _MODULE_NO2_SHAPEKEYPAD_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_No2_ShapeKey;
class CScene3D;

class CModule_No2_ShapeKeyPad : public CModule_Base
{
public:

	//図形
	enum class SHAPE
	{
		NONE = -1,
		CIRCLE,				//〇
		CROSS,				//×
		TRIANGLE,			//△
		SQUARE,				//□
		MAX,
	};

	//問題数
	enum class QUESTION
	{
		NONE = -1,
		Q_1,
		Q_2,
		Q_3,
		Q_4,
		MAX,
	};

	enum PLACE
	{
		NONE = -1,
		LEFT_TO_1ST,
		LEFT_TO_2ND,
		LEFT_TO_3RD,
		LEFT_TO_4TH,

	};

	//過去にクリアしたボタンの情報
	typedef struct
	{
		SHAPE shape;
		PLACE place;
	}MEMORIES;


	CModule_No2_ShapeKeyPad();
	virtual ~CModule_No2_ShapeKeyPad();

	virtual HRESULT Init()			override;			//初期化
	virtual void Uninit()			override;			//終了
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
	void Operation()				override;			//モジュール操作

	void CreateDisplayShape();							//ディスプレイの生成
	void CreateKeyPad();								//キーパッド生成
	void CreateProgressLamp();							//進捗のランプ生成

	void CheckClear();									//クリアしたか確認
	void ShapeShuffle();								//図形のシャッフル
	void SetDisplayShape(int nShape);					//ディスプレイの図形設定

	void SetNextPushKey();								//次に押すキーの設定
	void SetNextPushKey_FromShape(SHAPE shape);			//図形を基に次に押すキー設定
	void SetNextPushKey_FromPlace(PLACE nPlace);			//場所を基に次に押すキー設定 ←から何番目か

	void QuestionClear();								//問題クリア

private:
	std::vector<std::shared_ptr<CModule_Parts_No2_ShapeKey>> m_pKeyPadList;				//キーパッドのポインタの配列
	std::unique_ptr<CScene3D> m_pDisplayShape;									//ディスプレイに映る図形のポリゴンのポインタ
	std::vector<MEMORIES> m_Memories;											//過去にどのボタン押したか記憶する
	std::vector<std::shared_ptr<CScene3D>> m_pProgressLamp;						//進捗度のランプ

	SHAPE m_DisplayShape;														//ディスプレイの図形
	QUESTION m_NowQuestion;														//現在何問目か
	PLACE m_nSelectNum;															//選択番号

	int m_nQuestionChangeCnt;													//問題切り替え中のカウント

};
#endif