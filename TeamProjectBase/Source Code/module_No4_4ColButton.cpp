//------------------------------------------------------------------------------
//
//モジュールの図形のキーパッド処理  [module_ShapeKeypad.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_No4_4ColButton.h"
#include "module_parts_No4_ColButton.h"
#include "module_parts_ProgressLamp.h"
#include "Bomb.h"
#include "Bomb_Exterior.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "modelinfo.h"
#include "timer.h"
#include "game.h"
#include "mouse.h"
#include "sound.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define COL_BUTTON_NUM									(4)										//ボタン数
#define COL_BUTTON_OFFSET								(D3DXVECTOR3(-10.0f,0.0f,-14.5f))			//ボタンのオフセット座標
#define COL_BUTTON_LIGHT_FLASH_INTERVAL					(45)									//ボタンの点灯の感覚
#define COL_BUTTON_LIGHT_LOOP_INTERVAL					(180)									//ボタンのループ間隔
#define PROGRESSLAMP_OFFSET								(D3DXVECTOR3(35.0f,-10.0f,-15.0f))		//進捗度ランプのオフセット座標
#define INPUT_GRACE										(180)									//プレイヤーがボタンを入力してからリセットされるまでの猶予


//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_No4_4ColButton::CModule_No4_4ColButton()
{
	m_pColButtonList.clear();
	m_nNowSelectButton = BUTTON::RED;
	m_buttonState = STATE::START;
	m_nButtonLightingCnt = 0;
	m_nNowFlashNumber = -1;
	m_nClearNum = 0;
	m_pProgressLamp.reset();
	m_nPlayerPushNum = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_No4_4ColButton::~CModule_No4_4ColButton()
{
	m_pColButtonList.clear();
	m_pProgressLamp.reset();

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_No4_4ColButton::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_NO4));

	int nCnt = 0;

	while (m_pColButtonList.size() < COL_BUTTON_NUM)
	{
		//右回りか左回りかの変数
		int nValue = nCnt <= 1 ? 1 : -1;

		//ボタン生成
		m_pColButtonList.emplace_back(CModule_Parts_Base::Create_ModuleParts<CModule_Parts_No4_ColButton>(COL_BUTTON_OFFSET, GetMtxWorldPtr()));
		m_pColButtonList[nCnt]->SetRot(D3DXVECTOR3(0.0f, 0.0f,
			((-D3DX_PI * 0.5f) * (nCnt % 2) * nValue) + ((D3DX_PI * 0.5f) * (nCnt / 2))));

		//ボタン色設定
		m_pColButtonList[nCnt]->SetButtonCol((BUTTON)nCnt);

		//答えのパターン生成
		m_QuestionButtonList.emplace_back((BUTTON)CHossoLibrary::RandomRangeUnsigned(0, 3));

		nCnt++;
	}

	//次のボタン設定
	NextButtonSet();

	//ランプ生成
	CModule_Base::CreateLamp();
	m_pProgressLamp = CModule_Parts_Base::Create_ModuleParts<CModule_Parts_ProgressLamp>(PROGRESSLAMP_OFFSET, GetMtxWorldPtr());
	m_pProgressLamp->SetRot(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.5f));

	//初期化
	CSceneX::Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::Update()
{
	CSceneX::Update();

	//ボタンのステートが終了してる時
	if (m_buttonState == CModule_No4_4ColButton::STATE::END)
	{
		//return
		return;
	}

	//点灯カウントダウン
	m_nButtonLightingCnt--;

	//ボタンの状態に応じて処理を変える
	switch (m_buttonState)
	{
	case CModule_No4_4ColButton::STATE::START:
	case CModule_No4_4ColButton::STATE::INTERVAL:
		if (m_nButtonLightingCnt <= 0)
		{
			//点灯状態にする
			SetButtonState(STATE::LIGHTING);
		}
		break;

	case CModule_No4_4ColButton::STATE::PLAYER_INPUT:
	case CModule_No4_4ColButton::STATE::RESET_INTERVAL:
		if (m_nButtonLightingCnt <= 0)
		{
			//スタート状態にする
			SetButtonState(STATE::START);
		}
		break;
	}

	//if()


}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::Draw()
{
	CSceneX::Draw();

}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::ShowDebugInfo()
{
#ifdef _DEBUG
	if (!GetSelect())
	{
		//return;
	}
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "-------------Module_No_4----------------\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_nButtonLightingCnt [%d]\n", m_nButtonLightingCnt);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_nNowFlashNumber [%d]\n", m_nNowFlashNumber);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_nNowSelectButton [%d]\n", m_nNowSelectButton);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_nClearNum [%d]\n", m_nClearNum);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_buttonState [%d]\n", m_buttonState);

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_nPlayerPushNum [%d]\n", m_nPlayerPushNum);

	for (size_t nCnt = 0; nCnt < m_pColButtonList.size(); nCnt++)
	{
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "[%d] ", m_pColButtonList[nCnt]->GetClearFlag());
	}

	CDebugProc::Print(CDebugProc::PLACE_LEFT,NEWLINE);

	CModule_Base::ShowDebugInfo();

