//------------------------------------------------------------------------------
//
//シーン2D処理  [scene2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
int CScene2D::m_nNumScene2D = 0;

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CScene2D::CScene2D()
{
	//初期化
	m_fAngle = 0.0f;
	m_fLength = 0.0f;

	//総数加算
	m_nNumScene2D++;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CScene2D::~CScene2D()
{
	//総数減算
	m_nNumScene2D--;
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CScene2D::Init()
{
	MakeVertex();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CScene2D::Update()
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//nullcheck
	if (!GetVtxBuff())
	{
		return;
	}

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos.x = GetPos().x + sinf(-D3DX_PI + m_fAngle + GetRot().z)	* m_fLength;
	pVtx[0].pos.y = GetPos().y + cosf(-D3DX_PI + m_fAngle + GetRot().z)	* m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = GetPos().x + sinf(D3DX_PI - m_fAngle + GetRot().z)	* m_fLength;
	pVtx[1].pos.y = GetPos().y + cosf(D3DX_PI - m_fAngle + GetRot().z)	* m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = GetPos().x + sinf(-m_fAngle + GetRot().z)	* m_fLength;
	pVtx[2].pos.y = GetPos().y + cosf(-m_fAngle + GetRot().z)	* m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = GetPos().x + sinf(m_fAngle + GetRot().z)	* m_fLength;
	pVtx[3].pos.y = GetPos().y + cosf(m_fAngle + GetRot().z)	* m_fLength;
	pVtx[3].pos.z = 0.0f;

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
void CScene2D::Draw()
{
	//写すかどうか
	if (GetDisp())
	{
		//デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, GetVtxBuff(), 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, GetTexture());

		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
								0,					//開始するインデックス(頂点)
								2);					//ポリゴンの枚数
	}
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CScene2D::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//テクスチャアニメーションの更新
//------------------------------------------------------------------------------
void CScene2D::SetAnimation(D3DXVECTOR2 UV, D3DXVECTOR2 size)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

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
//頂点バッファ作成
//------------------------------------------------------------------------------
HRESULT CScene2D::MakeVertex()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//頂点バッファ

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL)))
	{
		//頂点バッファ作成失敗
		return E_FAIL;
	}

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos.x = GetPos().x + sinf(-D3DX_PI + m_fAngle + GetRot().z)	* m_fLength;
	pVtx[0].pos.y = GetPos().y + cosf(-D3DX_PI + m_fAngle + GetRot().z)	* m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = GetPos().x + sinf(D3DX_PI - m_fAngle + GetRot().z)	* m_fLength;
	pVtx[1].pos.y = GetPos().y + cosf(D3DX_PI - m_fAngle + GetRot().z)	* m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = GetPos().x + sinf(-m_fAngle + GetRot().z)	* m_fLength;
	pVtx[2].pos.y = GetPos().y + cosf(-m_fAngle + GetRot().z)	* m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = GetPos().x + sinf(m_fAngle + GetRot().z)	* m_fLength;
	pVtx[3].pos.y = GetPos().y + cosf(m_fAngle + GetRot().z)	* m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点の同次座標
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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

//------------------------------------------------------------------------------
//サイズ設定 ポリゴン回転用の変数の計算もしておく
//------------------------------------------------------------------------------
void CScene2D::SetSize(D3DXVECTOR3 const &size)
{
	CSceneBase::SetSize(size);

	m_fAngle = atan2f(GetSize().x, GetSize().y);
	m_fLength = (float)sqrt(GetSize().x * GetSize().x + GetSize().y * GetSize().y) / 2;

}