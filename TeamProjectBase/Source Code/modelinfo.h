//------------------------------------------------------------------------------
//
//モデル情報管理  [modelinfo.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODELINFO_H_
#define _MODELINFO_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
//前方宣言
class CModelInfo
{
public:
	CModelInfo();
	~CModelInfo();

	enum MODEL_TYPE
	{
		MODEL_NONE = -1,

		//爆弾関係 100番台
		MODEL_BOMBBOX = 100,						//ボム
		MODEL_BATTERY_SMALL,						//バッテリー
		MODEL_BATTERY_BIG,							//バッテリー

		//モジュール 200番台
		MODEL_MODULE_DEFAULT = 200,					//モジュール
		MODEL_MODULE_NONE,							//モジュール　何も入ってない
		MODEL_MODULE_TIMER,							//タイマー
		MODEL_MODULE_NO0,							//0番目のモジュール
		MODEL_MODULE_NO1,							//1番目のモジュール
		MODEL_MODULE_NO2,							//2番目のモジュール
		MODEL_MODULE_NO3,							//3番目のモジュール
		MODEL_MODULE_NO4,							//4番目のモジュール

		//モジュールパーツ 300番台
		MODEL_MODULEPARTS_CLEARLAMP = 300,			//クリアランプ
		MODEL_MODULEPARTS_PROGRESSLAMP,				//進捗度ランプ
		MODEL_MODULEPARTS_NO0_KEYPAD,				//1番目のモジュールのパーツ
		MODEL_MODULEPARTS_NO1_KEYPAD,				//2番目のモジュールのパーツ
		MODEL_MODULEPARTS_NO2_WIRE,					//2番目のモジュールのパーツ
		MODEL_MODULEPARTS_NO2_WIRE_CUT,				//2番目のモジュールのパーツ ワイヤー切れてる状態
		MODEL_MODULEPARTS_NO3_BUTTON,				//3番目のモジュールパーツ
		MODEL_MODULEPARTS_NO3_DECITION_BUTTON,		//3番目のモジュールパーツ 送信ボタン

		MODEL_MODULEPARTS_NO4_BUTTON,				//4番目のモジュールのパーツ

		// タブレット 400番台
		MODEL_TABLET = 400,							// タブレット
		MODEL_TABLET_BUTTON,						// タブレットのボタン

		// 背景オブジェクト 500番台
		MODEL_BACKGROUND = 500,						//背景
	};


	typedef struct MODELNAME
	{
		std::string modelname;
		MODEL_TYPE modeltype;
	}MODELNAME;

	static void ModelLoad(HWND hwnd);			//モデル生成
	static void ModelUnload();					//モデル生成

	static S_ptr<CModelInfo> GetModelInfo(CModelInfo::MODEL_TYPE modeltype);			//モデル情報取得

	//Get関数
	std::string GetName()		{ return m_aName; };					//モデル名
	LPD3DXMESH GetMesh()		{ return m_mesh; };						//メッシュ情報
	LPD3DXBUFFER GetMatBuff()	{ return m_matBuff; };					//頂点情報
	DWORD GetMatNum()			{ return m_matNum; };					//マテリアル数
	MODEL_TYPE GetModelType()	{ return m_modeltype; };				//モデルタイプ

private:
	static Vec<S_ptr<CModelInfo>> m_apModelList;				//モデルへのポインタのリスト
	static Vec<MODELNAME> m_aModelFileNameList;					//モデルのファイル名

	MODEL_TYPE		m_modeltype;		//モデルの種類
	std::string		m_aName;			//モデル名
	LPD3DXMESH		m_mesh;				//メッシュ
	LPD3DXBUFFER	m_matBuff;			//頂点情報
	DWORD			m_matNum;			//マテリアル数

};

#endif


