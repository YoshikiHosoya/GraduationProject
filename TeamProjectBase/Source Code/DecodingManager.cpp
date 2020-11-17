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

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define DECODINGMANA_FILENAME "data/TEXT/DecodingUI_Info.txt"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバの初期化
//-------------------------------------------------------------------------------------------------------------
CHash *    CDecodingManager::m_pHash = nullptr;										// ハッシュポインタ
LPDIRECT3DTEXTURE9 CDecodingManager::m_pTexture[UI_MAX] = Mybfunc_array(nullptr);	// テクスチャ情報
SETINGINFO CDecodingManager::m_Seting[UI_MAX];										// 設定情報

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
	};
	// ハッシュの作成
	m_pHash = CHash::Create();

	// ハッシュに挿入する
	for (int nCntHash = 0; nCntHash < UI_MAX; nCntHash++)
	{
		m_pHash->Insert(Key[nCntHash], Data[nCntHash]);
	}

}

//-------------------------------------------------------------------------------------------------------------
// ハッシュの開放
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::ReleaseHash(void)
{
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
	m_pTexture[UI_TELLTHEPICTURE]    = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T00);
	m_pTexture[UI_SHAPEMEMORIZATION] = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T01);
	m_pTexture[UI_CORDCUTTING]       = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T02);
	m_pTexture[UI_MAZE]              = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T03);
	m_pTexture[UI_SIMON]             = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T04);
	m_pTexture[UI_NOWMAKING]         = CTexture::GetTexture(CTexture::TEX_UI_DECODING_T05);
	m_pTexture[UI_BACKLIGHT]         = CTexture::GetTexture(CTexture::TEX_UI_DECODING_BACKLIGHT);

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
	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
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
	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{
		// UIの生成
		m_pUi[nCntUI] = CDecodingUI::Create(m_Seting[nCntUI]);
		// テクスチャの連結
		m_pUi[nCntUI]->BindTexture(m_pTexture[nCntUI]);
	}

	m_mode = MODE_SELECT;

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
		MLB_CASE(CDecodingManager::MODE_SELECT)UpdateSelect();
		MLB_CASE(CDecodingManager::MODE_SHOW)  UpdateShow();
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
// 選択する時の更新処理
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::UpdateSelect(void)
{
	// 変数宣言
	CMouse *    pMouse = CManager::GetMouse();	// マウスのポインタ
	D3DXVECTOR2 MousePos = pMouse->GetMousePos();	// マウスの位置

	for (int nCntUi = 0; nCntUi < UI_MAX; nCntUi++)
	{

	}
}

//-------------------------------------------------------------------------------------------------------------
// 解読遺書表示しているときの処理
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::UpdateShow(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 1行から情報を読み取る
//-------------------------------------------------------------------------------------------------------------
void CDecodingManager::ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData)
{
	// 変数宣言
	D3DXVECTOR3 pos   = MYLIB_3DVECTOR_ZERO;										// 位置
	D3DXVECTOR2 size  = MYLIB_2DVECTOR_ZERO;										// 大きさ
	D3DXCOLOR   col   = MYLIB_D3DXCOR_SET;											// 色
	float       fData = MYLIB_FLOAT_UNSET;											// float型のデータ
	int         nData = MYLIB_INT_UNSET;											// int型のデータ
	SETINGINFO* pSet  = &m_Seting[atoi(m_pHash->Search((char*)cnpEntryType))];		// 設定情報のポインタ

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
}
