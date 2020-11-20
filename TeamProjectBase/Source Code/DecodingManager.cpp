//*************************************************************************************************************
//
// 解読書のマネージャー[処理 [DecodingManager.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "DecodingManager.h"
#include "DecodingUI.h"
#include "myhash.h"
#include "texture.h"
#include "mouse.h"
#include "manager.h"
#include "DecodingWindow.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define DECODINGMANA_FILENAME "data/TEXT/DecodingUI_Info.txt"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバの初期化
//-------------------------------------------------------------------------------------------------------------
CHash *            CDecodingManager::m_pHash = nullptr;										// ハッシュポインタ
LPDIRECT3DTEXTURE9 CDecodingManager::m_pTexture[TEX_MAX] = Mybfunc_array(nullptr);	// テクスチャ情報
SETINGINFO         CDecodingManager::m_Seting[UI_MAX];										// 設定情報
int                CDecodingManager::m_nFrameMax = MYLIB_INT_UNSET;

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CDecodingManager::CDecodingManager()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CDecodingManager::~CDecodingManager()
{
}

//-------------------------------------------------------------------------------------------------------------
// ハッシュの作成
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::MakeHash(void)
{
	// キーデータ
	char * Key[UI_MAX] =
	{
		{ "Backlight" },
		{ "TellThePicture" },
		{ "ShapeMemorization" },
		{ "CordCutting" },
		{ "Maze" },
		{ "Simon" },
		{ "Nowmaking" },
		{ "Document" },
		{ "Close" },
		{ "ScrollBar" },
		{ "ScrollHandle" },
	};
	// データ
	char * Data[UI_MAX] =
	{
		{ "0" },
		{ "1" },
		{ "2" },
		{ "3" },
		{ "4" },
		{ "5" },
		{ "6" },
		{ "7" },
		{ "8" },
		{ "9" },
		{ "10" },
	};
	// ハッシュの作成
	m_pHash = CHash::Create();

	// ハッシュに挿入する
	for (int nCntHash = 0; nCntHash < UI_MAX; nCntHash++)
	{
		m_pHash->Insert(Key[nCntHash], Data[nCntHash]);
	}

	// ウィンドのハッシュの作成
	CDecodingWindow::MakeHash();
}

