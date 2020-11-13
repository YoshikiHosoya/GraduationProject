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
#include "particle.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define WURE_OFFSET					(D3DXVECTOR3(-37.0f,0.0f,-23.0f))
#define WURE_ITNERVAL				(D3DXVECTOR3(12.0f,0.0f,0.0f))
#define WIRE_NUM					(6)
#define LAMP_OFFSET					(D3DXVECTOR3(-28.0f,-5.0f,-30.0f))
#define LAMP_INTERVAL				(D3DXVECTOR3(9.0f,0.0f,0.0f))
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

	//モジュール内のランプ生成
	CreateModuleLamp();

	//クリアランプ生成
	CModule_Base::CreateLamp();

	//カットするワイヤー設定
	SetToCutWire();

	//初期化
	CSceneX::Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::Update()
{
	D3DXVECTOR3 posorigin = CHossoLibrary::CalcMtxToVector3(*GetMtxWorldPtr());

	for (int nCnt = 0; nCnt < LAMP_NUM; nCnt++)
	{
		if (nCnt < m_nRedLampNum)
		{
			CParticle::CreateFromText(posorigin + LAMP_OFFSET + D3DXVECTOR3(LAMP_INTERVAL.x * nCnt, 0.0f, 0.0f), ZeroVector3, CParticleParam::EFFECT_MODULE_3_LED,RedColor);
		}
		else
		{
			CParticle::CreateFromText(posorigin + LAMP_OFFSET + D3DXVECTOR3(LAMP_INTERVAL.x * nCnt, 0.0f, 0.0f), ZeroVector3, CParticleParam::EFFECT_MODULE_3_LED,BlueColor);
		}
	}

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

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		//nullcheck
		if (m_pWireList[m_nSelectPlace].get())
		{
			//ワイヤーカットされてない時
			if (!m_pWireList[m_nSelectPlace]->GetWireCut())
			{
				//間違えたワイヤーだった時
				if (!m_pWireList[m_nSelectPlace]->GetClearFlag())
				{
					//失敗
					Module_Failed();
				}

				//ワイヤーカット
				WireCut();

				//クリアしたかチェック
				CheckClear();
			}
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


	CDebugProc::Print(CDebugProc::PLACE_LEFT, "-------------Module_No_3----------------\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "RedLampNum >> [%d]\n", m_nRedLampNum);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "BlueLampNum >> [%d]\n", m_nBlueLampNum);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "Flag >>> [%d]  [%d]  [%d]  [%d]  [%d]  [%d]\n",
		m_pWireList[0]->GetClearFlag(), m_pWireList[1]->GetClearFlag(), m_pWireList[2]->GetClearFlag(),
		m_pWireList[3]->GetClearFlag(), m_pWireList[4]->GetClearFlag(), m_pWireList[5]->GetClearFlag());

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

		m_pWireList[m_pWireList.size() - 1]->SetWire(wirecol[nCnt]);
	}

}
//------------------------------------------------------------------------------
//ランプ生成
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::CreateModuleLamp()
{
	//赤いランプをランダム設定
	m_nRedLampNum = CHossoLibrary::RandomRangeUnsigned(0, LAMP_NUM);

	//赤いランプ数から逆算して青いランプ設定
	m_nBlueLampNum = LAMP_NUM - m_nRedLampNum;




}

