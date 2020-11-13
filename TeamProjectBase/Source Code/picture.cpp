//*************************************************************************************************************
//
// 絵の処理 [picture.cpp]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "picture.h"
#include "manager.h"
#include "renderer.h"
#include "mouse.h"
#include "camera.h"
#include "PaintingPen.h"
#include "game.h"
#include "tablet.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define PICTURE_FILENAME		"data/TEXT/PictureInfo.txt"
#define PICTURE_WRITINGPASS		"data/SAVEDATA/PictureTextures/"
#define PICTURE_WRITINGNAME		"PicTex.txt"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
INTEGER2      CPicture::m_nNumPixelBlock  = MYLIB_INT2_UNSET;						// ピクセル数
FLOAT2        CPicture::m_size            = MYLIB_2DVECTOR_ZERO;					// 大きさ
D3DXVECTOR3   CPicture::m_PlaneNor        = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 平面の法線
D3DXVECTOR2   CPicture::m_PixelSize       = MYLIB_VEC2_UNSET;						// ピクセルサイズ
UINT          CPicture::m_nNumDataMax     = MYLIB_INT_UNSET;						// 最大データ数最大データ数
CPaintingPen* CPicture::m_pPen            = nullptr;								// ペンのポインタ
D3DXVECTOR2   CPicture::m_PixelSizehalf   = MYLIB_VEC2_UNSET;						// ピクセルサイズの半分
D3DXVECTOR2*  CPicture::m_pPixelPos       = nullptr;								// ピクセル位置のポインタ
UINT          CPicture::m_nNumMakeFile    = MYLIB_INT_UNSET;						// ファイルを作った回数
CString       CPicture::m_WriteToFile;												// 書き込み先のァイル名
D3DXVECTOR3   CPicture::m_SetingPos       = MYLIB_VEC3_UNSET;						// 設定用の位置

//-------------------------------------------------------------------------------------------------------------
// 静的メンバの初期化
//-------------------------------------------------------------------------------------------------------------
void CPicture::InitStaticMember(void)
{
	m_PixelSize = m_size / m_nNumPixelBlock;
	m_PixelSizehalf = m_PixelSize * MYLIB_HALF_SIZE;
	m_nNumDataMax = m_nNumPixelBlock.nY * m_nNumPixelBlock.nX;
	m_nNumMakeFile = MYLIB_INT_UNSET;
	// ピクセル位置の開放
	ReleasePixelPos();
	// ピクセル位置の生成
	CreatePixelPos();
	// ペンの開放
	ReleasePen();
	// ペンの生成
	m_pPen = CPaintingPen::Create();
}

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の終了
//-------------------------------------------------------------------------------------------------------------
void CPicture::UninitStaticMember(void)
{
	// ペンの開放
	ReleasePen();
	// ピクセル位置の開放
	ReleasePixelPos();
}