//-------------------------------------------------------------------------------------------------------------
// ハッシュの開放
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::ReleaseHash(void)
{
	// ウィンドウのハッシュの開放
	CDecodingWindow::ReleaseHash();

	// nullチェック
	if (m_pHash != nullptr)
	{
		// ハッシュテーブルの開放
		m_pHash->ReleaseHashtable();
		// ハッシュの破棄
		delete m_pHash;
		m_pHash = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CDecodingManager::Load(void)
{
	// 


	// テクスチャ情報を取得する
	m_pTexture[TEX_TELLTHEPICTURE]      = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T00);
	m_pTexture[TEX_SHAPEMEMORIZATION]   = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T01);
	m_pTexture[TEX_CORDCUTTING]         = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T02);
	m_pTexture[TEX_MAZE]                = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T03);
	m_pTexture[TEX_SIMON]               = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T04);
	m_pTexture[TEX_NOWMAKING]           = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T05);
	m_pTexture[TEX_BACKLIGHT]           = CTexture::GetTexture(CTexture::TEX_UI_DECODING_BACKLIGHT);
	m_pTexture[TEX_D_TELLTHEPICTURE]    = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D00);
	m_pTexture[TEX_D_SHAPEMEMORIZATION] = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D01);
	m_pTexture[TEX_D_CORDCUTTING]       = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D02);
	m_pTexture[TEX_D_MAZE]              = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D03);
	m_pTexture[TEX_D_SIMON]             = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D04);
	m_pTexture[TEX_D_NOWMAKING]         = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D05);
	m_pTexture[TEX_CLOSE]               = CTexture::GetTexture(CTexture::TEX_UI_DECODING_CLOSE);
	m_pTexture[TEX_SCROLLBAR]           = CTexture::GetTexture(CTexture::TEX_UI_DECODING_S_BAR);
	m_pTexture[TEX_SCROLLHANDLE]        = CTexture::GetTexture(CTexture::TEX_UI_DECODING_S_HANDLE);

	// 設定情報の初期化
	for (int nCntSet = 0; nCntSet < UI_MAX ; nCntSet++)
	{
		m_Seting[nCntSet] = SETINGINFO();
	}

	// ファイル読み込み
	if (CLoadFile::ReadLineByLineFromFile(DECODINGMANA_FILENAME, ReadFromLine) != CLoadFile::LR_SUCCESS)
	{
#ifdef _DEBUG
		std::cout << "<<<<<<CDecodingManagerのファイル読み込みが失敗しました。>>>>>>\n";
#endif // _DEBUG
		return E_FAIL;
	}
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::Unload(void)
{
	for (int nCntUI = 0; nCntUI < TEX_MAX; nCntUI++)
	{
		m_pTexture[nCntUI] = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
std::shared_ptr<CDecodingManager> CDecodingManager::Create(void)
{
	// スマートポインタの生成
	std::shared_ptr<CDecodingManager> pManager = std::make_shared<CDecodingManager>();
	// 初期化
	pManager->Init();

	//Sceneで管理
	pManager->SetObjType(OBJTYPE_UI);
	pManager->AddSharedList(pManager);
	return pManager;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CDecodingManager::Init()
{
	// UIの生成
	CreateUi();

	RANGE ra;
	// スクロールハンドルの大きさの設定を変更
	m_pUi[UI_SCROLLHANDLE]->SetSize(D3DXVECTOR2(m_pUi[UI_SCROLLHANDLE]->GetSize().x, m_Seting[UI_SCROLLBAR].size.y * 0.75f));
	m_Seting[UI_SCROLLHANDLE].size.y = m_Seting[UI_SCROLLBAR].size.y * 0.75f;

	// 可動域の計算
	m_ScrollRange.fMax = m_Seting[UI_SCROLLBAR].size.y*MYLIB_HALF_SIZE - (m_Seting[UI_SCROLLHANDLE].size.y*MYLIB_HALF_SIZE);
	m_ScrollRange.fMin = -m_Seting[UI_SCROLLBAR].size.y*MYLIB_HALF_SIZE + (m_Seting[UI_SCROLLHANDLE].size.y*MYLIB_HALF_SIZE);
	m_fScrollRangeValue = m_ScrollRange.GetFloatValue();

	// スクロールハンドルの位置の設定を変更
	m_Seting[UI_SCROLLHANDLE].pos.y = m_Seting[UI_SCROLLBAR].pos.y + m_ScrollRange.fMin - m_Seting[UI_SCROLLBAR].pos.y;

	// 親をもとに位置を設定する
	SetPosAccordingParent();

	// ドキュメントのUV情報の初期化
	InitDocumentUVInfo();

	// 選択している番号の初期化
	m_nSelectIndex = -1;

	// モードの初期化
	m_mode = MODE_SELECT;
	m_modeNext = m_mode;

	// 接続モードの初期化
	InitConnect();

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::Update()
{
	// モード別の処理
	switch (m_mode)
	{
		MLB_CASE(CDecodingManager::MODE_SELECT) UpdateSelect();
		MLB_CASE(CDecodingManager::MODE_CONNECT)UpdateConnect();
		MLB_CASE(CDecodingManager::MODE_SHOW)   UpdateShow();
		MLB_CASEEND;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::Draw()
{
}

//-------------------------------------------------------------------------------------------------------------
// モードの設定
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::SstMode(MODE mode)
{
	m_mode = CDecodingManager::MODE_CONNECT;
	m_modeNext = mode;
	// 接続モードの初期化
	InitConnect();
}

//-------------------------------------------------------------------------------------------------------------
// UIの生成
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::CreateUi(void)
{
	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{
		// UIの生成
		m_pUi[nCntUI] = CDecodingUI::Create(m_Seting[nCntUI]);
		if (nCntUI > UI_DOCUMENT)
		{
			// テクスチャの連結
			m_pUi[nCntUI]->BindTexture(m_pTexture[nCntUI + 5]);
		}
		else {
			// テクスチャの連結
			m_pUi[nCntUI]->BindTexture(m_pTexture[nCntUI]);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 親に従って位置を設定する
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::SetPosAccordingParent(void)
{
	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{
		int nParentID = m_Seting[nCntUI].nParentID;
		if (nParentID != -1)
		{
			m_pUi[nCntUI]->SetPos(m_pUi[nParentID]->GetPos() + m_Seting[nCntUI].pos);
			m_pUi[nCntUI]->SetParent(m_pUi[nParentID].get());
			m_pUi[nCntUI]->UpdateVertex(true);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// ドキュメントのUV情報の初期化
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::InitDocumentUVInfo(void)
{
	CDecodingUI::POLYVERTEXSUVINFO SetingTex;
	SetingTex.pos.u = 0.5f;
	SetingTex.pos.v = 0.75f * 0.5f;
	SetingTex.size.u = 1.0f;
	SetingTex.size.v = 0.75f;
	m_pUi[UI_DOCUMENT]->UpdateVertex(NULL, NULL, NULL, &SetingTex);
}

//-------------------------------------------------------------------------------------------------------------
// 接続モードの初期化
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::InitConnect(void)
{
	switch (m_modeNext)
	{
		MLB_CASE(CDecodingManager::MODE_SELECT) m_nSign = -1;
		MLB_CASE(CDecodingManager::MODE_SHOW)   m_nSign = 1;
		MLB_CASEEND;
	}

	m_nFrame = 0;
	m_fScalValue = (1.0f / (m_nFrameMax + 1) * m_nSign);
	if (m_fScalValue >= 0)
	{
		m_fScal = 0.0f;
	}
	else
	{
		m_fScal = 1.0f;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 選択する時の更新処理
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::UpdateSelect(void)
{
	// 変数宣言
	CMouse *    pMouse = CManager::GetMouse();		// マウスのポインタ
	D3DXVECTOR2 MousePos = pMouse->GetMousePos();	// マウスの位置
	bool        bOverlap = false;					// 重なっているフラグ

	// UI数ループ
	for (int nCntUi = UI_TELLTHEPICTURE; nCntUi < UI_DOCUMENT; nCntUi++)
	{
		// 2Dの衝突判定
		if (m_pUi[nCntUi]->Collision2D(MousePos) == false)
		{// 選択フラグを消す
			m_pUi[nCntUi]->SetSelectFlag(false);
			// 処理を飛ばす
			continue;
		}
		// 選択フラグが消えている時
		if (m_pUi[nCntUi]->GetSelectFlag() == false)
		{// 選択フラグを立てる
			m_pUi[nCntUi]->SetSelectFlag(true);
			// 頂点情報の更新
			m_pUi[UI_BACKLIGHT]->UpdateVertex((D3DXVECTOR3 *)m_pUi[nCntUi]->GetPosPtr());
			// 描画フラグを設定
			m_pUi[UI_BACKLIGHT]->SetDispFlag(true);
			m_nSelectIndex = nCntUi;
		}
		bOverlap = true;
		// 右クリックを離した時かつ選択された番号と同じとき
		if (pMouse->GetRelease(0) &&
			m_nSelectIndex == nCntUi)
		{
			AppeaDocument(nCntUi);
			SstMode(MODE_SHOW);
		}
	}
	// 重なっていなかったとき
	if (bOverlap == false)
	{// 描画フラグを消す
		m_pUi[UI_BACKLIGHT]->SetDispFlag(false);
		// マウスが押されていない時
		if (!pMouse->GetPress(0))
		{
			m_nSelectIndex = -1;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 接続モードの時の更新処理
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::UpdateConnect(void)
{
	// フレームを加算する
	m_nFrame++;

	// フレームが最大以上の時
	if (m_nFrame > m_nFrameMax)
	{
		// フレームを初期化
		m_nFrame = 0;
		// 次のモードに設定
		m_mode = m_modeNext;

		switch (m_mode)
		{
			MLB_CASE(CDecodingManager::MODE_SELECT)
				// ウィンドウの更新
				m_pUi[UI_DOCUMENT]->SetDispFlag(false);
				// そのほかの更新
				for (int nCntUi = UI_CLOSE; nCntUi < UI_MAX; nCntUi++)
				{
					m_pUi[nCntUi]->SetDispFlag(false);
				}
			MLB_CASE(CDecodingManager::MODE_SHOW)   DisplayDocumentsInPlace();
			MLB_CASEEND;
		}
	}

	// 拡大値を加算
	m_fScal += m_fScalValue;
	// ウィンドウの更新
	m_pUi[UI_DOCUMENT]->SetSize(m_Seting[UI_DOCUMENT].size *m_fScal);
	m_pUi[UI_DOCUMENT]->UpdateVertex(true);
	// そのほかの更新
	for (int nCntUi = UI_CLOSE; nCntUi < UI_MAX; nCntUi++)
	{
		m_pUi[nCntUi]->SetPos(m_pUi[nCntUi]->GetParent()->pParent->GetPos() + m_pUi[nCntUi]->GetParent()->vecParent * m_fScal);
		m_pUi[nCntUi]->SetSize(m_Seting[nCntUi].size *m_fScal);
		m_pUi[nCntUi]->UpdateVertex(true);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 解読遺書表示しているときの処理
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::UpdateShow(void)
{
	// 変数宣言
	CMouse *    pMouse = CManager::GetMouse();		// マウスのポインタ
	D3DXVECTOR2 MousePos = pMouse->GetMousePos();	// マウスの位置

	// 閉じるボタンの処理
	CloseButtonProc(pMouse, &MousePos);
	// スクロール処理
	ScrollProc(pMouse, &MousePos);
}

//-------------------------------------------------------------------------------------------------------------
// 閉じるボタンの処理
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::CloseButtonProc(CMouse *pMouse, D3DXVECTOR2 *pMousePos)
{
	// 2Dの衝突判定
	if (m_pUi[UI_CLOSE]->Collision2D(*pMousePos) == false)
	{
		if (m_nSelectIndex != UI_SCROLLHANDLE)
		{
			m_nSelectIndex = -1;
		}
		return;
	}

	// 選択フラグが消えている時
	if (m_pUi[UI_CLOSE]->GetSelectFlag() == false)
	{// 選択フラグを立てる
		m_pUi[UI_CLOSE]->SetSelectFlag(true);
	}
	// 右クリックを離した時かつ選択された番号と同じとき
	if (pMouse->GetRelease(0) &&
		m_nSelectIndex == UI_CLOSE)
	{
		SstMode(MODE_SELECT);
	}
	// マウスを押したとき
	if (pMouse->GetTrigger(0))
	{
		m_nSelectIndex = UI_CLOSE;
	}

}

//-------------------------------------------------------------------------------------------------------------
// スクロール処理
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::ScrollProc(CMouse * pMouse, D3DXVECTOR2 * pMousePos)
{
	// スクロールハンドルをつまむ
	PinchScrollHandle(pMouse, pMousePos);

	// スクロールハンドルを動かす
	MoveScrollHandle(pMouse, pMousePos);

	// マウスホイールスクロール
	MouseWheelScroll(pMousePos);
}

//-------------------------------------------------------------------------------------------------------------
// スクロールハンドルをつまむ
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::PinchScrollHandle(CMouse * pMouse, D3DXVECTOR2 * pMousePos)
{
	// スクロールバーを動かす処理
	if (m_pUi[UI_SCROLLHANDLE]->Collision2D(*pMousePos) == true &&
		pMouse->GetTrigger(0))
	{
		m_VecPinch_center = VEC2(m_pUi[UI_SCROLLHANDLE]->GetPos().x - pMousePos->x, m_pUi[UI_SCROLLHANDLE]->GetPos().y - pMousePos->y);
		m_nSelectIndex = UI_SCROLLHANDLE;
	}
}

//-------------------------------------------------------------------------------------------------------------
// スクロールハンドルを動かす
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::MoveScrollHandle(CMouse * pMouse, D3DXVECTOR2 * pMousePos)
{
	if (!pMouse->GetPress(0))
	{
		m_nSelectIndex = -1;
		return;
	}
	if (m_nSelectIndex == UI_SCROLLHANDLE)
	{
		m_pUi[UI_SCROLLHANDLE]->SetPosY(ScrollClamp(pMousePos, pMousePos->y + m_VecPinch_center.y));
		m_pUi[UI_SCROLLHANDLE]->UpdateVertex(true);
		// ドキュメントをスクロールする
		ScrollThroughDocument();
	}
}

//-------------------------------------------------------------------------------------------------------------
// マウスホイールスクロール
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::MouseWheelScroll(D3DXVECTOR2 * pMousePos)
{
	if (m_nSelectIndex != -1)
	{
		return;
	}

	int nScrollValu = CMouse::GetScrollValue();
	if (nScrollValu == 0)
	{
		return;
	}
	m_pUi[UI_SCROLLHANDLE]->SetPosY(ScrollClamp(pMousePos, m_pUi[UI_SCROLLHANDLE]->GetPos().y -= nScrollValu *30.0f));
	m_pUi[UI_SCROLLHANDLE]->UpdateVertex(true);
	// ドキュメントをスクロールする
	ScrollThroughDocument();
}

//-------------------------------------------------------------------------------------------------------------
// スクロールクランプ
//-------------------------------------------------------------------------------------------------------------
float CDecodingManager::ScrollClamp(D3DXVECTOR2 *pMousePos,float fPosY)
{
	// 変数宣言
	CDecodingUI * pScrollBar = m_pUi[UI_SCROLLBAR].get();
	CDecodingUI * pScrollHandle = m_pUi[UI_SCROLLHANDLE].get();

	// 枠内に収める
	if (pScrollBar->GetPos().y + pScrollBar->GetSize().y *MYLIB_HALF_SIZE <= fPosY + pScrollHandle->GetSize().y*MYLIB_HALF_SIZE)
	{
		fPosY = (pScrollBar->GetPos().y + pScrollBar->GetSize().y*MYLIB_HALF_SIZE - (pScrollHandle->GetSize().y*MYLIB_HALF_SIZE));
		m_VecPinch_center = VEC2(pScrollHandle->GetPos().x - pMousePos->x, fPosY - pMousePos->y);
	}
	if (pScrollBar->GetPos().y - pScrollBar->GetSize().y *MYLIB_HALF_SIZE >= fPosY - pScrollHandle->GetSize().y*MYLIB_HALF_SIZE)
	{
		fPosY = (pScrollBar->GetPos().y - pScrollBar->GetSize().y*MYLIB_HALF_SIZE + (pScrollHandle->GetSize().y*MYLIB_HALF_SIZE));
		m_VecPinch_center = VEC2(pScrollHandle->GetPos().x - pMousePos->x, fPosY - pMousePos->y);
	}
	return fPosY;
}

//-------------------------------------------------------------------------------------------------------------
// ドキュメントをスクロールする
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::ScrollThroughDocument(void)
{
	// ベクトルを計算する
	float fVec = m_pUi[UI_SCROLLHANDLE]->GetPos().y - (m_pUi[UI_SCROLLBAR]->GetPos().y + m_ScrollRange.fMin);
	CDecodingUI::POLYVERTEXSUVINFO *pSetingTex = m_pUi[UI_DOCUMENT]->GetTex();
	float fBasePosY = (0.75f * 0.5f);
	float fScal = 1.0f - (fBasePosY + fBasePosY);
	pSetingTex->pos.v = (fBasePosY)+(fScal *  (fVec / m_fScrollRangeValue));
	m_pUi[UI_DOCUMENT]->UpdateVertex(false, false, true);
}

//-------------------------------------------------------------------------------------------------------------
// ドキュメントを特定の位置に表示する
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::DisplayDocumentsInPlace(void)
{
	m_pUi[UI_DOCUMENT]->SetDispFlag(true);
	m_pUi[UI_DOCUMENT]->SetSize(m_Seting[UI_DOCUMENT].size);
	m_pUi[UI_DOCUMENT]->UpdateVertex(true);

	for (int nCntUi = UI_CLOSE; nCntUi < UI_MAX; nCntUi++)
	{
		m_pUi[nCntUi]->SetDispFlag(true);
		m_pUi[nCntUi]->SetPos(m_pUi[nCntUi]->GetParent()->pParent->GetPos() + m_Seting[nCntUi].pos);
		m_pUi[nCntUi]->SetSize(m_Seting[nCntUi].size);
		m_pUi[nCntUi]->UpdateVertex(true);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ドキュメントの出現
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::AppeaDocument(int nCntUi)
{
	m_pUi[UI_DOCUMENT]->BindTexture(m_pTexture[nCntUi + 6]);
	m_pUi[UI_DOCUMENT]->SetDispFlag(true);
	m_pUi[UI_DOCUMENT]->SetSize(D3DXVECTOR2(0.0f, 0.0f));
	m_pUi[UI_DOCUMENT]->UpdateVertex(true);

	for (int nCntUi = UI_CLOSE; nCntUi < UI_MAX; nCntUi++)
	{
		m_pUi[nCntUi]->SetDispFlag(true);
		m_pUi[nCntUi]->SetPos(m_pUi[nCntUi]->GetParent()->pParent->GetPos());
		m_pUi[nCntUi]->SetSize(D3DXVECTOR2(0.0f, 0.0f));
		m_pUi[nCntUi]->UpdateVertex(true);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ドキュメントの消滅
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::DisAppeaDocument(void)
{
	m_pUi[UI_DOCUMENT]->SetDispFlag(false);
	for (int nCntUi = UI_CLOSE; nCntUi < UI_MAX; nCntUi++)
	{
		m_pUi[nCntUi]->SetDispFlag(false);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 1行から情報を読み取る
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData)
{
	// 変数宣言
	D3DXVECTOR3 pos   = MYLIB_3DVECTOR_ZERO;	// 位置
	D3DXVECTOR2 size  = MYLIB_2DVECTOR_ZERO;	// 大きさ
	D3DXCOLOR   col   = MYLIB_D3DXCOR_SET;		// 色
	float       fData = MYLIB_FLOAT_UNSET;		// float型のデータ
	int         nData = MYLIB_INT_UNSET;		// int型のデータ
	char        sData[MYLIB_STRINGSIZE] = { 0 };
	SETINGINFO* pSet  = nullptr;				// 設定情報のポインタ
	if (strcmp(cnpEntryType, "SetUp") != 0)
	{
		pSet = &m_Seting[atoi(m_pHash->Search((char*)cnpEntryType))];
		if (sscanf(cnpLine, "pos = %f %f", &pos.x, &pos.y) == 2)
		{
			pSet->pos = pos;
		}
		else if (sscanf(cnpLine, "size = %f %f", &size.x, &size.y) == 2)
		{
			pSet->size = size;
		}
		else if (sscanf(cnpLine, "col = %f %f %f %f", &col.r, &col.g, &col.b, &col.a) == 4)
		{
			pSet->col = col;
		}
		else if (sscanf(cnpLine, "angle = %f", &fData) == 1)
		{
			pSet->fAngle = fData;
		}
		else if (sscanf(cnpLine, "originType = %d", &nData) == 1)
		{
			pSet->OriginType = (ORIGINVERTEXTYPE)nData;
		}
		else if (sscanf(cnpLine, "disp = %d", &nData) == 1)
		{
			pSet->bDisp = (nData == 1);
		}
		else if (sscanf(cnpLine, "Parent = %s", &sData[0]) == 1)
		{
			pSet->nParentID = atoi(m_pHash->Search(sData));
		}
	}
	else
	{
		if (sscanf(cnpLine, "ModeCount = %d", &nData) == 1)
		{
			m_nFrameMax = nData;
		}
	}
}
