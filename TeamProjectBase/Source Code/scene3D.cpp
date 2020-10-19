//------------------------------------------------------------------------------
//
//シーン3D処理  [scene3D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
int CScene3D::m_nNumScene3D = 0;
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CScene3D::CScene3D()
{
	//初期化
	m_bBillBoard = false;

	//総数加算
	m_nNumScene3D++;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CScene3D::~CScene3D()
{
	//総数減算
	m_nNumScene3D--;
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CScene3D::Init()
{
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CScene3D::Uninit()
{

}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CScene3D::Update()
{

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	//Xを軸にしてポリゴンを生成すること
	pVtx[0].pos = D3DXVECTOR3(-GetSize().x * 0.5f, +GetSize().y * 0.5f, +GetSize().z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(+GetSize().x * 0.5f, +GetSize().y * 0.5f, +GetSize().z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-GetSize().x * 0.5f, -GetSize().y * 0.5f, -GetSize().z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(+GetSize().x * 0.5f, -GetSize().y * 0.5f, -GetSize().z * 0.5f);

	//頂点の色
	pVtx[0].col = GetColor();
	pVtx[1].col = GetColor();
	pVtx[2].col = GetColor();
	pVtx[3].col = GetColor();

	//頂点データをアンロック
	GetVtxBuff()->Unlock();

}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CScene3D::Draw()
{
	//ワールドマトリックス計算
	CHossoLibrary::CalcMatrix(GetMtxWorldPtr(), GetPos(), GetRot(), GetScale());

	//ビルボードかどうか
	if (m_bBillBoard)
	{
		//ビルボード設定
		CHossoLibrary::SetBillboard(GetMtxWorldPtr());
	}

	//nullcheck
	if (GetParentMtxPtr())
	{
		//親のマトリックスを掛け合わせる
		*GetMtxWorldPtr() *= *GetParentMtxPtr();
	}

	//ポリゴン描画
	DrawPolygon();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CScene3D::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}
//------------------------------------------------------------------------------
//設定処理
//------------------------------------------------------------------------------
void CScene3D::DrawSettingMtx(D3DXMATRIX const &Mtx)
{
	//引数のmtxを利用
	SetMtxWorld(Mtx);

	//ポリゴン描画
	DrawPolygon();
}
//------------------------------------------------------------------------------
//設定処理
//------------------------------------------------------------------------------
void CScene3D::DrawPolygon()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,GetMtxWorldPtr());

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, GetVtxBuff(), 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, GetTexture());

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
							0,					//開始するインデックス(頂点)
							2);					//ポリゴンの枚数

}

//------------------------------------------------------------------------------
//テクスチャアニメーションの更新
//------------------------------------------------------------------------------
void CScene3D::SetAnimation(D3DXVECTOR2 const UV, D3DXVECTOR2 const size)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(UV.x, UV.y);
	pVtx[1].tex = D3DXVECTOR2(UV.x + size.x, UV.y);
	pVtx[2].tex = D3DXVECTOR2(UV.x, UV.y + size.y);
	pVtx[3].tex = D3DXVECTOR2(UV.x + size.x, UV.y + size.y);

	//頂点データをアンロック
	GetVtxBuff()->Unlock();
}

//------------------------------------------------------------------------------
//生成処理　共有管理用
//------------------------------------------------------------------------------
std::shared_ptr<CScene3D> CScene3D::Create_Shared(D3DXVECTOR3 const pos, D3DXVECTOR3 const size, D3DXCOLOR const col, OBJTYPE const objtype)
{
	//メモリ確保
	std::shared_ptr<CScene3D> pScene3D = std::make_shared<CScene3D>();

	//初期化
	pScene3D->Init();

	//座標とサイズ設定
	pScene3D->MakeVertex(pos, size, col);

	//Scene側で管理
	pScene3D->SetObjType(objtype);
	pScene3D->AddSharedList(pScene3D);

	return pScene3D;
}
//------------------------------------------------------------------------------
//生成処理　Csceneで管理用
//------------------------------------------------------------------------------
void CScene3D::Create_SceneManagement(D3DXVECTOR3 const pos, D3DXVECTOR3 const size, D3DXCOLOR const col, OBJTYPE const objtype)
{
	//メモリ確保
	std::unique_ptr<CScene3D> pScene3D = std::make_unique<CScene3D>();

	//初期化
	pScene3D->Init();

	//座標とサイズ設定
	pScene3D->MakeVertex(pos, size, col);

	//Sceneで管理
	pScene3D->SetObjType(objtype);
	pScene3D->AddUniqueList(std::move(pScene3D));
}
//------------------------------------------------------------------------------
//生成処理　Return先で管理用
//------------------------------------------------------------------------------
std::unique_ptr<CScene3D> CScene3D::Create_SelfManagement(D3DXVECTOR3 const pos, D3DXVECTOR3 const size, D3DXCOLOR const col)
{
	//メモリ確保
	std::unique_ptr<CScene3D> pScene3D = std::make_unique<CScene3D>();

	//初期化
	pScene3D->Init();

	//座標とサイズ設定
	pScene3D->MakeVertex(pos, size, col);

	//return
	return std::move(pScene3D);
}


//------------------------------------------------------------------------------
//頂点バッファ生成
//------------------------------------------------------------------------------
HRESULT CScene3D::MakeVertex(D3DXVECTOR3 const pos, D3DXVECTOR3 const size, D3DXCOLOR const col)
{
	//情報設定
	SetPos(pos);
	SetSize(size);
	SetColor(col);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//頂点バッファ

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL)))
	{
		//頂点バッファ作成失敗
		return E_FAIL;
	}

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	//Xを軸にしてポリゴンを生成すること
	pVtx[0].pos = D3DXVECTOR3(-GetSize().x * 0.5f, +GetSize().y * 0.5f, +GetSize().z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(+GetSize().x * 0.5f, +GetSize().y * 0.5f, +GetSize().z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-GetSize().x * 0.5f, -GetSize().y * 0.5f, -GetSize().z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(+GetSize().x * 0.5f, -GetSize().y * 0.5f, -GetSize().z * 0.5f);

	//頂点の同次座標
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点の色
	pVtx[0].col = GetColor();
	pVtx[1].col = GetColor();
	pVtx[2].col = GetColor();
	pVtx[3].col = GetColor();

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロック
	pVtxBuff->Unlock();

	//頂点バッファ設定
	BindVtxBuff(pVtxBuff);

	return S_OK;

}
