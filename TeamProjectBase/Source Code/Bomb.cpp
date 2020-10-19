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
#include "modelinfo.h"
#include "module_none.h"
#include "module_timer.h"
#include "module_Button.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
//#define MODULE_INTERVAL (D3DXVECTOR3(120.0f,50.0f,60.0f))
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

	static  int nSelectNum = 0;


	//配列が空だったらreturn
	if (m_pModuleList.empty())
	{
		return;
	}

	//キーボードのポインタ
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_LEFT))
	{
		nSelectNum--;
	}
	if (pKeyboard->GetTrigger(DIK_RIGHT))
	{
		nSelectNum++;
	}

	//範囲内に抑える
	CHossoLibrary::RangeLimit_Equal(nSelectNum, 0, (int)m_pModuleList.size() - 1);

	for (int nCnt = 0; nCnt < 12; nCnt++)
	{

		//nullcheck
		if (m_pModuleList[nCnt].get())
		{
			//現在の選択番号とあっていた場合はtrueにしておく
			nCnt == nSelectNum ?
				m_pModuleList[nCnt]->SetSelect(true) :
				m_pModuleList[nCnt]->SetSelect(false);

		}
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
std::shared_ptr<CBomb> CBomb::Create(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot, int const nModuleNum)
{
	//メモリ確保
	std::shared_ptr<CBomb> pBomb = std::make_shared<CBomb>();

	//初期化
	pBomb->Init();

	//座標とサイズ設定
	pBomb->SetPos(pos);
	pBomb->SetRot(rot);

	//モデル情報設定
	pBomb->BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_BOMBOBJECT_BOMB));

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
//	int nCnt = 0;
//	m_nModuleNum = nModuleNum;
//
//	while(nCnt < nModuleNum)
//	{
//		int nSetNumber = nCnt;
//
//		int nX = nSetNumber % 3;
//		int nY = nSetNumber / 3;
//		int nZ = nSetNumber / 6;
//
//		m_pModuleList.emplace_back(CModule_Base::Create<CModule_Timer>
//		(D3DXVECTOR3(	(-MODULE_INTERVAL.x + (MODULE_INTERVAL.x * nX) ) * (1 - (nZ * 2)),
//					MODULE_INTERVAL.y - (MODULE_INTERVAL.y * ((nY % 2) * 2)),
//					-MODULE_INTERVAL.z + (MODULE_INTERVAL.z * nZ * 2)),
//		D3DXVECTOR3(0.0f,(D3DX_PI) * nZ,0.0f),GetMtxWorldPtr()));
//
//		nCnt++;
//	}

//Debug用
#ifdef _DEBUG
	//1番目
	CBomb::CreateModuleOne<CModule_Timer>(0);
	//2番目
	CBomb::CreateModuleOne<CModule_Button>(1);
	//3番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_None>(D3DXVECTOR3(MODULE_INTERVAL.x,		MODULE_INTERVAL.y,	-MODULE_INTERVAL.z), ZeroVector3, GetMtxWorldPtr()));
	//4番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_Button>(D3DXVECTOR3(-MODULE_INTERVAL.x,		-MODULE_INTERVAL.y, -MODULE_INTERVAL.z), ZeroVector3, GetMtxWorldPtr()));
	//5番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_Button>(D3DXVECTOR3(0.0f,					-MODULE_INTERVAL.y, -MODULE_INTERVAL.z), ZeroVector3, GetMtxWorldPtr()));
	//6番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_Button>(D3DXVECTOR3(MODULE_INTERVAL.x,		-MODULE_INTERVAL.y, -MODULE_INTERVAL.z), ZeroVector3, GetMtxWorldPtr()));

	//7番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_Button>(D3DXVECTOR3(MODULE_INTERVAL.x,		MODULE_INTERVAL.y,	MODULE_INTERVAL.z), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), GetMtxWorldPtr()));
	//8番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_Button>(D3DXVECTOR3(0.0f,					MODULE_INTERVAL.y,	MODULE_INTERVAL.z), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), GetMtxWorldPtr()));
	//9番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_Button>(D3DXVECTOR3(-MODULE_INTERVAL.x,		MODULE_INTERVAL.y,	MODULE_INTERVAL.z), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), GetMtxWorldPtr()));
	//10番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_Button>(D3DXVECTOR3(MODULE_INTERVAL.x,		-MODULE_INTERVAL.y, MODULE_INTERVAL.z), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), GetMtxWorldPtr()));
	//11番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_Button>(D3DXVECTOR3(0.0f,					-MODULE_INTERVAL.y, MODULE_INTERVAL.z), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), GetMtxWorldPtr()));
	//12番目
	m_pModuleList.emplace_back(CModule_Base::Create<CModule_Button>(D3DXVECTOR3(-MODULE_INTERVAL.x,		-MODULE_INTERVAL.y, MODULE_INTERVAL.z), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), GetMtxWorldPtr()));

#endif //_DEBUG
}
