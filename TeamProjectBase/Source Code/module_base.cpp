//------------------------------------------------------------------------------
//
//モジュールの基底クラス  [module_base.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_base.h"
#include "module_parts_lamp.h"
#include "modelinfo.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "mouse.h"
#include "camera.h"
#include "Bomb.h"
#include "game.h"
#include "sound.h"
#include "chatTab.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define LAMP_OFFSET_POS (D3DXVECTOR3(40.0f, 30.0f, -20.0f))
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_Base::CModule_Base()
{
	m_bModuleClearFlag = false;
	m_bOperating = false;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_Base::~CModule_Base()
{
	m_pLamp.reset();
}

//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_Base::ShowDebugInfo()
{
	ImGui::DragFloat3("BombPos", GetPos());

}

//------------------------------------------------------------------------------
//ランプ生成
//------------------------------------------------------------------------------
void CModule_Base::Operation_Keyboard()
{
	//決定キー
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		//モジュールのアクション
		ModuleAction();
	}

	//キャンセル
	if (CManager::GetKeyboard()->GetTrigger(DIK_BACKSPACE))
	{
		//モジュールの選択解除
		ModuleCancel();

		//ゲームの視点変更
		CManager::GetGame()->SetGaze(CGame::GAZE_BOMB);
	}
}

//------------------------------------------------------------------------------
//ランプ生成
//------------------------------------------------------------------------------
void CModule_Base::Operation_Mouse()
{
	//左クリック
	if (CManager::GetMouse()->GetTrigger(0))
	{
		//モジュールのアクション
		ModuleAction();

		//もしモジュール内をクリックしていなかった場合
		if (!CHossoLibrary::MouseRayCollision_Boolean(GetMtxWorldPtr(), GetModelInfo()->GetMesh()))
		{
			//チャットタブらへんをクリックしていない時
			if (!(/*CManager::GetGame()->GetChatBase()->GetTabPtr()->CheckMouseHit(CChatTab::POLY_TABLET) ||*/
				CManager::GetGame()->GetChatBase()->GetTabPtr()->CheckMouseHit(CChatTab::POLY_TAB)))
			{
				//モジュールの選択解除
				ModuleCancel();

				//ゲームの視点変更
				CManager::GetGame()->SetGaze(CGame::GAZE_BOMB);
			}
		}
	}

	//キャンセル
	if (CManager::GetKeyboard()->GetTrigger(DIK_BACKSPACE))
	{
		//モジュールの選択解除
		ModuleCancel();

		//ゲームの視点変更
		CManager::GetGame()->SetGaze(CGame::GAZE_BOMB);
	}

}

//------------------------------------------------------------------------------
//ランプ生成
//------------------------------------------------------------------------------
void CModule_Base::CreateLamp()
{
	//ランプ生成
	m_pLamp = CModule_Parts_Base::Create_ModuleParts<CModule_Parts_Lamp>(LAMP_OFFSET_POS, GetMtxWorldPtr());
}

//------------------------------------------------------------------------------
//カメラを近づける
//------------------------------------------------------------------------------
void CModule_Base::CameraApproach()
{
	//カメラを近づける
	CManager::GetRenderer()->GetCamera()->ApproachCamera(CHossoLibrary::CalcMtxToVector3(*GetMtxWorldPtr()));
}

//------------------------------------------------------------------------------
//モジュールクリア
//------------------------------------------------------------------------------
void CModule_Base::Module_Clear()
{
	//クリアフラグtrue
	m_bModuleClearFlag = true;

	//nullcheck
	if (m_pLamp)
	{
		m_pLamp->SetLampState(CModule_Parts_Lamp::LAMP_STATE::GREEN);
	}

	//ボムのポインタが開放されていないか
	if (!m_pBomb.expired())
	{
		//クリアチェック
		m_pBomb.lock()->ModuleClearCheck();
	}
}

//------------------------------------------------------------------------------
//モジュール失敗
//------------------------------------------------------------------------------
void CModule_Base::Module_Failed()
{
	//nullcheck
	if (m_pLamp)
	{
		m_pLamp->SetLampState(CModule_Parts_Lamp::LAMP_STATE::RED);
	}

	//ボムのポインタが開放されていないか
	if (!m_pBomb.expired())
	{
		//モジュールミスった
		m_pBomb.lock()->ModuleMiss();
	}
}

