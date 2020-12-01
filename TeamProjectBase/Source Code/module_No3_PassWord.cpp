//------------------------------------------------------------------------------
//
//モジュールのランプとワイヤーのやつの処理  [Module_No2_LampAndWire.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_parts_No3_Button.h"
#include "module_No3_PassWord.h"
#include "modelinfo.h"
#include "manager.h"
#include "keyboard.h"
#include "game.h"
#include "particle.h"
#include "mouse.h"
#include "sound.h"
#include "scene3D.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define BUTTON_OFFSET					(D3DXVECTOR3(-3.1f,0.0f,-20.0f))
#define BUTTON_ITNERVAL					(D3DXVECTOR3(14.0f,18.0f,0.0f))
#define BUTTON_NUM						(10)
#define NUMBER_NUM						(5)

#define DISPLAYNUM_OFFSET				(D3DXVECTOR3(-3.1f,0.0f,-20.0f))
#define DISPLAYNUM_INTERVAL				(D3DXVECTOR3(14.0f,0.0f,0.0f))
#define DISPLAYNUM_SIZE					(D3DXVECTOR3(11.0f,18.0f,0.0f))

#define DECITION_BUTTON_OFFSET			(D3DXVECTOR3(0.0f,-30.0f,-20.0f))


//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_No3_PassWord::CModule_No3_PassWord()
{
	m_nSelectNum = 0;
	m_pDecitionButton.reset();
	m_pDisplayNum = {};
	m_nDisplayNum = { 0,0,0,0,0 };
	m_nAnswerNum = 0;
	m_AnswerPattern = ANSWER_PATTERN::ANSWER_1;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_No3_PassWord::~CModule_No3_PassWord()
{
	m_pDecitionButton.reset();
	m_pDisplayNum = {};
	m_nDisplayNum = {};
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_No3_PassWord::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_NO3));

	//ボタン生成
	CreateButton();

	//クリアランプ生成
	CModule_Base::CreateLamp();

	//ディスプレイの数値生成
	CreateDisplayNum();

	//答え設定
	SetAnswer();

	//初期化
	CSceneX::Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_No3_PassWord::Update()
{
	CSceneX::Update();

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "DisplayNum >> [%d] [%d] [%d] [%d] [%d]\n", m_nDisplayNum[0], m_nDisplayNum[1], m_nDisplayNum[2], m_nDisplayNum[3], m_nDisplayNum[4]);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "AnswerPattern >> [%d]\n", m_AnswerPattern);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "AnswerNum >> [%d]\n", m_nAnswerNum);

}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_No3_PassWord::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_No3_PassWord::ShowDebugInfo()
{
#ifdef _DEBUG
	CModule_Base::ShowDebugInfo();

#endif //DEBUG
}

//------------------------------------------------------------------------------
//モジュール操作
//------------------------------------------------------------------------------
void CModule_No3_PassWord::Operation_Keyboard()
{
	int nSelectNumOld = m_nSelectNum;

	//選択処理
	CHossoLibrary::Selecting(m_nSelectNum, nSelectNumOld, 5, 2);

	//モジュール操作
	CModule_Base::Operation_Keyboard();

}


//------------------------------------------------------------------------------
//モジュール操作　マウス
//------------------------------------------------------------------------------
void CModule_No3_PassWord::Operation_Mouse()
{
	//レイの判定
	CHossoLibrary::RayCollision_ModuleSelect(m_pButtonList.begin(), m_pButtonList.end(), (int&)m_nSelectNum);

	//マウス操作
	CModule_Base::Operation_Mouse();

	if (CHossoLibrary::MouseRayCollision_Boolean(m_pDecitionButton->GetMtxWorldPtr(), m_pDecitionButton->GetModelInfo()->GetMesh()))
	{
		m_pDecitionButton->SetSelect(true);
	}
	else
	{
		m_pDecitionButton->SetSelect(false);
	}
}

//------------------------------------------------------------------------------
//モジュールアクション
//------------------------------------------------------------------------------
void CModule_No3_PassWord::ModuleAction()
{
	//決定ボタンが選択されていた時
	if (m_pDecitionButton->GetSelect())
	{
		//クリアチェック
		CheckClear();
	}

	//選択番号が-1とかだった時
	if (m_nSelectNum < 0)
	{
		return;
	}

	//nullcheck
	if (m_pButtonList[m_nSelectNum].get())
	{
		//何桁目か算出
		int nLocalValue = m_nSelectNum % 5;

		//↑か↓かで加算するか減算するか
		m_nSelectNum / 5 == 0 ? m_nDisplayNum[nLocalValue]++ : m_nDisplayNum[nLocalValue]--;

		//0から9の間でループさせる
		CHossoLibrary::RangeLimit_Loop(m_nDisplayNum[nLocalValue], 0, 9);

		//ディスプレイの番号変更
		ChangeDisplayNum();

	}
}

//------------------------------------------------------------------------------
//モジュールの選択解除
//------------------------------------------------------------------------------
void CModule_No3_PassWord::ModuleCancel()
{
	//選択解除
	//CModule_Base::ModuleParts_Select<CModule_Parts_No2_Wire>(m_pButtonList, -1);
}

