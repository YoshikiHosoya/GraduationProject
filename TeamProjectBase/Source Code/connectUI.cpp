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
#define TIME_ANIM_LOADICON	(7)	// アニメーション処理の間隔
#define NUM_ANIMPATTER_LOADICON	(8)	// ロードアイコンのパターン数

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================

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
	// UI生成
	CreateUI();

	return S_OK;
}

//=============================================================================
// 更新
//=============================================================================
void CConnectUI::Update(void)
{
	for (int nCnt = 0; nCnt < CONNECTUI_MAX; nCnt++)
	{
		if (!m_pConnectUI[nCnt])
			return;

		if (nCnt == CONNECTUI_LOADICON)
			LoadAnim(m_pConnectUI[nCnt]);

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
void CConnectUI::CreateUI(void)
{
	m_pConnectUI[CONNECTUI_LOADICON] = CPolygon2D::Create();
	m_pConnectUI[CONNECTUI_LOADICON]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pConnectUI[CONNECTUI_LOADICON]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	m_pConnectUI[CONNECTUI_LOADICON]->SetAnim(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f / NUM_ANIMPATTER_LOADICON, 1.0f));
	m_pConnectUI[CONNECTUI_LOADICON]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
	m_pConnectUI[CONNECTUI_LOADICON]->BindTexture(CTexture::GetTexture(CTexture::TEX_CONNECT_LOAD));
}

//=============================================================================
// ロードアイコンのアニメーション
//=============================================================================
void CConnectUI::LoadAnim(CPolygon2D *pUI)
{
	static int nCntAnim = 0;
	static int nCntPat = 0;

	// 時間でないなら、処理を終える
	nCntAnim++;
	if (nCntAnim < TIME_ANIM_LOADICON)
		return;

	// 一定フレームおきにアニメーション
	nCntPat++;
	if (nCntPat >= NUM_ANIMPATTER_LOADICON)
		nCntPat = 0;
	nCntAnim = 0;
	pUI->SetAnim(D3DXVECTOR2(0.0f + (1.0f / NUM_ANIMPATTER_LOADICON * nCntPat), 0.0f), D3DXVECTOR2(1.0f / NUM_ANIMPATTER_LOADICON, 1.0f));
}
