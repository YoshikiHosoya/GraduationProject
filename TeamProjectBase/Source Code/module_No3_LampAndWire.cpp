//------------------------------------------------------------------------------
//
//モジュールのランプとワイヤーのやつの処理  [Module_No3_LampAndWire.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_No3_LampAndWire.h"
#include "module_parts_No3_Wire.h"
#include "modelinfo.h"
#include "manager.h"
#include "keyboard.h"
#include "game.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define WURE_OFFSET					(D3DXVECTOR3(-37.0f,0.0f,-23.0f))
#define WURE_ITNERVAL				(D3DXVECTOR3(12.0f,0.0f,0.0f))
#define WIRE_NUM					(6)
#define LAMP_NUM					(8)
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_No3_LampAndWire::CModule_No3_LampAndWire()
{
	m_NowSelectWire = WIRE::RED;
	m_nSelectPlace = 0;

	m_nRedLampNum = 0;
	m_nBlueLampNum = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_No3_LampAndWire::~CModule_No3_LampAndWire()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_No3_LampAndWire::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_NO3));

	//ワイヤー生成
	CreateWire();

	//ランプ生成
	CreateLamp();

	//初期化
	CSceneX::Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::Update()
{
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::ShowDebugInfo()
{
#ifdef _DEBUG
#endif //DEBUG
}

//------------------------------------------------------------------------------
//モジュール操作
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::Operation()
{
	int nSelectNumOld = m_nSelectPlace;

	//選択処理
	CHossoLibrary::Selecting(m_nSelectPlace, nSelectNumOld, 6, 1);

	for (size_t nCnt = 0; nCnt < m_pWireList.size(); nCnt++)
	{
		//nullcheck
		if (m_pWireList[nCnt].get())
		{
			//現在の選択番号と同じモノだけtrueにしておく
			nCnt == m_nSelectPlace ?
				m_pWireList[nCnt]->SetSelect(true) :
				m_pWireList[nCnt]->SetSelect(false);
		}
	}


	//nullcheck
	if (CManager::GetKeyboard()->GetTrigger(DIK_BACKSPACE))
	{
		//選択解除
		CModule_Base::SelectRelease<CModule_Parts_No3_Wire>(m_pWireList);

		//ゲームの視点変更
		CManager::GetGame()->SetGaze(CGame::GAZE_BOMB);
	}
}

//------------------------------------------------------------------------------
//ワイヤー生成
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::CreateWire()
{
	//ローカル変数　ワイヤーは6本で6色バラバラ
	Vec<WIRE> wirecol = { WIRE::RED,WIRE::BLUE,WIRE::GREEN ,WIRE::YELLOW,WIRE::WHITE, WIRE::BLACK };

	//シャッフル
	CHossoLibrary::Vec_Shuffle(wirecol);

	for(int nCnt = 0;  nCnt < WIRE_NUM; nCnt++)
	{
		m_pWireList.emplace_back(CModule_Parts_Base::Create_ModuleParts<CModule_Parts_No3_Wire>
			(WURE_OFFSET + D3DXVECTOR3(nCnt * WURE_ITNERVAL.x, 0.0f, 0.0f), GetMtxWorldPtr()));

		m_pWireList[m_pWireList.size() - 1]->SetWireCol(wirecol[nCnt]);
	}

}
//------------------------------------------------------------------------------
//ランプ生成
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::CreateLamp()
{
	m_nRedLampNum = CHossoLibrary::RandomRangeUnsigned(0, LAMP_NUM);
	m_nBlueLampNum = LAMP_NUM - m_nRedLampNum;


}

//------------------------------------------------------------------------------
//切るべきワイヤー設定
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::SetToCutWire()
{
	//赤いランプ数を基に切るコード設定
	//詳細は仕様書参考
	switch (m_nRedLampNum)
	{
	case 4:

		break;

	case 1:

		break;

	case 6:

		break;


		//排他的パターン
	default:
		//青ランプの方が多い時
		if (m_nRedLampNum < m_nBlueLampNum)
		{

		}
		//赤ランプが多い時
		else
		{

		}

		break;
	}


	//ランプルールを基に切るコード設定


}

//------------------------------------------------------------------------------
//切るべきワイヤー設定 ワイヤーの色を基に設定
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::SetCutWire_FromColor(WIRE wire)
{

}

//------------------------------------------------------------------------------
//切るべきワイヤー設定 場所を基に設定
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::SetCutWire_FromPlace(int nPlace)
{


}
