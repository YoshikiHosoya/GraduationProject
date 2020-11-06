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
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "modelinfo.h"
#include "timer.h"
#include "game.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define COL_BUTTON_NUM									(4)										//ボタン数
#define COL_BUTTON_OFFSET								(D3DXVECTOR3(0.0f,0.0f,-25.0f))			//ボタンのオフセット座標
#define COL_BUTTON_LIGHT_FLASH_INTERVAL					(45)									//ボタンの点灯の感覚
#define COL_BUTTON_LIGHT_LOOP_INTERVAL					(180)									//ボタンのループ間隔

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

	m_nPlayerPushNum = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_No4_4ColButton::~CModule_No4_4ColButton()
{
	m_pColButtonList.clear();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_No4_4ColButton::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_DEFAULT));

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
		m_QuestionButtonList.emplace_back((BUTTON)nCnt);

		nCnt++;
	}

	//次のボタン設定
	NextButtonSet();

	//ランプ生成
	CModule_Base::CreateLamp();

	CSceneX::Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::Update()
{
	CSceneX::Update();

	if (m_buttonState == CModule_No4_4ColButton::STATE::END)
	{
		return;
	}

	m_nButtonLightingCnt--;

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


#endif //DEBUG
}
//------------------------------------------------------------------------------
//キーパッド操作
//------------------------------------------------------------------------------
void CModule_No4_4ColButton::Operation()
{
	int nSelectNumOld = m_nNowSelectButton;

	//入力が無かった時はbreak
	CHossoLibrary::Selecting((int&)m_nNowSelectButton, nSelectNumOld, 2, 2);

	for (size_t nCnt = 0; nCnt < m_pColButtonList.size(); nCnt++)
	{
		//nullcheck
		if (m_pColButtonList[nCnt].get())
		{
			//現在の選択番号と同じモノだけtrueにしておく
			nCnt == m_nNowSelectButton ?
				m_pColButtonList[nCnt]->SetSelect(true) :
				m_pColButtonList[nCnt]->SetSelect(false);
		}
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
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
			}
			else
			{
				//失敗
				Module_Failed();
			}
		}
	}

	//nullcheck
	if (CManager::GetKeyboard()->GetTrigger(DIK_BACKSPACE))
	{
		for (size_t nCnt = 0; nCnt < m_pColButtonList.size(); nCnt++)
		{
			if (m_pColButtonList[nCnt].get())
			{
				//現在の選択番号と同じモノだけtrueにしておく
				m_pColButtonList[nCnt]->SetSelect(false);

			}
		}
		CManager::GetGame()->SetGaze(CGame::GAZE_BOMB);
	}
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
		PlayerInputReset();
		break;

	case CModule_No4_4ColButton::STATE::INTERVAL:
		m_nButtonLightingCnt = COL_BUTTON_LIGHT_FLASH_INTERVAL;

		break;

	case CModule_No4_4ColButton::STATE::LIGHTING:
		m_nNowFlashNumber++;
		CHossoLibrary::RangeLimit_Equal(m_nNowFlashNumber, 0, 3);
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

	BUTTON NextButton = BUTTON::RED;

	switch (m_QuestionButtonList[m_nPlayerPushNum])
	{
	case BUTTON::RED:
		NextButton = BUTTON::GREEN;
		break;

	case BUTTON::GREEN:
		NextButton = BUTTON::BLUE;
		break;

	case BUTTON::BLUE:
		NextButton = BUTTON::RED;
		break;

	case BUTTON::YELLOW:
		NextButton = BUTTON::YELLOW;
		break;
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
	m_nPlayerPushNum++;

	if (m_nPlayerPushNum > m_nClearNum)
	{
		m_nClearNum++;
	}

	if (!CheckModuleClear())
	{
		NextButtonSet();
	}
}


//------------------------------------------------------------------------------
//次のクリアボタン設定
//------------------------------------------------------------------------------
bool CModule_No4_4ColButton::CheckModuleClear()
{
	if (m_nClearNum >= (int)m_QuestionButtonList.size())
	{
		Module_Clear();
		SetButtonState(STATE::END);
		return true;
	}
	return false;
}

