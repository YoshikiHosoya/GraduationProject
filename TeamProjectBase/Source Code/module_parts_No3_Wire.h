//------------------------------------------------------------------------------
//
//モジュールパーツのワイヤー  [module_parts_No2_ShapeKey.h]
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
#include "module_No3_LampAndWire.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModule_Parts_No3_Wire : public CModule_Parts_Base
{
public:

	CModule_Parts_No3_Wire();
	virtual ~CModule_Parts_No3_Wire();

	virtual HRESULT Init()			override;			//初期化
	virtual void Update()			override;			//更新
	virtual void Draw()				override;			//描画
	virtual void ShowDebugInfo()	override;			//デバッグ情報表記

	void SetWire(CModule_No3_LampAndWire::WIRE wire);			//ワイヤー設定
	void SetWireCut(bool bCut) { m_bCut = bCut; };					//ワイヤーカットしたか設定

	CModule_No3_LampAndWire::WIRE GetWire() { return m_Wire; };		//ワイヤー情報取得
	bool GetWireCut() { return m_bCut; };							//ワイヤーカットしたか取得
protected:

private:
	CModule_No3_LampAndWire::WIRE m_Wire;				//ワイヤー
	D3DXCOLOR m_WireColor;								//ワイヤーの色
	bool m_bCut;										//切ったかどうか
};
#endif