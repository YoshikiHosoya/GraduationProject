//------------------------------------------------------------------------------
//
//Xファイルモデル処理  [SceneX.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "sceneX.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "light.h"
#include "modelinfo.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
int CSceneX::m_nNumSceneX = 0;

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define SHADOW_COLOR (D3DXCOLOR(0.0f,0.0f,0.0f,0.5f))
#define STENCIL_COLOR (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define HARDEDGE_SCALE (D3DXVECTOR3(1.1f,1.1f,1.1f))
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CSceneX::CSceneX()
{
	//初期化
	m_SceneXInfo = nullptr;
	m_bSelecting = false;

	//総数加算
	m_nNumSceneX++;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CSceneX::~CSceneX()
{
	//ポインタの開放
	m_SceneXInfo = nullptr;

	//総数減算
	m_nNumSceneX--;
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CSceneX::Init()
{

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CSceneX::Update()
{
}
//------------------------------------------------------------------------------
//描画処理
//親のマトリックスが存在したときには親のマトリックスを掛け合わせる
//------------------------------------------------------------------------------
void CSceneX::Draw()
{
	//選択されているとき
	if (m_bSelecting)
	{
		//ハードエッジ描画
		DrawHardEdgeStencil();
	}

	//マトリックス計算
	CHossoLibrary::CalcMatrix(GetMtxWorldPtr(), GetPos(), GetRot(),GetScale());

	//nullcheck
	if (GetParentMtxPtr())
	{
		//親のマトリックスを掛け合わせる
		*GetMtxWorldPtr() *= *GetParentMtxPtr();
	}

	//メッシュ描画
	DrawMesh();
}

//------------------------------------------------------------------------------
//頂点情報を元にメッシュの描画
//------------------------------------------------------------------------------
void CSceneX::DrawMesh()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL		*pMat;
	D3DMATERIAL9		matDef;
	D3DMATERIAL9		DrawMat;

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, GetMtxWorldPtr());

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_SceneXInfo->GetMatBuff()->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_SceneXInfo->GetMatNum(); nCntMat++)
	{
		//描画用のマテリアル取得
		DrawMat = pMat[nCntMat].MatD3D;

		if (DrawMat.Diffuse.a <= 0)
		{
			continue;
		}

		// マテリアルの設定
		pDevice->SetMaterial(&DrawMat);

		// 描画
		m_SceneXInfo->GetMesh()->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//------------------------------------------------------------------------------
//ステンシル用の描画処理
//------------------------------------------------------------------------------
void CSceneX::DrawStencil()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, GetMtxWorldPtr());

	for (int nCntMat = 0; nCntMat < (int)m_SceneXInfo->GetMatNum(); nCntMat++)
	{
		// 描画
		m_SceneXInfo->GetMesh()->DrawSubset(nCntMat);
	}
}

//------------------------------------------------------------------------------
//入力された色に応じたマテリアルで描画
//------------------------------------------------------------------------------
void CSceneX::DrawMesh_SetMaterial(D3DXCOLOR col)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DMATERIAL9		MatDef;		//デフォルトのマテリアル
	D3DMATERIAL9		MatInput;	//入力された色で生成されたマテリアル

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&MatDef);

	//初期化
	MatInput = MatDef;

	//色の設定
	MatInput.Diffuse = col;
	MatInput.Emissive = col;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, GetMtxWorldPtr());

	for (int nCntMat = 0; nCntMat < (int)m_SceneXInfo->GetMatNum(); nCntMat++)
	{
		//ステンシル設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

		// マテリアルの設定
		pDevice->SetMaterial(&MatInput);

		// 描画
		m_SceneXInfo->GetMesh()->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&MatDef);

}
//------------------------------------------------------------------------------
//影のマトリックス描画
//------------------------------------------------------------------------------
void CSceneX::DrawShadow()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL		*pMat;
	D3DMATERIAL9		matDef;
	D3DMATERIAL9		matBlack;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//初期化
	matBlack = matDef;

	//黒設定
	matBlack.Diffuse = SHADOW_COLOR;

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_SceneXInfo->GetMatBuff()->GetBufferPointer();

	//ステンシル有効　0のところにのみ描画可能
	pDevice->SetRenderState(D3DRS_STENCILREF, 0);					//ステンシルの条件の値
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);		//ステンシルの条件 ==
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);				//ステンシル･バッファ有効

	//Ｚテストoff
	CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_ZTEST_OFF_ZWRITING_ON);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, GetMtxWorldPtr());


	for (int nCntMat = 0; nCntMat < (int)m_SceneXInfo->GetMatNum(); nCntMat++)
	{
		//ステンシル設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

		// マテリアルの設定
		pDevice->SetMaterial(&matBlack);

		// 描画
		m_SceneXInfo->GetMesh()->DrawSubset(nCntMat);
	}

	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);	// ステンシル･バッファ無効

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}
//------------------------------------------------------------------------------
//ハードエッジ描画用のステンシル描画
//------------------------------------------------------------------------------
void CSceneX::DrawHardEdgeStencil()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ステンシル有効　0のところにのみ描画可能
	pDevice->SetRenderState(D3DRS_STENCILREF, 0);					//ステンシルの条件の値
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);		//ステンシルの条件 ==
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);				//ステンシル･バッファ有効

	//ZWriteoff
	CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_ZTEST_ON_ZWRITING_OFF);

	//ステンシル用の描画
	CSceneX::DrawStencil();

	//Zwrite元に戻す
	CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_ZTEST_DEFAULT);

	//マトリックス計算
	CHossoLibrary::CalcMatrix(GetMtxWorldPtr(), GetPos(), GetRot(), HARDEDGE_SCALE);

	//nullcheck
	if (GetParentMtxPtr())
	{
		//親のマトリックスを掛け合わせる
		*GetMtxWorldPtr() *= *GetParentMtxPtr();
	}

	//ステンシル有効　0のところにのみ描画可能
	pDevice->SetRenderState(D3DRS_STENCILREF, 0);					//ステンシルの条件の値
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);		//ステンシルの条件 ==
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);				//ステンシル･バッファ有効

	//ライティングOFF
	//CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_LIGHTING_OFF);

	//メッシュ描画
	CSceneX::DrawMesh_SetMaterial(STENCIL_COLOR);

	//ライティングを基に戻す
	CHossoLibrary::CheckLighting();

	// ステンシル･バッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);


}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CSceneX::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}