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

//-------------------------------------------------------------------------------------------------------------
//マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define PICTURE_FILENAME  "data/TEXT/PictureInfo.txt"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
INTEGER2    CPicture::m_nNumPolyBlock   = MYLIB_INT2_UNSET;		// ポリゴン数(ずらす大きさの設定後2倍にする)
UINT        CPicture::m_nNumVertex      = MYLIB_INT_UNSET;		// 総頂点数
UINT        CPicture::m_nNumIndex       = MYLIB_INT_UNSET;		// 総インデックス数
UINT        CPicture::m_nNumPolygon     = MYLIB_INT_UNSET;		// 総ポリゴン数
FLOAT2      CPicture::m_size            = MYLIB_2DVECTOR_ZERO;	// 大きさ
FLOAT2      CPicture::m_sizeShift       = MYLIB_2DVECTOR_ZERO;	// ずらす大きさ

//-------------------------------------------------------------------------------------------------------------
// メッシュ情報のコンストラクタ
//-------------------------------------------------------------------------------------------------------------
CPicture::MESHINFO::MESHINFO()
{
	pVtexBuff = nullptr;								// 頂点バッファのポインタ
	pIdxBuff = nullptr;								// インデックスのバッファのポインタ
	pTexture = nullptr;								// テクスチャのポインタ}
}

