//------------------------------------------------------------------------------
//
//爆弾の処理  [Bomb.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "Bomb.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "game.h"
#include "modelinfo.h"
#include "module_none.h"
#include "module_timer.h"
#include "module_Button.h"
#include "module_keypad.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CBomb::CBomb()
{
	m_nModuleNum = 0;
	m_pModuleList = {};
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CBomb::~CBomb()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CBomb::Init()
{
	CSceneX::Init();
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CBomb::Uninit()
{
	CSceneX::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CBomb::Update()
{
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CBomb::Draw()
{
	CSceneX::Draw();

}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CBomb::ShowDebugInfo()
{
#ifdef _DEBUG

	//選択番号
	static  int nSelectNum = 0;
	//1F前の選択番号
	int nSelectNumOld = nSelectNum;

	//配列が空だったらreturn
	if (m_pModuleList.empty())
	{
		return;
	}

	//キーボードのポインタ
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	switch (CManager::GetGame()->GetGaze())
	{
	case CGame::GAZE_DEFAULT:
		break;

	case CGame::GAZE_BOMB:
		while (1)
		{
			//入力が無かった時はbreak
			if (!CHossoLibrary::Selecting(nSelectNum, nSelectNumOld, 3, 4))
			{
				break;
			}

			//nullcheck
			if (m_pModuleList[nSelectNum].get())
			{
				if (m_pModuleList[nSelectNum]->GetCanModuleSelect())
				{
					break;
				}
			}
		}

		for (int nCnt = 0; nCnt < 12; nCnt++)
		{
			//nullcheck
			if (m_pModuleList[nCnt].get())
			{
				//現在の選択番号と同じモノだけtrueにしておく
				nCnt == nSelectNum ?
					m_pModuleList[nCnt]->SetSelect(true) :
					m_pModuleList[nCnt]->SetSelect(false);
			}
		}


		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			CManager::GetGame()->SetGaze(CGame::GAZE::GAZE_MODULE);

			//カメラを近づける
			m_pModuleList[nSelectNum]->CameraApproach();
		}

		break;

	case CGame::GAZE_MODULE:
		for (int nCnt = 0; nCnt < 12; nCnt++)
		{
			//nullcheck
			if (m_pModuleList[nCnt].get())
			{
				m_pModuleList[nCnt]->SetSelect(false);

			}
		}
		//nullcheck
		if (m_pModuleList[nSelectNum].get())
		{
			m_pModuleList[nSelectNum]->Operation();
		}

		break;

	default:
		break;
	}



	//左のCtrlキー
	if (pKeyboard->GetPress(DIK_LCONTROL))
	{
		for (int nCnt = 0; nCnt < 12; nCnt++)
		{
			//数字
			if (pKeyboard->GetTrigger(0x01 + nCnt))
			{
				//nullcheck
				if (m_pModuleList[nCnt].get())
				{
					//モジュールクリア
					m_pModuleList[nCnt]->Module_Clear();
				}
			}
		}
	}

#endif //DEBUG
}

//------------------------------------------------------------------------------
//生成関数
//------------------------------------------------------------------------------
std::shared_ptr<CBomb> CBomb::CreateBomb(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot, int const nModuleNum)
{
	//メモリ確保
	std::shared_ptr<CBomb> pBomb = std::make_shared<CBomb>();

	//初期化
	pBomb->Init();

	//座標とサイズ設定
	pBomb->SetPos(pos);
	pBomb->SetRot(rot);

	//モデル情報設定
	pBomb->BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_BOMBBOX));

	//モジュール生成
	pBomb->CreateModule(nModuleNum);

	//Scene側で管理
	pBomb->SetObjType(CScene::OBJTYPE_BOMB);
	pBomb->AddSharedList(pBomb);

	return pBomb;
}

//------------------------------------------------------------------------------
//モジュール生成
//------------------------------------------------------------------------------
void CBomb::CreateModule(int const nModuleNum)
{
	//モデル数代入
	m_nModuleNum = nModuleNum;

	//もしモジュールを表示できる範囲外の時は収める
	CHossoLibrary::RangeLimit_Equal(m_nModuleNum, 0, MAX_MODULE_NUM);

	//タイマー生成
	CBomb::CreateModuleOne<CModule_Timer>();

	//モジュール数分に達するまで
	while ((int)m_pModuleList.size() < m_nModuleNum)
	{
		//モジュールタイプをランダムに
		CModule_Base::MODULE_TYPE type = (CModule_Base::MODULE_TYPE)(rand() % (int)CModule_Base::MODULE_TYPE::MAX);

		//モジュールタイプに応じて生成
		switch (type)
		{
			//モジュール無し
		case CModule_Base::MODULE_TYPE::NONE:
			CBomb::CreateModuleOne<CModule_None>();
			break;

		//	//ボタンモジュール
		//case CModule_Base::MODULE_TYPE::BUTTON:
		//	CBomb::CreateModuleOne<CModule_Button>();
		//	break;

			//キーパッド
		case CModule_Base::MODULE_TYPE::KEYPAD:
			CBomb::CreateModuleOne<CModule_KeyPad>();
			break;
		}
	}
//Debug用
#ifdef _DEBUG
	////1番目
	//CBomb::CreateModuleOne<CModule_Timer>();
	////2番目
	//CBomb::CreateModuleOne<CModule_Button>();
	////3番目
	//CBomb::CreateModuleOne<CModule_Button>();
	////4番目
	//CBomb::CreateModuleOne<CModule_Button>();
	////5番目
	//CBomb::CreateModuleOne<CModule_Button>();
	////6番目
	//CBomb::CreateModuleOne<CModule_Button>();

	////7番目
	//CBomb::CreateModuleOne<CModule_Button>();
	////8番目
	//CBomb::CreateModuleOne<CModule_Button>();
	////9番目
	//CBomb::CreateModuleOne<CModule_Button>();
	////10番目
	//CBomb::CreateModuleOne<CModule_Button>();
	////11番目
	//CBomb::CreateModuleOne<CModule_Button>();
	////12番目
	//CBomb::CreateModuleOne<CModule_Button>();

#endif //_DEBUG
}