//-------------------------------------------------------------------------------------------------------------
// エラーの検出
//-------------------------------------------------------------------------------------------------------------
HRESULT CPicture::LoadError(void)
{
	// どちらもゼロの時
	if (m_nNumPixelBlock.OneIsZero() == true)
	{
#ifdef _DEBUG
		std::cout << "<<<<<<CPictureのどちらかのポリゴン数が0でした。>>>>>>\n";
		return E_FAIL;
#endif // _DEBUG
	}
	// ファイルサイズが設定されていない時
	if (m_WriteToFile.size() == MYLIB_INT_UNSET)
	{
#ifdef _DEBUG
		std::cout << "<<<<<<CPictureの書き込み先ファイルサイズが0でした。>>>>>>\n";
		return E_FAIL;
#endif // _DEBUG
	}
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// マトリックスの計算
//-------------------------------------------------------------------------------------------------------------
void CPicture::MatrixCal(void)
{
	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// マトリックスの初期化
	m_trans.Identity();

	// スケールの反映
	m_trans.mtxWorld.m[0][0] = m_trans.scal.x;
	m_trans.mtxWorld.m[1][1] = m_trans.scal.y;
	m_trans.mtxWorld.m[2][2] = m_trans.scal.z;

	// 回転マトリックスを作成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_trans.rot.y, m_trans.rot.x, m_trans.rot.z);

	// 平行移動マトリックスを作成
	D3DXMatrixTranslation(&mtxTrans, m_trans.pos.x, m_trans.pos.y, m_trans.pos.z);

	// マトリックス情報を合成
	m_trans.mtxWorld *= mtxRot * mtxTrans * *m_pMtxParent;
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
inline CPicture::~CPicture()
{
	this->m_pMtxParent = nullptr;
	// 頂点バッファの取得
	if (m_pVtexBuff != nullptr)
	{
		m_pVtexBuff->Release();
		m_pVtexBuff = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CPicture::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 初期化
	MatrixCal();
	m_Flags.data = MASK_DISP;
	try
	{	// テクスチャの作成
		MakeTexture(pDevice);
		// 頂点情報の作成
		MakeVertex(pDevice);
	}
	catch (HRESULT rh)
	{// 例外を返す
		std::cout << "例外が発生しました。\n";
		return rh;
	}

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CPicture::Update()
{
	// ペンの更新
	m_pPen->Update();
	// 絵を描く
	PaintProc();
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CPicture::Draw()
{
	if (Mybfunc_bit_comp(m_Flags.data, CPicture::FLAG_DISP))
	{
		//デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// マトリックスの計算
		MatrixCal();

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_trans.mtxWorld);

		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, m_pVtexBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,										//開始するインデックス(頂点)
			2);										//ポリゴンの枚数
	}
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
std::shared_ptr<CPicture> CPicture::Create(D3DMATRIX *pMtxParent)
{
	// スマートポインタの生成
	std::shared_ptr<CPicture> pPicture = std::make_shared<CPicture>();
	// 親マトリックスの設定
	pPicture->SetParent(pMtxParent);
	// 位置の設定
	pPicture->SetPos(m_SetingPos);
	// 初期化
	pPicture->Init();

	//Sceneで管理
	pPicture->SetObjType(OBJTYPE_PICTURE);
	pPicture->AddSharedList(pPicture);
	return pPicture;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャの作成
//-------------------------------------------------------------------------------------------------------------
HRESULT CPicture::MakeTexture(LPDIRECT3DDEVICE9 pDevice, CONST_STRING TextureFile, LPDIRECT3DTEXTURE9 *ppTexture)
{
	if (FAILED(pDevice->CreateTexture(m_nNumPixelBlock.nX, m_nNumPixelBlock.nY, 1, 0, D3DFMT_A32B32G32R32F,
		D3DPOOL_MANAGED, ppTexture, NULL)))
	{
		throw E_FAIL;
	}

	Reading(*ppTexture, m_WriteToFile);
	return E_NOTIMPL;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャの作成
//-------------------------------------------------------------------------------------------------------------
void CPicture::MakeTexture(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateTexture(m_nNumPixelBlock.nX, m_nNumPixelBlock.nY, 1, 0, D3DFMT_A32B32G32R32F,
		D3DPOOL_MANAGED, &m_pTexture, NULL)))
	{
		throw E_FAIL;
	}
	// テクスチャカラーの初期化
	InitTextureColor();
	// 読み込み
	//Reading(m_pTexture, m_WriteToFile);
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の作成
//-------------------------------------------------------------------------------------------------------------
void CPicture::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtexBuff,
		NULL)))
	{
		throw E_FAIL;
	}

	// 変数宣言
	VERTEX_3D *pVtx;								// 頂点情報の設定
	FLOAT2    sizeUpdate = MYLIB_2DVECTOR_ZERO;		// 更新する大きさ

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtexBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 縦の四角形の個数分
	for (int nCntVertical = 0; nCntVertical < 2; nCntVertical++)
	{
		// 横の四角形の個数分
		for (int nCntHeng = 0; nCntHeng < 2; nCntHeng++)
		{
			// 位置の設定
			pVtx[0].pos = D3DXVECTOR3(sizeUpdate.x, sizeUpdate.y, MYLIB_INT_UNSET);
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 色の設定
			pVtx[0].col = MYLIB_D3DXCOR_SET;
			// UV情報の設定
			pVtx[0].tex = D3DXVECTOR2(1.0f*nCntHeng, 1.0f*nCntVertical);
			// 頂点ポインタのインクリメント
			pVtx++;
			// 更新する大きさX軸を加算
			sizeUpdate.x += m_size.x;
		}
		// 更新する大きさX軸の初期化
		sizeUpdate.x = MYLIB_FLOAT_UNSET;
		// 更新する大きさY軸を加算
		sizeUpdate.y -= m_size.y;
	}
	// 頂点データをアンロック
	m_pVtexBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャカラーの初期化
//-------------------------------------------------------------------------------------------------------------
void CPicture::InitTextureColor(void)
{
	// 変数宣言
	D3DXCOLOR *    pData;			// カラーデータポインタ
	D3DLOCKED_RECT LockRect;		// ロックしたテクスチャ情報

	// テクスチャのロック
	m_pTexture->LockRect(0, &LockRect, NULL, 0);

	// データの先頭を代入
	pData = (D3DXCOLOR*)LockRect.pBits;
	// データの設定
	for (UINT nCntPixel = 0; nCntPixel < m_nNumDataMax; nCntPixel++)
	{
		*pData = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pData++;
	}
	// テクスチャのアンロック
	m_pTexture->UnlockRect(0);

}

//-------------------------------------------------------------------------------------------------------------
// 1行ずつ読みこむ
//-------------------------------------------------------------------------------------------------------------
void CPicture::ReadFromLine(CONST_STRING Line)
{
	INTEGER2    NumPoly = MYLIB_INT2_UNSET;
	D3DXVECTOR2 size    = MYLIB_2DVECTOR_ZERO;
	char aFileName[MYLIB_STRINGSIZE] = { '\0' };
	// ポリゴン数
	if (sscanf(Line, "NumPolygon = %d %d", &NumPoly.nX , &NumPoly.nY) == 2)
	{
		m_nNumPixelBlock = NumPoly;
	}
	// ポリゴンサイズ
	else if (sscanf(Line, "Size = %f %f",&size.x, &size.y) == 2)
	{
		m_size = size;
	}
	// 書き込み先のファイル
	else if (sscanf(Line, "WriteToFile = %s", aFileName) == 1)
	{
		m_WriteToFile = aFileName;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ピクセル位置の生成
//-------------------------------------------------------------------------------------------------------------
void CPicture::CreatePixelPos(void)
{
	// 最大データ数が0以下の時
	if (m_nNumDataMax <= MYLIB_INT_UNSET)
	{// nullを設定
		m_pPixelPos = nullptr;
	}

	// ピクセル位置の生成
	m_pPixelPos = new D3DXVECTOR2[m_nNumDataMax];

	// ポインタ渡し
	D3DXVECTOR2* pPixelPos = m_pPixelPos;

	// 位置の設定
	for (int nY = 0; nY < m_nNumPixelBlock.nY; nY++) {
		for (int nX = 0; nX < m_nNumPixelBlock.nX; nX++) {
			pPixelPos->x = nX * m_PixelSize.x + m_PixelSizehalf.x;
			pPixelPos->y = nY * m_PixelSize.y + m_PixelSizehalf.y;
			pPixelPos++;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// ピクセル位置の開放
//-------------------------------------------------------------------------------------------------------------
void CPicture::ReleasePixelPos(void)
{
	if (m_pPixelPos)
	{
		delete m_pPixelPos;
		m_pPixelPos = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ペンの開放
//-------------------------------------------------------------------------------------------------------------
void CPicture::ReleasePen(void)
{
	if (m_pPen)
	{
		m_pPen->Uninit();
		delete m_pPen;
		m_pPen = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 塗る処理
//-------------------------------------------------------------------------------------------------------------
void CPicture::PaintProc(void)
{
	// 絵上の位置の取得
	if (GetMousePosOnPicture() == false)
	{// 取得失敗
		return;
	}

	// タブレットのボタンを押しているか
	if (CManager::GetGame()->GetTablet()->ItIsPressingButtons() == true)
	{// 押している
		return;
	}

	// カプセルの設定
	m_pPen->SetCapsule();

	// ロックした情報
	D3DLOCKED_RECT LockRect;
	// テクスチャリソースの長方形をロックし、ロックした情報を取得
	m_pTexture->LockRect(0, &LockRect, NULL, 0);
	
	D3DXCOLOR *pData = (D3DXCOLOR*)LockRect.pBits;				// カラーデータの先頭を取得
	CAPSULE_2D *pCap = m_pPen->GetCapsule();					// カプセル情報の取得
	D3DXVECTOR2 *pPixelPos = m_pPixelPos;						// ピクセル位置の取得
	const float fRadius = pCap->fRadius * pCap->fRadius;		// 比較用の半径を算出
	const float fSegLeng = pCap->Segment.vec.Length();			// カプセルの線分の長さを取得
	int nPenMode = m_pPen->GetMode();							// モードの取得

	// 線分の長さが許容以下の時
	if (fSegLeng <= MYLIB_OX_EPSILON)
	{
		for (UINT nCntPixel = 0; nCntPixel < m_nNumDataMax; nCntPixel++, pPixelPos++, pData++)
		{
			// (白の時 == 1、黒の時 == 0) == モード(ブラシ == 0、消しゴム == 1)の時
			if ((pData->r > 0.5f) == nPenMode)
			{// 処理をスキップする
				continue;
			}
			// 位置の差分を算出
			D3DXVECTOR2 DiffPos = pCap->Segment.pos - *pPixelPos;
			// 距離を比較
			if (DiffPos.x * DiffPos.x + DiffPos.y * DiffPos.y <= fRadius)
			{// ペンで色を塗る
				m_pPen->PaintCol(pData);
			}
		}
	}
	else
	{
		// 線分の終端を取得
		CONST FLOAT2 SegEndPoint = pCap->Segment.GetEndPoint();

		for (UINT nCntPixel = 0; nCntPixel < m_nNumDataMax; nCntPixel++, pPixelPos++, pData++)
		{
			// (白の時 == 1、黒の時 == 0) == モード(ブラシ == 0、消しゴム == 1)の時
			if ((pData->r > 0.5f) == nPenMode)
			{// 処理をスキップする
				continue;
			}
			// 鋭角じゃない時
			if (CMylibrary::IsSharpAngle(*pPixelPos, pCap->Segment.pos, SegEndPoint) == false)
			{// 位置の差分を算出
				D3DXVECTOR2 DiffPos = pCap->Segment.pos - *pPixelPos;
				// 距離を比較
				if (DiffPos.x * DiffPos.x + DiffPos.y * DiffPos.y <= fRadius)
				{// ペンで色を塗る
					m_pPen->PaintCol(pData);
				}
			}
			// 鋭角じゃない時
			else if (CMylibrary::IsSharpAngle(*pPixelPos, SegEndPoint, pCap->Segment.pos) == false)
			{// 位置の差分を算出
				D3DXVECTOR2 DiffPos = SegEndPoint - *pPixelPos;
				// 距離を比較
				if (DiffPos.x * DiffPos.x + DiffPos.y * DiffPos.y <= fRadius)
				{// ペンで色を塗る
					m_pPen->PaintCol(pData);
				}
			}
			else
			{// 直線と点の長さを計算
				float fLegth = pCap->Segment.vec.Cross(*pPixelPos - pCap->Segment.pos) / fSegLeng;
				// 距離を比較
				if (fLegth <= pCap->fRadius && fLegth >= -pCap->fRadius)
				{// ペンで色を塗る
					m_pPen->PaintCol(pData);
				}
			}
		}
	}
	
	// アンロック
	m_pTexture->UnlockRect(0);
}

//-------------------------------------------------------------------------------------------------------------
// 絵上のマウス位置の取得
//-------------------------------------------------------------------------------------------------------------
bool CPicture::GetMousePosOnPicture(void)
{
	// 前回の位置の設定
	m_pPen->SetPosOld();
	// マウスの取得
	CMouse  *pMouse = CManager::GetMouse();
	// 位置の算出
	m_pPen->PosCalculation(&D3DXVECTOR3(m_trans.mtxWorld._41, m_trans.mtxWorld._42, m_trans.mtxWorld._43), &m_PlaneNor);
	// 交点の取得
	D3DXVECTOR2 *pCrossPos = m_pPen->GetPos();
	// 絵上の位置の取得
	*pCrossPos = { pCrossPos->x - m_trans.mtxWorld._41,m_trans.mtxWorld._42 - pCrossPos->y };
	
	// 絵の上にいるか比較する
	if (0.0f > pCrossPos->x || pCrossPos->x > m_size.x ||
		0.0f > pCrossPos->y || pCrossPos->y > m_size.y)
	{
		CManager::GetMouse()->SetDisp(false);
	}
	else
	{
		CManager::GetMouse()->SetDisp(true);
	}

	// マウスの左クリックが押されている時
	return pMouse->GetPress(0);
}

//-------------------------------------------------------------------------------------------------------------
// 書き込み
//-------------------------------------------------------------------------------------------------------------
void CPicture::Writing(void)
{
	// ファイルを開く
	FILE *pFile = fopen(m_WriteToFile.Get(), "wb");
	// nullチェック
	if (pFile == NULL)
	{// 開けなかったとき
		return;
	}

	// ロックした情報
	D3DLOCKED_RECT LockRect;

	// ロック
	m_pTexture->LockRect(0, &LockRect, NULL, 0);

	D3DXCOLOR *pData = (D3DXCOLOR*)LockRect.pBits;				// カラーデータの取得

	// 最大数の書き込み
	fprintf(pFile, "max = %d\n", m_nNumDataMax);

	// カラーフラグの書き込み
	for (UINT nCntPixel = 0; nCntPixel < m_nNumDataMax; nCntPixel++, pData++)
	{
		fprintf(pFile, "%d%s", (pData->r > 0.5f) ? 1 : 0, ((nCntPixel % 128) == 127) ? "\n" : "");
	}

	// アンロック
	m_pTexture->UnlockRect(0);

	// ファイルを閉じる
	fclose(pFile);
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
void CPicture::Reading(LPDIRECT3DTEXTURE9 pTexture,CString & file)
{
	// データ格納
	MyVector<bool> bData;
	// ファイル読み込み
	if (CLoadFile::ReadLineByLineFromFile(file.Get(), TexterReadFromLine, &bData) != CLoadFile::LR_SUCCESS)
	{
		std::cout << "読み込み失敗しました。\n";
	}

	// ロックした情報
	D3DLOCKED_RECT LockRect;
	// ロック
	pTexture->LockRect(0, &LockRect, NULL, 0);

	D3DXCOLOR *pData = (D3DXCOLOR*)LockRect.pBits;				// カラーデータの取得
	// 読み込みサイズの取得
	UINT nReadSize = bData.size();
	// 最大数ループ
	for (UINT nCntPixel = 0; nCntPixel < m_nNumDataMax; nCntPixel++, pData++)
	{
		// 読み込みサイズよりカウントが大きいとき
		if (nReadSize <= nCntPixel)
		{// ループを抜ける
			break;
		}
		// データフラグが立っている時、白にする。それ以外の時、黒にする。
		*pData = (bData[nCntPixel] == true) ? D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}

	// アンロック
	pTexture->UnlockRect(0);
	// クリア
	bData.clear();
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャ情報の1行から情報を読み取る
//-------------------------------------------------------------------------------------------------------------
void CPicture::TexterReadFromLine(CONST_STRING cnpLine, void*pOut)
{
	// 変数宣言
	char aData[129]       = {};							// 文字列データ
	int nData             = MYLIB_INT_UNSET;			// INT型データ
	MyVector<bool> *pData = (MyVector<bool> *)pOut;		// キャストデータを取得

	// データ最大数
	if (sscanf(cnpLine, "max = %d", &nData) == 1)
	{// 内容量を変更
		pData->reserve(nData);
	}
	// フラグ情報を文字列として取得
	else if (sscanf(cnpLine, "%s",&aData[0]) == 1)
	{
		for (int nCnt = 0; nCnt < 128; nCnt++)
		{// 後ろに追加していく
			pData->push_back(aData[nCnt] == '1');
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 文字列から設定する
//-------------------------------------------------------------------------------------------------------------
void CPicture::SetFromString(CONST_STRING str)
{
	// 変数宣言
	INTEGER2    NumPoly = MYLIB_INT2_UNSET;
	D3DXVECTOR2 size = MYLIB_2DVECTOR_ZERO;
	D3DXVECTOR3 pos = MYLIB_VEC3_UNSET;
	char aFileName[MYLIB_STRINGSIZE] = { '\0' };

	// 位置
	if (sscanf(str, "pos = %f %f %f", &pos.x, &pos.y, &pos.z) == 3)
	{
		m_SetingPos = pos;
	}
	// ポリゴン数
	else if (sscanf(str, "NumPolygon = %d %d", &NumPoly.nX, &NumPoly.nY) == 2)
	{
		m_nNumPixelBlock = NumPoly;
	}
	// ポリゴンサイズ
	else if (sscanf(str, "Size = %f %f", &size.x, &size.y) == 2)
	{
		m_size = size;
	}
	// 書き込み先のファイル
	else if (sscanf(str, "WriteToFile = %s", aFileName) == 1)
	{
		m_WriteToFile = aFileName;
	}
}
