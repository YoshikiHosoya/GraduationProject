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
	enum class SHAPE
	{
		CIRCLE = 0,
		CROSS,
		TRIANGLE,
		SQUARE,
		MAX,
	};


	//enum ANSWER_PATTERN
	//{
	//	ANSWER_1 = 0,
	//	ANSWER_2,
	//	ANSWER_3,
	//	ANSWER_4,
	//	ANSWER_5,
	//	ANSWER_6,
	//	ANSWER_MAX,
	//};

	CModule_No2_ShapeKeyPad();
	virtual ~CModule_No2_ShapeKeyPad();

	virtual HRESULT Init()			override;			//初期化
	virtual void Uninit()			override;			//終了
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
	void Operation()				override;			//モジュール操作
	void CreateKeyPad();								//キーパッド生成
	void CheckClear();									//クリアしたか確認
	void ShapeShuffle();								//図形のシャッフル
	void SetDisplayShape(int nShape);					//ディスプレイの図形設定

	void SetNextPushKey();								//次に押すキーの設定
	void SetNextPushKey_FromShape(SHAPE shape);			//図形を基に次に押すキー設定
	void SetNextPushKey_FromPlace(int nPlace);			//場所を基に次に押すキー設定 ←から何番目か


private:
	std::vector<std::shared_ptr<CModule_Parts_No2_ShapeKey>> m_pKeyPadList;				//キーパッドのポインタの配列
	std::unique_ptr<CScene3D> m_pDisplayShape;									//ディスプレイに映る図形のポリゴンのポインタ

	SHAPE m_DisplayShape;														//ディスプレイの図形

	//ANSWER_PATTERN m_Answer;													//答えのパターン
	//int m_nNextSymbolNum;														//次のシンボル番号
};
#endif