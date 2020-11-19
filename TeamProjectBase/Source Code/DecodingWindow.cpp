//*************************************************************************************************************
//
// 解読書のマネージャー[処理 [DecodingManager.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "DecodingWindow.h"
#include "DecodingUI.h"
#include "myhash.h"
#include "texture.h"
#include "mouse.h"
#include "manager.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define DECODINGWINDOW_FILENAME "data/TEXT/InitWindowInfo.txt"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CDecodingWindow::m_pTexture[TEX_MAX] = Mybfunc_array(nullptr);
CHash *            CDecodingWindow::m_pHash	            = nullptr;
SETINGINFO         CDecodingWindow::m_Seting[TYPE_MAX];

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CDecodingWindow::CDecodingWindow()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CDecodingWindow::~CDecodingWindow()
{
}

//-------------------------------------------------------------------------------------------------------------
// ハッシュの作成
//-------------------------------------------------------------------------------------------------------------
void CDecodingWindow::MakeHash(void)
{
	// キーデータ
	char * Key[TYPE_MAX] =
	{
		{ "Window" },
		{ "Close" },
		{ "ScrollBar" },
		{ "ScrollHandle" },
	};
	// データ
	char * Data[TYPE_MAX] =
	{
		{ "0" },
		{ "1" },
		{ "2" },
		{ "3" },
	};
	// ハッシュの作成
	m_pHash = CHash::Create();

	// ハッシュに挿入する
	for (int nCntHash = 0; nCntHash < TYPE_MAX; nCntHash++)
	{
		m_pHash->Insert(Key[nCntHash], Data[nCntHash]);
	}

}

//-------------------------------------------------------------------------------------------------------------
// ハッシュの開放
//-------------------------------------------------------------------------------------------------------------
void CDecodingWindow::ReleaseHash(void)
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
HRESULT CDecodingWindow::Load(void)
{
	// テクスチャ情報を取得する
	m_pTexture[TEX_TELLTHEPICTURE]    = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D00);
	m_pTexture[TEX_SHAPEMEMORIZATION] = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D01);
	m_pTexture[TEX_CORDCUTTING]       = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D02);
	m_pTexture[TEX_MAZE]              = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D03);
	m_pTexture[TEX_SIMON]             = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D04);
	m_pTexture[TEX_NOWMAKING]         = CTexture::GetTexture(CTexture::TEX_UI_DECODING_D05);

	// 設定情報の初期化
	for (int nCntSet = 0; nCntSet < TYPE_MAX; nCntSet++)
	{
		m_Seting[nCntSet] = SETINGINFO();
	}

	// ファイル読み込み
	if (CLoadFile::ReadLineByLineFromFile(DECODINGWINDOW_FILENAME, ReadFromLine) != CLoadFile::LR_SUCCESS)
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
void CDecodingWindow::UnLoad(void)
{
	for (int nCntUI = 0; nCntUI < TEX_MAX; nCntUI++)
	{
		m_pTexture[nCntUI] = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CDecodingWindow* CDecodingWindow::Create(void)
{
	// スマートポインタの生成
	CDecodingWindow* pWindow = new CDecodingWindow;
	// 初期化
	pWindow->Init();
	return pWindow;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CDecodingWindow::Init()
{
	// UIの作成
	MakeUI();

	// スクロールハンドルの大きさの設定を変更
	m_pUi[TYPE_SCROLLHANDLE]->SetSize(D3DXVECTOR2(m_pUi[TYPE_SCROLLHANDLE]->GetSize().x, m_Seting[TYPE_SCROLLBAR].size.y * 0.75f));
	m_Seting[TYPE_SCROLLHANDLE].size.y = m_Seting[TYPE_SCROLLBAR].size.y * 0.75f;

	// 可動域の計算
	//m_ScrollRange.fMax = m_Seting[TYPE_SCROLLBAR].size.y*MYLIB_HALF_SIZE - (m_Seting[TYPE_SCROLLHANDLE].size.y*MYLIB_HALF_SIZE);
	//m_ScrollRange.fMin = -m_Seting[TYPE_SCROLLBAR].size.y*MYLIB_HALF_SIZE + (m_Seting[TYPE_SCROLLHANDLE].size.y*MYLIB_HALF_SIZE);
	//m_fScrollRangeValue = m_ScrollRange.GetFloatValue();

	// スクロールハンドルの位置の設定を変更
	//m_Seting[TYPE_SCROLLHANDLE].pos.y = m_Seting[TYPE_SCROLLBAR].pos.y + m_ScrollRange.fMin - m_Seting[TYPE_SCROLLBAR].pos.y;

	// 親の設定
	SetPosAccordingParent();
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CDecodingWindow::Update()
{
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CDecodingWindow::Draw()
{
}

//-------------------------------------------------------------------------------------------------------------
// UIの作成
//-------------------------------------------------------------------------------------------------------------
void CDecodingWindow::MakeUI(void)
{
	// ループ
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// UIの生成
		m_pUi[nCntType] = CDecodingUI::Create(m_Seting[nCntType]);
		m_pUi[nCntType]->BindTexture(m_pTexture[nCntType]);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 親に従って位置を設定する
//-------------------------------------------------------------------------------------------------------------
void CDecodingWindow::SetPosAccordingParent(void)
{
	// 変数宣言
	SETINGINFO* pSet = &m_Seting[0];				// 設定情報のポインタ
	// ループ
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		// 親IDの取得
		int nParentID = pSet[nCntType].nParentID;
		if (nParentID != -1)
		{	// 位置を設定する
			m_pUi[nCntType]->SetPos(m_pUi[nParentID]->GetPos() + pSet[nCntType].pos);
			// 親情報の設定
			m_pUi[nCntType]->SetParent(m_pUi[nParentID].get());
			// 頂点情報の更新
			m_pUi[nCntType]->UpdateVertex(true);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 1行から情報を読み取る
//-------------------------------------------------------------------------------------------------------------
void CDecodingWindow::ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData)
{
	// 変数宣言
	D3DXVECTOR3 pos = MYLIB_3DVECTOR_ZERO;	// 位置
	D3DXVECTOR2 size = MYLIB_2DVECTOR_ZERO;	// 大きさ
	D3DXCOLOR   col = MYLIB_D3DXCOR_SET;		// 色
	float       fData = MYLIB_FLOAT_UNSET;		// float型のデータ
	int         nData = MYLIB_INT_UNSET;		// int型のデータ
	char        sData[MYLIB_STRINGSIZE] = { 0 };
	SETINGINFO* pSet = nullptr;				// 設定情報のポインタ

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
