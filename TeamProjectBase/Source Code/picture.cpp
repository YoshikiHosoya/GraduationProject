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

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define PICTURE_FILENAME  "data/TEXT/PictureInfo.txt"

//-------------------------------------------------------------------------------------------------------------
// マクロ関数
//-------------------------------------------------------------------------------------------------------------
#define Paint(col) Mybfunc_bit_set(col,0)
#define Clear(col) Mybfunc_bit_clear(col,0)

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
INTEGER2    CPicture::m_nNumPolyBlock   = MYLIB_INT2_UNSET;		// ポリゴン数(ずらす大きさの設定後2倍にする)
FLOAT2      CPicture::m_size            = MYLIB_2DVECTOR_ZERO;	// 大きさ
D3DXMATRIX* CPicture::m_pMtxView = nullptr;
D3DXMATRIX* CPicture::m_pMtxProj = nullptr;
INTEGER2    CPicture::m_MousePos = MYLIB_INT2_UNSET;
INTEGER2    CPicture::m_ScreenPos = INTEGER2(SCREEN_WIDTH, SCREEN_HEIGHT);
D3DXVECTOR3 CPicture::m_PlaneNor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
D3DXVECTOR2 CPicture::m_PixelSize = MYLIB_VEC2_UNSET;

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
		std::cout << "<<<<<<CPictureのファイル読み込みが失敗しました。>>>>>>\n";
