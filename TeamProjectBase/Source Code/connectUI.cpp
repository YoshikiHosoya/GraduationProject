//=============================================================================
//
// 接続のUI処理 [ connectUI.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "connectUI.h"
#include "polygon2D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define INTERVAL_ANIM_LOADICON	(10)	// アニメーション処理の間隔
#define NUM_ANIMPATTER_LOADICON	(8)	// ロードアイコンのパターン数

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
CPolygon2D *CConnectUI::m_pConnectUI[CConnectUI::CONNECTUI_MAX] = {};
D3DXVECTOR2 CConnectUI::m_UIPos[CConnectUI::CONNECTUI_MAX] = 
{
	D3DXVECTOR2(SCREEN_WIDTH / 2 - 260.0f, SCREEN_HEIGHT / 2),
	D3DXVECTOR2(SCREEN_WIDTH / 2 + 260.0f, SCREEN_HEIGHT / 2),
	D3DXVECTOR2(SCREEN_WIDTH / 2 - 260.0f, SCREEN_HEIGHT / 2),
	D3DXVECTOR2(SCREEN_WIDTH / 2 + 260.0f, SCREEN_HEIGHT / 2)
};

D3DXVECTOR2 CConnectUI::m_UISize[CConnectUI::CONNECTUI_MAX] =
{
	D3DXVECTOR2(500.0f, 700.0f),
	D3DXVECTOR2(500.0f, 700.0f),
	D3DXVECTOR2(50.0f, 50.0f),
	D3DXVECTOR2(50.0f, 50.0f)
};

//=============================================================================
// コンストラクタ
//=============================================================================
CConnectUI::CConnectUI()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CConnectUI::~CConnectUI()
{
	for (int nCnt = 0; nCnt < CONNECTUI_MAX; nCnt++)
	{
		if (m_pConnectUI[nCnt])
		{
			delete m_pConnectUI[nCnt];
			m_pConnectUI[nCnt] = nullptr;
		}
	}
}

//=============================================================================
// 生成
//=============================================================================
CConnectUI *CConnectUI::Create(void)
{
	// メモリ確保
	CConnectUI* pUI = new CConnectUI;

	// 確保失敗
	if (!pUI)
		return nullptr;

	// 初期化
	pUI->Init();

	// 値を返す
	return pUI;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CConnectUI::Init(void)
{
	m_nCntAnim = 0;
	m_nCntPattern = 0;
	// 背景生成
	CreateBackTab();
	// ロードアイコン生成
	CreateLoadIcon();

	return S_OK;
}

//=============================================================================
// 更新
//=============================================================================
void CConnectUI::Update(void)
{
	if (m_pConnectUI[CONNECTUI_LOADICON_00])
		UpdateCntAnim();
	else if (m_pConnectUI[CONNECTUI_LOADICON_01])
		UpdateCntAnim();

	for (int nCnt = 0; nCnt < CONNECTUI_MAX; nCnt++)
	{
		if (!m_pConnectUI[nCnt])
			return;

		// ロードアイコンのアニメーション
		if (nCnt == CONNECTUI_LOADICON_00 || 
			nCnt == CONNECTUI_LOADICON_01)
			m_pConnectUI[nCnt]->SetAnim(D3DXVECTOR2(0.0f + (1.0f / NUM_ANIMPATTER_LOADICON * m_nCntPattern), 0.0f), D3DXVECTOR2(1.0f / NUM_ANIMPATTER_LOADICON, 1.0f));

		m_pConnectUI[nCnt]->Update();
	}
}

//=============================================================================
// 描画
//=============================================================================
void CConnectUI::Draw(void)
{
	for (int nCnt = 0; nCnt < CONNECTUI_MAX; nCnt++)
	{
		if (m_pConnectUI[nCnt])
			m_pConnectUI[nCnt]->Draw();
	}
}

//=============================================================================
// UIの生成
//=============================================================================
void CConnectUI::CreateLoadIcon(void)
{
	for (int nCnt = CONNECTUI_LOADICON_00; nCnt < CONNECTUI_LOADICON_00 + 2; nCnt++)
	{
		m_pConnectUI[nCnt] = CPolygon2D::Create();
		m_pConnectUI[nCnt]->SetPos(m_UIPos[nCnt]);
		m_pConnectUI[nCnt]->SetSize(m_UISize[nCnt]);
		m_pConnectUI[nCnt]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f / NUM_ANIMPATTER_LOADICON, 1.0f));
		m_pConnectUI[nCnt]->SetCol(BlackColor);
		m_pConnectUI[nCnt]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
		m_pConnectUI[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_LOAD));
	}
}

//=============================================================================
// 背景の生成
//=============================================================================
void CConnectUI::CreateBackTab(void)
{
	for (int nCnt = CONNECTUI_BACK_00; nCnt < CONNECTUI_BACK_00 + 2; nCnt++)
	{
		m_pConnectUI[nCnt] = CPolygon2D::Create();
		m_pConnectUI[nCnt]->SetPos(m_UIPos[nCnt]);
		m_pConnectUI[nCnt]->SetSize(m_UISize[nCnt]);
		m_pConnectUI[nCnt]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
	}

	// テクスチャのバインド
	m_pConnectUI[CONNECTUI_BACK_00]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_BACK_00));
	m_pConnectUI[CONNECTUI_BACK_01]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_BACK_01));
}

//=============================================================================
// アニメーションカウンタの更新
//=============================================================================
void CConnectUI::UpdateCntAnim(void)
{
	// 時間でないなら、処理を終える
	m_nCntAnim++;
	if (m_nCntAnim < INTERVAL_ANIM_LOADICON)
		return;

	// 一定フレームおきにアニメーション
	m_nCntPattern++;
	if (m_nCntPattern >= NUM_ANIMPATTER_LOADICON)
		m_nCntPattern = 0;
	m_nCntAnim = 0;
}
