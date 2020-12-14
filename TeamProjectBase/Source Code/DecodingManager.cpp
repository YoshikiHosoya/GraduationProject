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
#define DECODINGMANA_UIINFO_FILENAME   "data/TEXT/DecodingUI_Info.txt"
#define DECODINGMANA_WINDINFO_FILENAME "data/TEXT/DecodingWind_Info.txt"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバの初期化
//-------------------------------------------------------------------------------------------------------------
CHash *            CDecodingManager::m_pHash = nullptr;								// ハッシュポインタ
LPDIRECT3DTEXTURE9 CDecodingManager::m_pTexture[TEX_MAX] = Mlf_array(nullptr);	// テクスチャ情報
SETINGINFO         CDecodingManager::m_Seting[UI_MAX];								// 設定情報
WINDOW_SETING      CDecodingManager::m_WindSeting[WIND_MAX];						// ウィンドウの設定情報

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
	char * Key[UI_MAX + WIND_MAX] =
	{
		{ "Backlight" },
		{ "TellThePicture" },
		{ "ShapeMemorization" },
		{ "CordCutting" },
		{ "Maze" },
		{ "Simon" },
		{ "TellThePicture_Wind" },
		{ "ShapeMemorization_Wind" },
		{ "CordCutting_Wind" },
		{ "Maze_Wind" },
		{ "Simon_Wind" },
	};
	// データ
	char * Data[UI_MAX + WIND_MAX] =
	{
		{ "0" },
		{ "1" },
		{ "2" },
		{ "3" },
		{ "4" },
		{ "5" },
		{ "0" },
		{ "1" },
		{ "2" },
		{ "3" },
		{ "4" },
	};
	// ハッシュの作成
	m_pHash = CHash::Create();

	// ハッシュに挿入する
	for (int nCntHash = 0; nCntHash < UI_MAX + WIND_MAX; nCntHash++)
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
	// テクスチャ情報を取得する
	m_pTexture[TEX_TELLTHEPICTURE]      = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T00);
	m_pTexture[TEX_SHAPEMEMORIZATION]   = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T01);
	m_pTexture[TEX_CORDCUTTING]         = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T02);
	m_pTexture[TEX_MAZE]                = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T03);
	m_pTexture[TEX_SIMON]               = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T04);
	m_pTexture[TEX_BACKLIGHT]           = CTexture::GetTexture(CTexture::TEX_UI_DECODING_BACKLIGHT);

	// 設定情報の初期化
	for (int nCntSet = 0; nCntSet < UI_MAX ; nCntSet++)
	{
		m_Seting[nCntSet] = SETINGINFO();
	}

	// ファイル読み込み
	if (CLoadFile::ReadLineByLineFromFile(DECODINGMANA_UIINFO_FILENAME, UiInfoReadFromLine) != CLoadFile::LR_SUCCESS)
	{
#ifdef _DEBUG
		std::cout << "<<<<<<CDecodingManagerのファイル読み込みが失敗しました。>>>>>>\n";
#endif // _DEBUG
		return E_FAIL;
	}
	// ファイル読み込み
	if (CLoadFile::ReadLineByLineFromFile(DECODINGMANA_WINDINFO_FILENAME, WindInfoReadFromLine) != CLoadFile::LR_SUCCESS)
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
	// ウィンドウの生成
	CreateWind();

	// 親をもとに位置を設定する
	SetPosAccordingParent();

	// 選択している番号の初期化
	m_nSelectIndex = -1;

	// モードの初期化
	m_mode = MODE_SELECT;
	m_modeNext = m_mode;

	// アクティブなウィンドウのポインタを初期化
	m_pActiveWind = nullptr;

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
		ML_CASE(CDecodingManager::MODE_SELECT)  UpdateSelect();		// 選択モード
		ML_CASE(CDecodingManager::MODE_CONNECT) UpdateConnect();	// 接続モード
		ML_CASE(CDecodingManager::MODE_SHOW)    UpdateShow();		// 表示モード
		ML_CASEEND;
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
	// 最大から接続モードの範囲内の時
	if (Mlf_InRange(mode, MODE_MAX, MODE_CONNECT))
	{
		// 接続モードに設定
		m_mode = CDecodingManager::MODE_CONNECT;
		// 引数のモードに設定
		m_modeNext = mode;
		// 接続モードの初期化
		InitConnect();
	}
}