//------------------------------------------------------------------------------
//切るべきワイヤー設定
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::SetToCutWire()
{
	auto Itr_Begin = m_pWireList.begin();
	auto Itr_End = m_pWireList.end();

	//赤いランプ数を基に切るコード設定
	//詳細は仕様書参考
	switch (m_nRedLampNum)
	{
	case 4:
		SetCutWire_FromColor(WIRE::BLACK, Itr_Begin, Itr_End, true, false, true, true);		//黒の両側
		SetCutWire_FromColor(WIRE::YELLOW, Itr_Begin, Itr_End, true, false, true, false);	//黄の左
		SetCutWire_FromColor(WIRE::RED, Itr_Begin, Itr_End, true, false, false, true);		//赤の右
		SetCutWire_FromPlace(3);															//左から4番目

		break;

	case 1:
		SetCutWire_FromColor(WIRE::GREEN, Itr_Begin, Itr_End, true, false, true, true);		//緑の両側
		SetCutWire_FromColor(WIRE::WHITE, Itr_Begin, Itr_End, true, false, true, true);		//白の両側
		SetCutWire_FromPlace(4);															//右から2番目
		SetCutWire_FromPlace(0);															//左端
		SetCutWire_FromPlace(5);															//右端

		break;

	case 2:
		SetCutWire_FromColor(WIRE::WHITE, Itr_Begin, Itr_End, true, false, true, true);				//白の両側
		SetCutWire_FromColor(WIRE::YELLOW, Itr_Begin, Itr_End, true, false, false, true);			//黄の右
		SetCutWire_FromPlace(4);																	//左から5番目
		break;


		//排他的パターン
	default:
		//青ランプが多い時
		if (m_nRedLampNum < m_nBlueLampNum)
		{
			SetCutWire_FromColor(WIRE::GREEN, Itr_Begin, Itr_End, true, true, true, true);			//緑と緑の両側
			SetCutWire_FromColor(WIRE::BLUE, Itr_Begin, Itr_End, true);								//青の両側
			SetCutWire_FromPlace(2);																//左から3番目
		}
		//赤ランプが多い時
		else
		{
			SetCutWire_FromColor(WIRE::WHITE, Itr_Begin, Itr_End, true);							//白
			SetCutWire_FromColor(WIRE::YELLOW, Itr_Begin, Itr_End, true);							//黄
			SetCutWire_FromColor(WIRE::RED, Itr_Begin, Itr_End, true, false, true, true);			//赤の両側
			SetCutWire_FromPlace(1);																//右から5番目

		}

		break;
	}


	//ランプルールを基に切るコード設定
	SetCutWire_FromLampRule();

}

//------------------------------------------------------------------------------
//切るべきワイヤー設定
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::WireCut()
{
	m_pWireList[m_nSelectPlace]->SetWireCut(true);
	m_pWireList[m_nSelectPlace]->BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULEPARTS_NO1_KEYPAD));
	m_pWireList[m_nSelectPlace]->SetClearFlag(false);

}

//------------------------------------------------------------------------------
//切るべきワイヤー設定
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::CheckClear()
{
	int nCount = std::count_if(m_pWireList.begin(), m_pWireList.end(),
		[](S_ptr<CModule_Parts_No3_Wire> ptr) {return ptr->GetClearFlag(); });

	if (nCount <= 0)
	{
		Module_Clear();
	}
}

//------------------------------------------------------------------------------
//切るべきワイヤー設定 場所を基に設定
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::SetCutWire_FromPlace(int nPlace)
{
	if (nPlace < WIRE_NUM)
	{
		m_pWireList[nPlace] -> SetClearFlag(true);
	}
}

//------------------------------------------------------------------------------
//切るべきワイヤー設定 ランプルールを基に設定
//------------------------------------------------------------------------------
void CModule_No3_LampAndWire::SetCutWire_FromLampRule()
{
	auto Itr_Begin = m_pWireList.begin();
	auto Itr_End = m_pWireList.end();

	//青ランプ3つ以上
	if (m_nBlueLampNum >= 3)
	{
		SetCutWire_FromColor(WIRE::BLUE, Itr_Begin, Itr_End, false);				//青を切らない

		//青ランプ5つ以上
		if (m_nBlueLampNum >= 5)
		{
			SetCutWire_FromColor(WIRE::RED, Itr_Begin, Itr_End, true);				//赤を切る

			//全部青ランプ
			if (m_nBlueLampNum >= LAMP_NUM)
			{
				SetCutWire_FromColor(WIRE::WHITE, Itr_Begin, Itr_End, true);		//白を切る
				SetCutWire_FromColor(WIRE::BLACK, Itr_Begin, Itr_End, true);		//黒を切る

			}
		}
	}

	//赤ランプ3つ以上
	if (m_nRedLampNum >= 3)
	{
		SetCutWire_FromColor(WIRE::BLACK, Itr_Begin, Itr_End, false);				//白を切らない

		//赤ランプ5つ以上
		if (m_nRedLampNum >= 5)
		{
			SetCutWire_FromColor(WIRE::GREEN, Itr_Begin, Itr_End, true);			//黒を切る

			//全部赤ランプ
			if (m_nRedLampNum >= LAMP_NUM)
			{
				SetCutWire_FromColor(WIRE::RED, Itr_Begin, Itr_End, true);			//赤を切る
				SetCutWire_FromColor(WIRE::BLUE, Itr_Begin, Itr_End, true);			//青を切る
			}
		}
	}



}
