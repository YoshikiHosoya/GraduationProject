//------------------------------------------------------------------------------
//
//モジュールのランプとワイヤーのやつの処理  [Module_No2_LampAndWire.h]
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
class CModule_Parts_No2_Wire;
class CScene3D;

class CModule_No2_LampAndWire : public CModule_Base
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

	CModule_No2_LampAndWire();
	virtual ~CModule_No2_LampAndWire();

	virtual HRESULT Init()				override;			//初期化
	virtual void Update()				override;			//更新
	virtual void Draw()					override;			//描画
	virtual void ShowDebugInfo()		override;			//デバッグ情報表記
	void Operation_Keyboard()			override;			//モジュール操作 キーボード
	void Operation_Mouse()				override;			//モジュール操作 マウス
	void ModuleAction()					override;			//モジュールのアクション
	void ModuleCancel()					override;			//モジュールの選択解除

private:
	Vec<S_ptr<CModule_Parts_No2_Wire>> m_pWireList;		//ワイヤーのリスト
	WIRE m_NowSelectWire;								//現在選択しているワイヤー
	int m_nSelectPlace;									//現在選択している場所

	int m_nRedLampNum;									//赤いランプ数
	int m_nBlueLampNum;									//青いランプ数

	void CreateWire();									//ワイヤー生成
	void CreateModuleLamp();							//ランプ生成
	void SetToCutWire();								//カットするワイヤー設定

	void WireCut();										//ワイヤーカット
	void CheckClear();									//クリアしたかチェック

	//色を基に切るワイヤー設定
	void SetCutWire_FromPlace(int nPlace);						//場所を基に切るワイヤー設定
	void SetCutWire_FromLampRule();								//ランプルール

//テンプレート関数
//色を基に切るワイヤーを設定
//指定した色右側、左側にも対応
//ワイヤ色　Itr_Begin Itr_End 切る切らないフラグ　指定した色の設定か　左側のコードか　右側のコードか
	template<class Itr>
	inline void SetCutWire_FromColor(WIRE wire, Itr begin, Itr end, bool bFlag, bool bThisCode = true, bool bLeft = false, bool bRight = false)
	{
		//指定した色のイテレータ取得
		auto itr = std::find_if(begin, end,
			[wire](S_ptr<CModule_Parts_No2_Wire> &ptr) {return ptr->GetWire() == wire; });

		//イテレータできた時
		if (itr != end)
		{
			//このコードかどうか
			if (bThisCode)
			{
				//クリアフラグを立てる
				itr->get()->SetClearFlag(bFlag);
			}
			//左側のコードか
			if (bLeft)
			{
				//仮でイテレータ生成
				auto LocalItr = itr;

				//イテレータが範囲内だった時
				if (LocalItr != begin)
				{
					//一つ手前
					--LocalItr;

					//クリアフラグ設定
					LocalItr->get()->SetClearFlag(true);
				}
			}
			//右側のコードか
			if (bLeft)
			{
				//仮でイテレータ生成
				auto LocalItr = itr;


				++LocalItr;

				//一つ後ろのイテレータが範囲内だった時
				if (LocalItr < end)
				{

					//クリアフラグ設定
					LocalItr->get()->SetClearFlag(true);
				}
			}
		}
	}
};
#endif