#endif // _DEBUG
		return E_FAIL;
	}
	// どちらもゼロの時
	if (m_nNumPolyBlock.OneIsZero() == true)
	{
#ifdef _DEBUG
		std::cout << "<<<<<<CPictureのどちらかのポリゴン数が0でした。>>>>>>\n";
#endif // _DEBUG
		return E_FAIL;
	}
	CCamera *pCamera = CManager::GetRenderer()->GetCamera();
	m_pMtxView = pCamera->GetMtxView();
	m_pMtxProj = pCamera->GetMtxProjection();

	m_PixelSize = m_size / m_nNumPolyBlock;
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
void CPicture::InitStaticMember(void)
{
	m_nNumPolyBlock = MYLIB_INT2_UNSET;		// ポリゴン数(ずらす大きさの設定後2倍にする)
	m_size          = MYLIB_2DVECTOR_ZERO;	// 大きさ
	m_pMtxView = nullptr;
	m_pMtxProj = nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// マトリックスの計算
//-------------------------------------------------------------------------------------------------------------
void CPicture::MatrixCal(void)
{
	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// マトリックスの初期化
	trans.Identity();

	// スケールの反映
	trans.mtxWorld.m[0][0] = trans.scal.x;
	trans.mtxWorld.m[1][1] = trans.scal.y;
	trans.mtxWorld.m[2][2] = trans.scal.z;

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, trans.rot.y, trans.rot.x, trans.rot.z);
	D3DXMatrixMultiply(&trans.mtxWorld, &trans.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, trans.pos.x, trans.pos.y, trans.pos.z);
	D3DXMatrixMultiply(&trans.mtxWorld, &trans.mtxWorld, &mtxTrans);
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
	m_Flags.cValue = MASK_DISP;

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
// 終了
//-------------------------------------------------------------------------------------------------------------
void CPicture::Uninit()
{
	// 頂点バッファの取得
	if (pVtexBuff != nullptr)
	{
		pVtexBuff->Release();
		pVtexBuff = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CPicture::Update()
{
	// 変数宣言
	CMouse  *pMouse = CManager::GetMouse();
	INTEGER2 MousePos(pMouse->GetMousePoint().x, pMouse->GetMousePoint().y);
	FLOAT3 StartPos;
	FLOAT3 EndPos;
	VEC3 Ray;
	CMylibrary::CalScreenRay(&Ray, &StartPos, &EndPos,&MousePos, &m_ScreenPos, (MATRIX*)m_pMtxView,(MATRIX*)m_pMtxProj);
	D3DXVECTOR3 pos;
	CMylibrary::CalIntersectionPointToPlaneAndLine(&pos, &trans.pos, &m_PlaneNor, &StartPos, &Ray);

	if (pMouse->GetPress(0))
	{

#ifdef _DEBUG
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "マウスのワールド座標 == [%.3f][%.3f][%.3f]\n", pos.x, pos.y, pos.z);
#endif

		if (trans.pos.x < pos.x && pos.x < trans.pos.x + m_size.x &&
			trans.pos.y > pos.y && pos.y > trans.pos.y - m_size.y)
		{
#ifdef _DEBUG
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "X軸Y軸に入っている\n");
#endif
			D3DXVECTOR2 PicturePos = { pos.x - trans.pos.x,trans.pos.y - pos.y };

			CDebugProc::Print(CDebugProc::PLACE_LEFT, "マウスの絵上の座標 == [%.3f][%.3f]\n", PicturePos.x, PicturePos.y);

			float fNumX = PicturePos.x / m_PixelSize.x;
			float fNumY = PicturePos.y / m_PixelSize.y;
			int nNumX = (int)fNumX;
			int nNumY = (int)fNumY;

			D3DXCOLOR *data;
			D3DLOCKED_RECT LockRect;
			m_pTexture->LockRect(0, &LockRect, NULL, 0);
			data = (D3DXCOLOR*)LockRect.pBits;
			for (int nY = 0; nY < m_nNumPolyBlock.nY; nY++)
			{
				for (int nX = 0; nX < m_nNumPolyBlock.nX; nX++)
				{
					D3DXVECTOR2 PixelPos = {
						nX * m_PixelSize.x + m_PixelSize.x * 0.5f,
						nY * m_PixelSize.y + m_PixelSize.y * 0.5f };
					D3DXVECTOR2 Dist = PixelPos - PicturePos;
					float fSize = 20.0f;
					if (Dist.x * Dist.x + Dist.y * Dist.y <= fSize)
					{
						*data = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					}
					data++;
				}
			}
			m_pTexture->UnlockRect(0);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CPicture::Draw()
{
	//CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_COMMAND::RENDERER_WIRE_ON);
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &trans.mtxWorld);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, pVtexBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,					//開始するインデックス(頂点)
		2);					//ポリゴンの枚数

	//CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_COMMAND::RENDERER_WIRE_OFF);
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

	//Sceneで管理
	pPicture->SetObjType(OBJTYPE_MESHFIELD);
	pPicture->AddSharedList(pPicture);
	return pPicture;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャの作成
//-------------------------------------------------------------------------------------------------------------
void CPicture::MakeTexture(LPDIRECT3DDEVICE9 pDevice)
{
#if 1
	if (FAILED(pDevice->CreateTexture(m_nNumPolyBlock.nX, m_nNumPolyBlock.nY, 1, 0, D3DFMT_A32B32G32R32F,
		D3DPOOL_MANAGED, &m_pTexture, NULL)))
	{
		throw E_FAIL;
	}
#else
	if (D3DXCreateTextureFromFile(pDevice, m_TextureFile.Get(), &m_pTexture) != S_OK)
	{
		throw E_FAIL;
	}
#endif
	D3DXCOLOR *data;
	//RECT           Rect;
	D3DLOCKED_RECT LockRect;
	HRESULT hr = m_pTexture->LockRect(0, &LockRect, NULL, 0);
	data = (D3DXCOLOR*)LockRect.pBits;
	for (int nY = 0; nY < m_nNumPolyBlock.nY; nY++)
	{
		for (int nX = 0; nX < m_nNumPolyBlock.nX; nX++)
		{
			if (nX == 0 || nX == m_nNumPolyBlock.nX - 1 ||
				nX == 0 || nY == m_nNumPolyBlock.nY - 1)
			{
				*data = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}
			else
			{
				*data = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			data++;
		}
	}
	m_pTexture->UnlockRect(0);
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
		&pVtexBuff,
		NULL)))
	{
		throw E_FAIL;
	}

	// 変数宣言
	VERTEX_3D *pVtx;								// 頂点情報の設定
	FLOAT2    sizeUpdate = MYLIB_2DVECTOR_ZERO;		// 更新する大きさ

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	pVtexBuff->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtexBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 1行ずつ読みこむ
//-------------------------------------------------------------------------------------------------------------
void CPicture::ReadFromLine(CONST_STRING Line)
{
	INTEGER2    NumPoly = MYLIB_INT2_UNSET;
	D3DXVECTOR2 size    = MYLIB_2DVECTOR_ZERO;
	char aFileName[MYLIB_STRINGSIZE] = { '\0' };

	if (sscanf(Line, "NumPolygon = %d %d", &NumPoly.nX , &NumPoly.nY) == 2)
	{
		m_nNumPolyBlock = NumPoly;
	}
	else if (sscanf(Line, "Size = %f %f",&size.x, &size.y) == 2)
	{
		m_size = size;
	}
}
