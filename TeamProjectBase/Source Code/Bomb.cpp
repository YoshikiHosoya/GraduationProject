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
	m_nSelectModuleNum = 0;
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

	ModuleClearCheck();
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
	//1F前の選択番号
	int m_SelectNumOld = m_nSelectModuleNum;

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
			//モジュール選択処理
			//入力が無かった時はbreak
			if (!CHossoLibrary::Selecting(m_nSelectModuleNum, m_SelectNumOld, 3, 4))
			{
				break;
			}

			//nullcheck
			if (m_pModuleList[m_nSelectModuleNum].get())
			{
				if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
				{
					break;
				}
			}
		}

		for (int nCnt = 0; nCnt < (int)m_pModuleList.size(); nCnt++)
		{
			//nullcheck
			if (m_pModuleList[nCnt].get())
			{
				//現在の選択番号と同じモノだけtrueにしておく
				nCnt == m_nSelectModuleNum ?
					m_pModuleList[nCnt]->SetSelect(true) :
					m_pModuleList[nCnt]->SetSelect(false);
			}
		}


		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			CManager::GetGame()->SetGaze(CGame::GAZE::GAZE_MODULE);

			//カメラを近づける
			m_pModuleList[m_nSelectModuleNum]->CameraApproach();
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
		if (m_pModuleList[m_nSelectModuleNum].get())
		{
			m_pModuleList[m_nSelectModuleNum]->Operation();
		}

		break;

	default:
		break;
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
//モジュールクリアしたかチェック
//------------------------------------------------------------------------------
void CBomb::ModuleClearCheck()
{
	//クリアフラグの個数がモジュール数以上になった時
	if (std::count_if(m_pModuleList.begin(), m_pModuleList.end(),
		[](std::shared_ptr<CModule_Base> pModule) {return pModule->GetModuleClearFlag(); }) >= m_nModuleNum)
	{
		CManager::GetGame()->SetState(CGame::STATE_GAMECLEAR);
	}
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

	//CreateModule_Random(m_nModuleNum);

//Debug用
#ifdef _DEBUG
	CreateModuleDebug();
#endif //_DEBUG

	//最初の選択番号設定
	for (size_t nCnt = 0; nCnt < m_pModuleList.size(); nCnt++)
	{
		if (m_pModuleList[nCnt]->GetCanModuleSelect())
		{
			m_nSelectModuleNum = nCnt;
			break;
		}
	}
}

//------------------------------------------------------------------------------
//モジュール生成　ランダム生成
//------------------------------------------------------------------------------
void CBomb::CreateModule_Random(int const nModuleNum)
{
	//タイマー生成
	CBomb::CreateModuleOne<CModule_Timer>();

	int nCntModule = 0;

	//モジュール数分に達するまで
	while ((int)m_pModuleList.size() < MAX_MODULE_NUM)
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
			if (nCntModule < m_nModuleNum)
			{
				CBomb::CreateModuleOne<CModule_KeyPad>();
				m_nModuleNum++;
			}
			break;
		}
	}
}


#ifdef _DEBUG
//------------------------------------------------------------------------------
//モジュール生成　デバッグ用　自分でモジュールを決めれる
//------------------------------------------------------------------------------
void CBomb::CreateModuleDebug()
{
	//モジュール数をここで決める debug用
	m_nModuleNum = 2;


	//1番目
	CBomb::CreateModuleOne<CModule_Timer>();
	//2番目
	CBomb::CreateModuleOne<CModule_KeyPad>();
	//3番目
	CBomb::CreateModuleOne<CModule_KeyPad>();
	//4番目
	CBomb::CreateModuleOne<CModule_None>();
	//5番目
	CBomb::CreateModuleOne<CModule_None>();
	//6番目
	CBomb::CreateModuleOne<CModule_None>();

	//7番目
	CBomb::CreateModuleOne<CModule_None>();
	//8番目
	CBomb::CreateModuleOne<CModule_None>();
	//9番目
	CBomb::CreateModuleOne<CModule_None>();
	//10番目
	CBomb::CreateModuleOne<CModule_None>();
	//11番目
	CBomb::CreateModuleOne<CModule_None>();
	//12番目
	CBomb::CreateModuleOne<CModule_None>();
}
#endif //_DEBUG