//-------------------------------------------------------------------------------------------------------------
// メッシュ情報のデストラクタ
//-------------------------------------------------------------------------------------------------------------
CPicture::MESHINFO::~MESHINFO()
{
	// 頂点バッファの開放
	if (pVtexBuff != nullptr)
	{
		pVtexBuff->Release();
		pVtexBuff = nullptr;
	}
	// インデックスバッファの開放
	if (pIdxBuff != nullptr)
	{
		pIdxBuff->Release();
		pIdxBuff = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CPicture::Load(void)
{
	// * メモ [Parameter]
	// ファイル読み込み
	if (CLoadFile::ReadLineByLineFromFile(PICTURE_FILENAME, ReadFromLine) != CLoadFile::LR_SUCCESS)
	{
#ifdef _DEBUG
		cout << "<<<<<<CPictureのファイル読み込みが失敗しました。>>>>>>\n";
#endif // _DEBUG
		return E_FAIL;
	}
	// どちらもゼロの時
	if (m_nNumPolyBlock.OneIsZero() == true)
	{
#ifdef _DEBUG
		cout << "<<<<<<CPictureのどちらかのポリゴン数が0でした。>>>>>>\n";
#endif // _DEBUG
		return E_FAIL;
	}

	// ずらす大きさを設定
	m_sizeShift = m_size / m_nNumPolyBlock;

	// ポリゴン数を2倍にする
	m_nNumPolyBlock *= 2;
	
	// 総数の設定
	m_nNumVertex  = (m_nNumPolyBlock.nX + 1) * (m_nNumPolyBlock.nY + 1);								// 総頂点数
	m_nNumIndex   = (m_nNumPolyBlock.nX * 2 + 2) * m_nNumPolyBlock.nY + ((m_nNumPolyBlock.nY - 1) * 2);	// 総インデックス数
	m_nNumPolygon = (m_nNumPolyBlock.nX * m_nNumPolyBlock.nY) * 2 + (m_nNumPolyBlock.nY - 1) * 4;		// 総ポリゴン数

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
void CPicture::InitStaticMember(void)
{
	m_nNumPolyBlock = MYLIB_INT2_UNSET;		// ポリゴン数(ずらす大きさの設定後2倍にする)
	m_nNumVertex    = MYLIB_INT_UNSET;		// 総頂点数
	m_nNumIndex     = MYLIB_INT_UNSET;		// 総インデックス数
	m_nNumPolygon   = MYLIB_INT_UNSET;		// 総ポリゴン数
	m_size          = MYLIB_2DVECTOR_ZERO;	// 大きさ
	m_sizeShift     = MYLIB_2DVECTOR_ZERO;	// ずらす大きさ
}

//-------------------------------------------------------------------------------------------------------------
// マトリックスの計算
//-------------------------------------------------------------------------------------------------------------
void CPicture::MatrixCal(void)
{
	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// マトリックスの初期化
	m_mesh.trans.Identity();

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_mesh.trans.rot.y, m_mesh.trans.rot.x, m_mesh.trans.rot.z);
	D3DXMatrixMultiply(&m_mesh.trans.mtxWorld, &m_mesh.trans.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_mesh.trans.pos.x, m_mesh.trans.pos.y, m_mesh.trans.pos.z);
	D3DXMatrixMultiply(&m_mesh.trans.mtxWorld, &m_mesh.trans.mtxWorld, &mtxTrans);
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
	m_Flags.cValue = 3;

	try
	{	// 頂点情報の作成
		MakeVertex(pDevice);
		// インデックス情報の作成
		MakeIndex(pDevice);
	}
	catch (HRESULT rh)
	{// 例外を返す
		return rh;
	}
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CPicture::Uninit()
{
	if (m_mesh.pVtexBuff != nullptr)
	{
		m_mesh.pVtexBuff->Release();
		m_mesh.pVtexBuff = nullptr;
	}
	if (m_mesh.pIdxBuff != nullptr)
	{
		m_mesh.pIdxBuff->Release();
		m_mesh.pIdxBuff = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CPicture::Update()
{
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CPicture::Draw()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mesh.trans.mtxWorld);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_mesh.pVtexBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデバイスのデータストリームにバインド
	pDevice->SetIndices(m_mesh.pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_mesh.pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,
		m_nNumVertex,			//使用する頂点数
		0,						//頂点の読み取りを開始する位置
		m_nNumPolygon);			//ポリゴンの枚数
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
std::shared_ptr<CPicture> CPicture::Create(CONST D3DXVECTOR3 &pos, CONST D3DXVECTOR3 &rot)
{
	// スマートポインタの生成
	std::shared_ptr<CPicture> pPicture = std::make_shared<CPicture>();
	// 位置の設定
	pPicture->SetPos(pos);
	// 向きの設定
	pPicture->SetRot(rot);
	// 初期化
	pPicture->Init();
	return pPicture;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の作成
//-------------------------------------------------------------------------------------------------------------
void CPicture::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_mesh.pVtexBuff,
		NULL)))
	{
		throw E_FAIL;
	}

	// 変数宣言
	VERTEX_3D *pVtx;								// 頂点情報の設定
	FLOAT2    sizeUpdate = MYLIB_2DVECTOR_ZERO;		// 更新する大きさ

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_mesh.pVtexBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 縦の四角形の個数分
	for (int nCntVertical = 0; nCntVertical < m_nNumPolyBlock.nY + 1; nCntVertical++)
	{
		// 横の四角形の個数分
		for (int nCntHeng = 0; nCntHeng < m_nNumPolyBlock.nX + 1; nCntHeng++)
		{
			// 位置の設定
			pVtx[0].pos = D3DXVECTOR3(sizeUpdate.x, sizeUpdate.y, MYLIB_INT_UNSET);
			// 法線の設定
			pVtx[0].nor = MYLIB_VEC3_ZONE;
			// 色の設定
			pVtx[0].col = MYLIB_D3DXCOR_SET;
			// UV情報の設定
			pVtx[0].tex = D3DXVECTOR2(1.0f*nCntHeng, 1.0f*nCntVertical);
			// 頂点ポインタのインクリメント
			pVtx++;
			// 更新する大きさX軸を加算
			if (nCntHeng % 2 == 0)sizeUpdate.x += m_sizeShift.x;
		}
		// 更新する大きさX軸の初期化
		sizeUpdate.x = MYLIB_FLOAT_UNSET;
		// 更新する大きさY軸を加算
		if (nCntVertical % 2 == 0)sizeUpdate.y += m_sizeShift.y;
	}
	// 頂点データをアンロック
	m_mesh.pVtexBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// インデックス情報の作成
//-------------------------------------------------------------------------------------------------------------
void CPicture::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_mesh.pIdxBuff,
		NULL)))
	{
		throw E_FAIL;
	}

	// インデックスデータへのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、インデックスバッファのポインタ取得
	m_mesh.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	// 奥行の個数
	for (int nCntVertical = 0; nCntVertical < m_nNumPolyBlock.nY ; nCntVertical++)
	{
		// 最初だけはいらないようにするため
		if (nCntVertical > 0)
		{
			// 縮退ポリゴン分
			pIdx[0] = (nCntVertical*(m_nNumPolyBlock.nX + 1) + m_nNumPolyBlock.nX + 1);
			pIdx++;
		}
		// 横の個数
		for (int nCntInd = 0; nCntInd < m_nNumPolyBlock.nX + 1; nCntInd++)
		{
			// 縮退ポリゴンを除いた分
			pIdx[0] = nCntVertical*(m_nNumPolyBlock.nX + 1) + nCntInd + m_nNumPolyBlock.nX + 1;
			pIdx[1] = nCntVertical*(m_nNumPolyBlock.nX + 1) + nCntInd;
			pIdx += 2;
		}
		// 最後だけ入らないようにする
		if (nCntVertical < m_nNumPolyBlock.nY - 1)
		{
			// 縮退ポリゴン分
			pIdx[0] = (nCntVertical*(m_nNumPolyBlock.nX + 1) + m_nNumPolyBlock.nX);
			pIdx++;
		}
	}
	// インデックスバッファをアンロック
	m_mesh.pIdxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 1行ずつ読みこむ
//-------------------------------------------------------------------------------------------------------------
void CPicture::ReadFromLine(CONST_STRING Line)
{
	INTEGER2    NumPoly = MYLIB_INT2_UNSET;
	D3DXVECTOR2 size    = MYLIB_2DVECTOR_ZERO;
	if (sscanf(Line, "NumPolygon = %d %d", &NumPoly.nX , &NumPoly.nY) == 2)
	{
		m_nNumPolyBlock = NumPoly;
	}
	else if (sscanf(Line, "Size = %f %f",&size.x, &size.y) == 2)
	{
		m_size = size;
	}
}