//-------------------------------------------------------------------------------------------------------------
// UIの生成
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::CreateUi(void)
{
	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{// UIの生成
		m_pUi[nCntUI] = CDecodingUI::Create(m_Seting[nCntUI]);
		// テクスチャの連結
		m_pUi[nCntUI]->BindTexture(m_pTexture[nCntUI]);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ウィンドウの生成
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::CreateWind(void)
{
	// ウィンドウのスケールの最大の設定
	CDecodingWindow::SetScalMax(1.0f);

	for (int nCntWind = 0; nCntWind < WIND_MAX; nCntWind++)
	{// ウィンドウの生成
		m_pWind[nCntWind] = CDecodingWindow::Create(m_WindSeting[nCntWind]);
		// ウィンドウテクスチャの設定
		m_pWind[nCntWind]->SetWindowTexture(nCntWind);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 親に従って位置を設定する
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::SetPosAccordingParent(void)
{
	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{// 親IDの取得
		int nParentID = m_Seting[nCntUI].nParentID;
		// 親IDが設定されていた時
		if (nParentID != -1)
		{
			// 親の設定
			m_pUi[nCntUI]->SetParent(m_pUi[nParentID].get());
			// 位置の設定
			m_pUi[nCntUI]->SetPos(m_pUi[nParentID]->GetPos() + m_pUi[nCntUI]->GetParent()->vecParent);
			// 頂点位置の更新
			m_pUi[nCntUI]->UpdateVertex(true);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 接続モードの初期化
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::InitConnect(void)
{
	// 符号の設定
	switch (m_modeNext)
	{
		ML_CASE(CDecodingManager::MODE_SELECT) m_pActiveWind->SetSign(-1);
		ML_CASE(CDecodingManager::MODE_SHOW)   m_pActiveWind->SetSign(1);
		ML_CASEEND;
	}

	// 出現情報の初期化
	m_pActiveWind->InitAppearInfo();
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
	for (int nCntUi = UI_TELLTHEPICTURE; nCntUi < UI_MAX; nCntUi++)
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
			m_pActiveWind = m_pWind[nCntUi - 1].get();
			m_pActiveWind->PreparingAppear();
			SstMode(MODE_SHOW);
		}
	}
	// 重なっていなかったとき
	if (bOverlap == false)
	{// 描画フラグを消す
		m_pUi[UI_BACKLIGHT]->SetDispFlag(false);
		m_nSelectIndex = -1;
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
	switch (m_modeNext)
	{
		ML_CASE(CDecodingManager::MODE_SELECT)
		{
			if (m_pActiveWind->Disappearance() == true)
			{
				// 次のモードに設定
				m_mode = m_modeNext;
				// アクティブなウィンドウのポインタを初期化
				m_pActiveWind = nullptr;
			}
		}
		ML_CASE(CDecodingManager::MODE_SHOW)
		{
			if (m_pActiveWind->Appearance() == true)
			{
				// 次のモードに設定
				m_mode = m_modeNext;
			}
		}
		ML_CASEEND;
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
	if (m_pActiveWind->CloseButtonProc(pMouse, &MousePos) == true)
	{
		SstMode(MODE_SELECT);
	}
	else
	{
		// スクロール処理
		m_pActiveWind->ScrollProc(pMouse, &MousePos);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 1行から情報を読み取る
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::UiInfoReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData)
{
	// 変数宣言
	D3DXVECTOR3 pos                     = MYLIB_3DVECTOR_ZERO;				// 位置
	D3DXVECTOR2 size                    = MYLIB_2DVECTOR_ZERO;				// 大きさ
	D3DXCOLOR   col                     = MYLIB_D3DXCOR_SET;				// 色
	float       fData                   = MYLIB_FLOAT_UNSET;				// float型のデータ
	int         nData                   = MYLIB_INT_UNSET;					// int型のデータ
	char        sData[MYLIB_STRINGSIZE] = Mlf_array(MYLIB_CHAR_UNSET);	// 文字情報のデータ
	SETINGINFO* pSet                    = nullptr;							// 設定情報のポインタ

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

//-------------------------------------------------------------------------------------------------------------
// 1行から情報を読み取る
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::WindInfoReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData)
{
	// 変数宣言
	D3DXVECTOR3    pos                     = MYLIB_3DVECTOR_ZERO;								// 位置
	D3DXVECTOR2    dataF2                  = MYLIB_2DVECTOR_ZERO;								// float型のデータ２個
	D3DXCOLOR      col                     = MYLIB_D3DXCOR_SET;									// 色
	float          fData                   = MYLIB_FLOAT_UNSET;									// float型のデータ
	int            nData                   = MYLIB_INT_UNSET;									// int型のデータ
	char           sData[MYLIB_STRINGSIZE] = Mlf_array(MYLIB_CHAR_UNSET);					// 文字情報のデータ
	WINDOW_SETING* pSet                    = nullptr;											// 設定情報のポインタ

	pSet = &m_WindSeting[atoi(m_pHash->Search((char*)cnpEntryType))];
	if (sscanf(cnpLine, "pos = %f %f", &pos.x, &pos.y) == 2)
	{
		pSet->pos = pos;
	}
	else if (sscanf(cnpLine, "size = %f %f", &dataF2.x, &dataF2.y) == 2)
	{
		pSet->size = dataF2;
	}
	else if (sscanf(cnpLine, "col = %f %f %f %f", &col.r, &col.g, &col.b, &col.a) == 4)
	{
		pSet->col = col;
	}
	else if (sscanf(cnpLine, "AspectRatio = %f %f", &dataF2.x, &dataF2.y) == 2)
	{
		pSet->AspectRatio = dataF2;
	}
	else if (sscanf(cnpLine, "SizeScal = %f %f", &dataF2.x, &dataF2.y) == 2)
	{
		pSet->SizeScal = dataF2;
	}
}
