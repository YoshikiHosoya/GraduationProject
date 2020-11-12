//------------------------------------------------------------------------------
//
//モジュールのランプとワイヤーのやつの処理  [Module_No3_LampAndWire.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODULE_NO3_LAMPANDWIRE_H_
#define _MODULE_NO3_LAMPANDWIRE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "module_base.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_No3_Wire;

class CModule_No3_LampAndWire : public CModule_Base
{
public:

	enum WIRE
	{
		NONE = -1,
		RED,			//赤
		BLUE,			//青
		GREEN,			//緑
		YELLOW,			//黄
		WHITE,			//白
		BLACK,			//黒
		MAX,
	};

	CModule_No3_LampAndWire();
	virtual ~CModule_No3_LampAndWire();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記
	void Operation()					override;			//モジュール操作
private:
	Vec<S_ptr<CModule_Parts_No3_Wire>> m_pWireList;		//ワイヤーのリスト
	WIRE m_NowSelectWire;								//現在選択しているワイヤー
	int m_nSelectPlace;									//現在選択している場所

	int m_nRedLampNum;									//赤いランプ数
	int m_nBlueLampNum;									//青いランプ数

	void CreateWire();									//ワイヤー生成
	void CreateLamp();									//ランプ生成
	void SetToCutWire();								//カットするワイヤー設定

	void SetCutWire_FromColor(WIRE wire);						//色を基に切るワイヤー設定
	void SetCutWire_FromPlace(int nPlace);						//場所を基に切るワイヤー設定
};
#endif