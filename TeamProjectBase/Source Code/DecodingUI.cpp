//*************************************************************************************************************
//
// 解読書のUI[処理 [DecodingUI.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "DecodingUI.h"
#include "manager.h"
#include "renderer.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバの初期化
//-------------------------------------------------------------------------------------------------------------
const float      CDecodingUI::m_fHalfPi    = D3DX_PI * MYLIB_HALF_SIZE;		// 円周率の半分の値
const float      CDecodingUI::m_fMinHalfPi = D3DX_PI * -MYLIB_HALF_SIZE;	// 円周率の半分の負の値

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CDecodingUI::CDecodingUI()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CDecodingUI::~CDecodingUI()
{
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
std::shared_ptr<CDecodingUI> CDecodingUI::Create(SETINGINFO Seting)
{
	// スマートポインタの生成
	std::shared_ptr<CDecodingUI> pPicture = std::make_shared<CDecodingUI>();
	// 設定用情報から設定する
	pPicture->SetFromSetingInfo(Seting);
	// 初期化
	pPicture->Init();

	//Sceneで管理
	pPicture->SetObjType(OBJTYPE_UI);
	pPicture->AddSharedList(pPicture);
	return pPicture;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CDecodingUI::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	try
	{// 頂点情報の作成
		MakeVertex(pDevice);
	}
	catch (HRESULT rh)
	{
		return rh;
	}
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CDecodingUI::Update()
{
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CDecodingUI::Draw()
{
	if (m_bDisp)
	{
		//デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, GetTexture());

		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,										//開始するインデックス(頂点)
			2);										//ポリゴンの枚数
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の更新
//-------------------------------------------------------------------------------------------------------------
void CDecodingUI::UpdateVertex(bool bPos, bool bCol)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;
	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bPos)
	{//頂点の座標
		SetVatexPos(pVtx, m_OriginType, &m_pos, &m_size, m_fAngle);
	}

	if (bCol)
	{//頂点の色
		SetVatexCol(pVtx);
	}

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の更新
//-------------------------------------------------------------------------------------------------------------
void CDecodingUI::UpdateVertex(D3DXVECTOR3 * pPos, D3DXVECTOR2 * pSize, D3DXCOLOR * pCol)
{
	// 頂点情報の更新
	UpdateVertex(SetWithPosPtr(pPos) || SetWithSizePtr(pSize), SetWithColPtr(pCol));
}

//-------------------------------------------------------------------------------------------------------------
// 設定用情報の設定
//-------------------------------------------------------------------------------------------------------------
void CDecodingUI::SetFromSetingInfo(CONST SETINGINFO & Seting)
{
	m_pos = Seting.pos;
	m_size = Seting.size;
	m_col = Seting.col;
	m_fAngle = Seting.fAngle;
	m_OriginType = Seting.OriginType;
	m_bDisp = Seting.bDisp;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の作成
//-------------------------------------------------------------------------------------------------------------
void CDecodingUI::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		//頂点バッファ作成失敗
		throw E_FAIL;
	}

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	SetVatexPos(pVtx, m_OriginType, &m_pos, &m_size, m_fAngle);

	//頂点の同次座標
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点の色
	SetVatexCol(pVtx);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点位置の設定
//-------------------------------------------------------------------------------------------------------------
void CDecodingUI::SetVatexPos(VERTEX_2D *pVtx, ORIGINVERTEXTYPE type, D3DXVECTOR3 * pPos, D3DXVECTOR2 * pSize, float fRotation)
{
	// 変数宣言
	D3DXVECTOR2 halfSize = *pSize * MYLIB_HALF_SIZE;	// 半分の大きさ
	switch (type)
	{
	case ORIGINVERTEXTYPE_LOWERLEFT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x,            pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x,            pPos->y,            MYLIB_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y,            MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_LOWERCENTER:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - halfSize.x, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + halfSize.x, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - halfSize.x, pPos->y,            MYLIB_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + halfSize.x, pPos->y,            MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_LOWERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x, pPos->y - pSize->y,            MYLIB_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y,            MYLIB_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x, pPos->y,                       MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_CENTERLEFT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x,            pPos->y - halfSize.y, MYLIB_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y - halfSize.y, MYLIB_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x,            pPos->y + halfSize.y, MYLIB_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y + halfSize.y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_CENTER:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - halfSize.x, pPos->y - halfSize.y, MYLIB_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + halfSize.x, pPos->y - halfSize.y, MYLIB_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - halfSize.x, pPos->y + halfSize.y, MYLIB_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + halfSize.x, pPos->y + halfSize.y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_CENTERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y - halfSize.y, MYLIB_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x,            pPos->y - halfSize.y, MYLIB_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y - halfSize.y, MYLIB_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x,            pPos->y - halfSize.y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_UPPERLEFT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x,            pPos->y,            MYLIB_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y,            MYLIB_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x,            pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_UPPERCENTER:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y,            MYLIB_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y,            MYLIB_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_UPPERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y,            MYLIB_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x,            pPos->y,            MYLIB_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x,            pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_ROTCENTER:
		pVtx[0].pos.x = pPos->x + sinf(D3DX_PI + fRotation)*halfSize.y + sinf(m_fMinHalfPi + fRotation)*halfSize.x;
		pVtx[0].pos.y = pPos->y + cosf(D3DX_PI + fRotation)*halfSize.y + cosf(m_fMinHalfPi + fRotation)*halfSize.x;
		pVtx[1].pos.x = pPos->x + sinf(D3DX_PI + fRotation)*halfSize.y + sinf(m_fHalfPi + fRotation)*halfSize.x;
		pVtx[1].pos.y = pPos->y + cosf(D3DX_PI + fRotation)*halfSize.y + cosf(m_fHalfPi + fRotation)*halfSize.x;
		pVtx[2].pos.x = pPos->x + sinf(fRotation)*halfSize.y           + sinf(m_fMinHalfPi + fRotation)*halfSize.x;
		pVtx[2].pos.y = pPos->y + cosf(fRotation)*halfSize.y           + cosf(m_fMinHalfPi + fRotation)*halfSize.x;
		pVtx[3].pos.x = pPos->x + sinf(fRotation)*halfSize.y           + sinf(m_fHalfPi + fRotation)*halfSize.x;
		pVtx[3].pos.y = pPos->y + cosf(fRotation)*halfSize.y           + cosf(m_fHalfPi + fRotation)*halfSize.x;
		pVtx[0].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[1].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[2].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[3].pos.z = MYLIB_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTCENTERLEFT:
		pVtx[0].pos.x = pPos->x + sinf(D3DX_PI + fRotation)*halfSize.y;
		pVtx[0].pos.y = pPos->y + cosf(D3DX_PI + fRotation)*halfSize.y;
		pVtx[1].pos.x = pPos->x + sinf(D3DX_PI + fRotation)*halfSize.y + sinf(m_fHalfPi + fRotation)*(pSize->x);
		pVtx[1].pos.y = pPos->y + cosf(D3DX_PI + fRotation)*halfSize.y + cosf(m_fHalfPi + fRotation)*(pSize->x);
		pVtx[2].pos.x = pPos->x + sinf(fRotation)*halfSize.y;
		pVtx[2].pos.y = pPos->y + cosf(fRotation)*halfSize.y;
		pVtx[3].pos.x = pPos->x + sinf(fRotation)*halfSize.y + sinf(m_fHalfPi + fRotation)*(pSize->x);
		pVtx[3].pos.y = pPos->y + cosf(fRotation)*halfSize.y + cosf(m_fHalfPi + fRotation)*(pSize->x);
		pVtx[0].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[1].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[2].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[3].pos.z = MYLIB_FLOAT_UNSET;
		break;

	case ORIGINVERTEXTYPE_ROTLOWERCENTER:
		pVtx[2].pos.x = pPos->x + sinf(m_fMinHalfPi + fRotation)*halfSize.x;
		pVtx[2].pos.y = pPos->y + cosf(m_fMinHalfPi + fRotation)*halfSize.x;
		pVtx[3].pos.x = pPos->x + sinf(m_fHalfPi + fRotation)*halfSize.x;
		pVtx[3].pos.y = pPos->y + cosf(m_fHalfPi + fRotation)*halfSize.x;
		pVtx[0].pos.x = pVtx[2].pos.x + sinf(D3DX_PI + fRotation)*(pSize->y);
		pVtx[0].pos.y = pVtx[2].pos.y + cosf(D3DX_PI + fRotation)*(pSize->y);
		pVtx[1].pos.x = pVtx[3].pos.x + sinf(D3DX_PI + fRotation)*(pSize->y);
		pVtx[1].pos.y = pVtx[3].pos.y + cosf(D3DX_PI + fRotation)*(pSize->y);

		pVtx[0].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[1].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[2].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[3].pos.z = MYLIB_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTCENTERRIGHT:
		pVtx[0].pos.x = pPos->x + sinf(D3DX_PI + fRotation)*halfSize.y + sinf(m_fMinHalfPi + fRotation)*(pSize->x);
		pVtx[0].pos.y = pPos->y + cosf(D3DX_PI + fRotation)*halfSize.y + cosf(m_fMinHalfPi + fRotation)*(pSize->x);
		pVtx[1].pos.x = pPos->x + sinf(D3DX_PI + fRotation)*halfSize.y;
		pVtx[1].pos.y = pPos->y + cosf(D3DX_PI + fRotation)*halfSize.y;
		pVtx[2].pos.x = pPos->x + sinf(fRotation)*halfSize.y + sinf(m_fMinHalfPi + fRotation)*(pSize->x);
		pVtx[2].pos.y = pPos->y + cosf(fRotation)*halfSize.y + cosf(m_fMinHalfPi + fRotation)*(pSize->x);
		pVtx[3].pos.x = pPos->x + sinf(fRotation)*halfSize.y;
		pVtx[3].pos.y = pPos->y + cosf(fRotation)*halfSize.y;
		pVtx[0].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[1].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[2].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[3].pos.z = MYLIB_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTUPPERCENTER:
		pVtx[0].pos.x = pPos->x + sinf(m_fMinHalfPi + fRotation)*halfSize.x;
		pVtx[0].pos.y = pPos->y + cosf(m_fMinHalfPi + fRotation)*halfSize.x;
		pVtx[1].pos.x = pPos->x + sinf(m_fHalfPi + fRotation)*halfSize.x;
		pVtx[1].pos.y = pPos->y + cosf(m_fHalfPi + fRotation)*halfSize.x;
		pVtx[2].pos.x = pVtx[0].pos.x + sinf(fRotation)*(pSize->y);
		pVtx[2].pos.y = pVtx[0].pos.y + cosf(fRotation)*(pSize->y);
		pVtx[3].pos.x = pVtx[1].pos.x + sinf(fRotation)*(pSize->y);
		pVtx[3].pos.y = pVtx[1].pos.y + cosf(fRotation)*(pSize->y);
		pVtx[0].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[1].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[2].pos.z = MYLIB_FLOAT_UNSET;
		pVtx[3].pos.z = MYLIB_FLOAT_UNSET;
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点カラーの設定
//-------------------------------------------------------------------------------------------------------------
void CDecodingUI::SetVatexCol(VERTEX_2D * pVtx)
{
	//頂点の色
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = m_col;
}