#endif //DEBUG
}

//------------------------------------------------------------------------------
//キーパッド操作
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::Operation_Keyboard()
{
	int nSelectNumOld = m_nNowSelectButton;

	//入力が無かった時はbreak
	CHossoLibrary::Selecting((int&)m_nNowSelectButton, nSelectNumOld, 2, 2);

	//選択解除
	CModule_Base::ModuleParts_Select<CModule_Parts_No4_ColButton>(m_pColButtonList, m_nNowSelectButton);

	//モジュール操作
	CModule_Base::Operation_Keyboard();
}

//------------------------------------------------------------------------------
//モジュール操作　マウス
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::Operation_Mouse()
{
	//レイの判定
	CHossoLibrary::RayCollision_ModuleSelect(m_pColButtonList.begin(), m_pColButtonList.end(), (int&)m_nNowSelectButton);

	//マウス操作
	CModule_Base::Operation_Mouse();

}

//------------------------------------------------------------------------------
//モジュールアクション
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::ModuleAction()
{
	//選択番号が-1とかだった時
	if (m_nNowSelectButton < 0)
	{
		return;
	}
	//すでに終了していた場合
	if (m_buttonState == STATE::END)
	{
		//ミス
		Module_Failed();
		return;
	}

	//nullcheck
	if (m_pColButtonList[m_nNowSelectButton].get())
	{
		//プレイヤーの入力状態
		SetButtonState(STATE::PLAYER_INPUT);
		m_pColButtonList[m_nNowSelectButton]->SetButtonLighting(true);

		//押したボタンがクリアボタンだった場合
		if (m_pColButtonList[m_nNowSelectButton]->GetClearFlag())
		{
			//正解のボタン押した
			ButtonPushSuccess();

			//音再生
			CManager::GetSound()->Play(CSound::LABEL_SE_MODULE_PUSH);

		}
		else
		{
			//失敗
			Module_Failed();
		}
	}
}

//------------------------------------------------------------------------------
//モジュールの選択解除
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::ModuleCancel()
{
	//選択解除
	CModule_Base::ModuleParts_Select<CModule_Parts_No4_ColButton>(m_pColButtonList, -1);
}

//------------------------------------------------------------------------------
//ボタンのステート設定
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::SetButtonState(STATE state)
{
	//ステート切り替え
	m_buttonState = state;

	switch (state)
	{
	case CModule_No4_4ColButton::STATE::START:
		m_nNowFlashNumber = -1;

		//プレイヤーの入力情報リセット
		PlayerInputReset();
		break;

	case CModule_No4_4ColButton::STATE::INTERVAL:
		//ボタンの点灯の間隔設定
		m_nButtonLightingCnt = COL_BUTTON_LIGHT_FLASH_INTERVAL;

		break;

	case CModule_No4_4ColButton::STATE::LIGHTING:
		m_nNowFlashNumber++;

		//基底回数以上にならないように設定
		CHossoLibrary::RangeLimit_Equal(m_nNowFlashNumber, 0, 3);

		//ボタンを光らせる
		m_pColButtonList[m_QuestionButtonList[m_nNowFlashNumber]]->SetButtonLighting(true);


		//点灯回数が規定までいったかどうか
		m_nClearNum > m_nNowFlashNumber ?
			SetButtonState(STATE::INTERVAL) :				//点灯のインターバル
			SetButtonState(STATE::RESET_INTERVAL);			//点灯終了

		break;

	case CModule_No4_4ColButton::STATE::PLAYER_INPUT:
		m_nButtonLightingCnt = INPUT_GRACE;

		break;

	case CModule_No4_4ColButton::STATE::RESET_INTERVAL:
		m_nButtonLightingCnt = COL_BUTTON_LIGHT_LOOP_INTERVAL;

		break;
	}
}