//------------------------------------------------------------------------------
//答え設定
//------------------------------------------------------------------------------
void CModule_No3_PassWord::SetAnswer()
{
	//答えのパターンをランダムで設定
	m_AnswerPattern = (ANSWER_PATTERN)CHossoLibrary::RandomRangeUnsigned(ANSWER_PATTERN::ANSWER_1, ANSWER_PATTERN::ANSWER_MAX);

	switch (m_AnswerPattern)
	{
	case CModule_No3_PassWord::ANSWER_1:
		m_nAnswerNum = 26816;
		break;
	case CModule_No3_PassWord::ANSWER_2:
		m_nAnswerNum = 48390;
		break;
	case CModule_No3_PassWord::ANSWER_3:
		m_nAnswerNum = 55163;
		break;
	case CModule_No3_PassWord::ANSWER_4:
		m_nAnswerNum = 72843;
		break;
	case CModule_No3_PassWord::ANSWER_5:
		m_nAnswerNum = 80390;
		break;
	case CModule_No3_PassWord::ANSWER_6:
		m_nAnswerNum = 92864;
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------
//ディスプレイに表記される数値
//------------------------------------------------------------------------------
void CModule_No3_PassWord::CreateDisplayNum()
{
	for (int nCnt = 0; nCnt < NUMBER_NUM ; nCnt++)
	{
		m_pDisplayNum.emplace_back(CSceneBase::ScenePolygonCreateShared<CScene3D>(DISPLAYNUM_OFFSET + D3DXVECTOR3(CHossoLibrary::CalcEvenPosition(5, nCnt, DISPLAYNUM_INTERVAL.x), 0.0f, 0.0f),
			DISPLAYNUM_SIZE, BlackColor, CTexture::GetSeparateTexture(CTexture::SEPARATE_TEX_UI_NUMBER), CScene::OBJTYPE_MODULE_PARTS_SYMBOL));
		m_pDisplayNum[nCnt]->SetParentMtxPtr(GetMtxWorldPtr());

		//UV座標設定
		m_pDisplayNum[nCnt]->SetAnimation(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.1f, 1.0f));

	}
}

//------------------------------------------------------------------------------
//ボタン生成
//------------------------------------------------------------------------------
void CModule_No3_PassWord::CreateButton()
{

	for (int nCnt = 0; nCnt < BUTTON_NUM; nCnt++)
	{
		//仮変数
		INTEGER3 Value = { 0,0,0 };
		Value.x = nCnt % 5;
		Value.y = nCnt < 5 ? 1 : -1;

		//モジュールパーツ生成
		m_pButtonList.emplace_back(CModule_Parts_Base::Create_ModuleParts<CModule_Parts_No3_Button>
		(BUTTON_OFFSET + D3DXVECTOR3(CHossoLibrary::CalcEvenPosition(5, Value.x, BUTTON_ITNERVAL.x), BUTTON_ITNERVAL.y / Value.y, 0.0f), GetMtxWorldPtr()));

		//ボタンタイプ設定
		m_pButtonList[nCnt]->SetButtonType(Value.y > 0 ? CModule_Parts_No3_Button::BUTTONTYPE::UP : CModule_Parts_No3_Button::BUTTONTYPE::DOWN);
	}

	//送信ボタン生成
	m_pDecitionButton = CSceneX::CreateShared(DECITION_BUTTON_OFFSET, ZeroVector3, GetMtxWorldPtr(), OBJTYPE_MODULE_PARTS, CModelInfo::MODEL_MODULEPARTS_NO3_DECITION_BUTTON);

}

//------------------------------------------------------------------------------
//クリアしたかチェック
//各桁があっているかチェック
//------------------------------------------------------------------------------
void CModule_No3_PassWord::CheckClear()
{
	for (int nCnt = 0; nCnt < (int)m_nDisplayNum.size(); nCnt++)
	{
		//数値を出す
		int nValue = m_nAnswerNum % (int)powf(10.0f, m_nDisplayNum.size() - (float)nCnt) / (int)powf(10.0f, m_nDisplayNum.size() - (float)nCnt - 1.0f);

		//ディスプレイの数値と値の比較
		if (m_nDisplayNum[nCnt] != nValue)
		{
			//違った時はモジュールミス
			Module_Failed();
			return;
		}
	}

	//全てあっていた場合はモジュールクリア
	Module_Clear();
}

//------------------------------------------------------------------------------
//ディスプレイの番号変更
//------------------------------------------------------------------------------
void CModule_No3_PassWord::ChangeDisplayNum()
{
	//各桁の計算
	for (size_t nCnt = 0; nCnt < NUMBER_NUM; nCnt++)
	{
		float fValue = (float)m_nDisplayNum[nCnt] / 10.0f;

		//UV座標設定
		m_pDisplayNum[nCnt]->SetAnimation(D3DXVECTOR2(fValue, 0.0f), D3DXVECTOR2(0.1f, 1.0f));
	}

}