//------------------------------------------------------------------------------
//次のクリアボタン設定
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::NextButtonSet()
{
	for (size_t nCnt = 0; nCnt < m_pColButtonList.size(); nCnt++)
	{
		//フラグ初期化
		m_pColButtonList[nCnt]->SetClearFlag(false);
	}

	//次に押すボタン設定
	BUTTON NextButton = BUTTON::NONE;

	////バッテリーがついていた場合
	//if (CModule_Base::GetBombWeakPtr()._Get()->GetBombExterior()->CountExteriorNum(CBomb_Exterior::BATTERY_BIG) ||
	//	CModule_Base::GetBombWeakPtr()._Get()->GetBombExterior()->CountExteriorNum(CBomb_Exterior::BATTERY_SMALL))
	//{
	//	SetNextButton_YesBattery_NotSerialNo(NextButton);
	//}
	//else
	{
		SetNextButton_NotBattery_NotSerialNo(NextButton);

	}

	//次のボタンと同じボタンを検索
	auto itr = std::find_if(m_pColButtonList.begin(), m_pColButtonList.end(),
		[NextButton](S_ptr<CModule_Parts_No4_ColButton> &ColButton) {return ColButton->GetButton() == NextButton; });

	//結果が出た時
	if (itr != m_pColButtonList.end())
	{
		//クリアフラグ設定
		itr->get()->SetClearFlag(true);
	}
}
//------------------------------------------------------------------------------
//プレイヤーの入力関係の変数リセット
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::PlayerInputReset()
{
	m_nPlayerPushNum = 0;
	NextButtonSet();

}

//------------------------------------------------------------------------------
//正解のボタンを押した
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::ButtonPushSuccess()
{
	//プレイヤーのボタンクリア回数++
	m_nPlayerPushNum++;

	//クリア回数に到達したとき
	if (m_nPlayerPushNum > m_nClearNum)
	{
		//クリアに必要なキー数追加
		m_nClearNum++;

		//進捗ランプ更新
		m_pProgressLamp->SetProgress(m_nClearNum);
	}

	//モジュールクリアしてない時
	if (!CheckModuleClear())
	{
		//次のボタン設定
		NextButtonSet();
	}
}


//------------------------------------------------------------------------------
//モジュールクリアしたか確認
//------------------------------------------------------------------------------
bool CModule_No4_4ColButton::CheckModuleClear()
{
	//出題数とクリアしたボタン数比較
	if (m_nClearNum >= (int)m_QuestionButtonList.size())
	{
		//モジュールクリア
		Module_Clear();

		//終了
		SetButtonState(STATE::END);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
//次に押すボタン設定
//バッテリ有
//シリアルナンバー母音無し
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::SetNextButton_YesBattery_NotSerialNo(BUTTON & NextButton)
{
	switch (m_QuestionButtonList[m_nPlayerPushNum])
	{
	case BUTTON::RED:
		NextButton = BUTTON::YELLOW;
		break;

	case BUTTON::BLUE:
		NextButton = BUTTON::GREEN;
		break;

	case BUTTON::YELLOW:
		NextButton = BUTTON::BLUE;
		break;

	case BUTTON::GREEN:
		NextButton = BUTTON::RED;
		break;
	}
}

//------------------------------------------------------------------------------
//次に押すボタン設定
//バッテリ無
//シリアルナンバー母音無し
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::SetNextButton_NotBattery_NotSerialNo(BUTTON & NextButton)
{
	switch (m_QuestionButtonList[m_nPlayerPushNum])
	{
	case BUTTON::RED:
		NextButton = BUTTON::GREEN;
		break;

	case BUTTON::BLUE:
		NextButton = BUTTON::RED;
		break;

	case BUTTON::YELLOW:
		NextButton = BUTTON::YELLOW;
		break;

	case BUTTON::GREEN:
		NextButton = BUTTON::BLUE;
		break;

	}
}

