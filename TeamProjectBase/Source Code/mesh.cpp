//------------------------------------------------------------------------------
//
//メッシュ処理  [mesh.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "mesh.h"
#include "renderer.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "camera.h"
#include "meshfield.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
int CMesh::m_nNumMesh = 0;

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CMesh::CMesh()
{
	//初期化
	m_pos = ZeroVector3;
	m_rot = ZeroVector3;
	m_col = WhiteColor;
	m_nNumVertex = 0;
	m_nNumIdx = 0;
	m_nNumPolygon = 0;
	m_apNormalList.clear();
	m_pVtxMeshList.clear();
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_bTex = true;

	//総数加算
	m_nNumMesh++;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CMesh::~CMesh()
{
	//頂点バッファの開放
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//インデックスバッファの開放
	if (m_pIdxBuff)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	//配列の消去
	m_apNormalList.clear();
	m_pVtxMeshList.clear();

	//総数減らす
	m_nNumMesh--;
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CMesh::Init()
{
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CMesh::Uninit()
{

}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CMesh::Update()
{
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CMesh::Draw()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの設定22
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデバイスのデータストリームにバインド
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	//bTexがfalseの場合はテクスチャを張らない
	m_bTex ?
		pDevice->SetTexture(0, m_pTexture) :
		pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
									0,
									0,
									m_nNumVertex,			//使用する頂点数
									0,						//頂点の読み取りを開始する位置
									m_nNumPolygon);			//ポリゴンの枚数

}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CMesh::ShowDebugInfo()
{
#ifdef _DEBUG

	//mesh情報
	if (ImGui::TreeNode("MeshInfo"))
	{
		//座標
		if (ImGui::TreeNode("pos"))
		{
			if (ImGui::InputFloat3("pos", m_pos))
			{
				//座標設定
				SetPos(m_pos);
			}
			ImGui::TreePop();
		}
		//頂点
		if (ImGui::TreeNode("Vtx"))
		{
			for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
			{
				ImGui::InputFloat3("pos", m_pVtxMeshList[nCnt]->pos);
			}
			ImGui::TreePop();
		}
		//法線
		if (ImGui::TreeNode("Nor"))
		{
			for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
			{
				ImGui::InputFloat3("nor", m_pVtxMeshList[nCnt]->nor);
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif //DEBUG
}

//------------------------------------------------------------------------------
//色設定
//------------------------------------------------------------------------------
void CMesh::SetCol(D3DXCOLOR const & col)
{
	//色反映
	m_col = col;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//縦
	for (int nCntVertical = 0; nCntVertical < m_BlockNum.nY + 1; nCntVertical++)
	{	//横
		for (int nCntHorizon = 0; nCntHorizon < m_BlockNum.nX + 1; nCntHorizon++)
		{
			//頂点番号
			int nID = (nCntVertical * (m_BlockNum.nX + 1)) + nCntHorizon;

			//色設定
			pVtx[nID].col = m_col;
		}
	}

	//頂点データをアンロック
	m_pVtxBuff->Unlock();

}
//------------------------------------------------------------------------------
//頂点情報更新
//------------------------------------------------------------------------------
void CMesh::ResetVtx(D3DXVECTOR3 size)
{
	SetSize(size);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//縦
	for (int nCntVertical = 0; nCntVertical < m_BlockNum.nY + 1; nCntVertical++)
	{	//横
		for (int nCntHorizon = 0; nCntHorizon < m_BlockNum.nX + 1; nCntHorizon++)
		{
			//頂点番号
			int nID = (nCntVertical * (m_BlockNum.nX + 1)) + nCntHorizon;

			//頂点情報設定
			//派生クラスがオーバーライドで処理を記入
			pVtx[nID].pos = SetVtx(INTEGER2(nCntHorizon, nCntVertical), m_BlockNum);
			m_pVtxMeshList[nID]->pos = pVtx[nID].pos + m_pos;
		}
	}

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------------------------
//空用のテクスチャ設定
//------------------------------------------------------------------------------
void CMesh::SetTexSphere()
{
	D3DXVECTOR2 UVSize = D3DXVECTOR2(1.0f / m_BlockNum.nX, 1.0f / m_BlockNum.nY);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//縦
	for (int nCntVertical = 0; nCntVertical < m_BlockNum.nY + 1; nCntVertical++)
	{	//横
		for (int nCntHorizon = 0; nCntHorizon < m_BlockNum.nX + 1; nCntHorizon++)
		{
			//頂点番号
			int nID = (nCntVertical * (m_BlockNum.nX + 1)) + nCntHorizon;

			//テクスチャ座標
			pVtx[nID].tex = D3DXVECTOR2(UVSize.x * nCntHorizon, UVSize.y * nCntVertical);
		}
	}
	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------------------------
//法線設定
//------------------------------------------------------------------------------
void CMesh::SetNormal()
{
	//変数宣言
	D3DXVECTOR3 VecA = ZeroVector3;
	D3DXVECTOR3 VecB = ZeroVector3;
	D3DXVECTOR3 VecC = ZeroVector3;
	int nID = 0;
	int nCntNormal = 0;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//縦
	for (int nCntVertical = 0; nCntVertical < m_BlockNum.nY; nCntVertical++)
	{
		//横
		for (int nCntHorizon = 0; nCntHorizon < m_BlockNum.nX; nCntHorizon++, nCntNormal += 2)
		{
			//頂点番号
			int nID = (nCntVertical * (m_BlockNum.nX + 1)) + nCntHorizon;

			//ベクトル計算
			VecA = m_pVtxMeshList[nID + m_BlockNum.nX + 1]->pos - m_pVtxMeshList[nID]->pos;
			VecB = m_pVtxMeshList[nID + m_BlockNum.nX + 2]->pos - m_pVtxMeshList[nID]->pos;
			VecC = m_pVtxMeshList[nID + 1]->pos - m_pVtxMeshList[nID]->pos;

			//外積を使って面の法線を求める
			D3DXVec3Cross(m_apNormalList[nCntNormal].get(), &-VecA, &VecB);
			D3DXVec3Cross(m_apNormalList[nCntNormal + 1].get(), &-VecB, &VecC);

			//正規化
			D3DXVec3Normalize(m_apNormalList[nCntNormal].get(), m_apNormalList[nCntNormal].get());
			D3DXVec3Normalize(m_apNormalList[nCntNormal + 1].get(), m_apNormalList[nCntNormal + 1].get());
		}
	}

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	nCntNormal = 0;
	//縦
	for (int nCntVertical = 0; nCntVertical < m_BlockNum.nY + 1; nCntVertical++)
	{
		//横
		for (int nCntHorizon = 0; nCntHorizon < m_BlockNum.nX + 1; nCntHorizon++, nID++)
		{
			//右下の端のとこ
			if (nCntHorizon == m_BlockNum.nX && nCntVertical == m_BlockNum.nY)
			{
				m_pVtxMeshList[nID]->nor = pVtx[nID].nor = (*m_apNormalList[(nCntNormal * 2)] + (*m_apNormalList[(nCntNormal * 2) + 1])) / 2;
			}

			//一番右と一番下以外の時
			if (nCntHorizon != m_BlockNum.nX && nCntVertical != m_BlockNum.nY)
			{
				if (nCntVertical == 0 && nCntHorizon == 0)
				{
					//左上
					m_pVtxMeshList[nID]->nor = pVtx[nID].nor = (*m_apNormalList[0] + *m_apNormalList[1]) / 2;
				}

				//一番上の段
				else if (nCntVertical == 0)
				{
					//3面の法線を加算して割る
					m_pVtxMeshList[nID]->nor = pVtx[nID].nor = (*m_apNormalList[(nCntNormal * 2)] + *m_apNormalList[(nCntNormal * 2) + 1] + *m_apNormalList[(nCntNormal * 2) - 1]) / 3;
				}
				//一番左の列
				else if (nCntHorizon == 0)
				{
					//3面の法線を加算して割る
					m_pVtxMeshList[nID]->nor = pVtx[nID].nor = (*m_apNormalList[(nCntNormal * 2)] + *m_apNormalList[(nCntNormal * 2) + 1] + *m_apNormalList[((nCntNormal - m_BlockNum.nX) * 2)]) / 3;
				}
				//その他　(端に面してない部分
				else
				{
					m_pVtxMeshList[nID]->nor = pVtx[nID].nor =
						(	*m_apNormalList[(nCntNormal * 2)] +								//下の面
							*m_apNormalList[(nCntNormal * 2) + 1] + 						//右下の面
							*m_apNormalList[(nCntNormal * 2) - 1] +							//左下の面
							*m_apNormalList[((nCntNormal - m_BlockNum.nX) * 2)] +			//右上の面
							*m_apNormalList[((nCntNormal - m_BlockNum.nX - 1) * 2)] + 		//左上の面
							*m_apNormalList[((nCntNormal - m_BlockNum.nX - 1) * 2) + 1]) 	//上の面
							/ 6;	//を割る
				}

				//normal用のカウンタ
				nCntNormal++;

				//カウンタが超えた時
				if (nCntNormal >= (m_BlockNum.nX * m_BlockNum.nY))
				{
					//一番下の列も処理するために戻す
					nCntNormal -= m_BlockNum.nX;
				}
			}
			//右端の時
			else if (nCntHorizon == m_BlockNum.nX)
			{
				//一番上以外の時

				if (nCntVertical == 0)
				{
					m_pVtxMeshList[nID]->nor = pVtx[nID].nor = *m_apNormalList[(nCntNormal * 2) + 1];
				}
				else if (nCntVertical != m_BlockNum.nY)
				{
					m_pVtxMeshList[nID]->nor = pVtx[nID].nor = (*m_apNormalList[((nCntNormal - m_BlockNum.nX) * 2)] + *m_apNormalList[((nCntNormal - m_BlockNum.nX) * 2) + 1] + *m_apNormalList[(nCntNormal * 2) + 1]) / 3;
				}
			}
			//下端の時
			else if (nCntVertical == m_BlockNum.nY)
			{
				if (nCntHorizon == 0)
				{
					m_pVtxMeshList[nID]->nor = pVtx[nID].nor = *m_apNormalList[(nCntNormal * 2)];
				}
				else if (nCntHorizon != m_BlockNum.nX)
				{
					m_pVtxMeshList[nID]->nor = pVtx[nID].nor = (*m_apNormalList[((nCntNormal - m_BlockNum.nX - 1) * 2)] + *m_apNormalList[((nCntNormal - m_BlockNum.nX - 1) * 2) + 1] + *m_apNormalList[((nCntNormal - m_BlockNum.nX) * 2)]) / 3;
				}
				//normal用のカウンタ
				nCntNormal++;

				//カウンタが超えた時
				if (nCntNormal >= (m_BlockNum.nX * m_BlockNum.nY))
				{
					//一番下の列も処理するために戻す
					nCntNormal = m_BlockNum.nX * m_BlockNum.nY - 1;
				}
			}
		}
	}

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------------------------
//頂点生成
//------------------------------------------------------------------------------
void CMesh::MakeVertex()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ブロック数から必要な情報を引き出す
	m_nNumVertex = (m_BlockNum.nX + 1) * (m_BlockNum.nY + 1);						//頂点数
	m_nNumIdx = (m_BlockNum.nX * 2 + 2) * m_BlockNum.nY + ((m_BlockNum.nY - 1) * 2);	//インデックス数
	m_nNumPolygon = (m_BlockNum.nX * m_BlockNum.nY) * 2 + (m_BlockNum.nY - 1) * 4;	//ポリゴン数

	//メモリ確保
	//頂点情報
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{
		//メモリ確保
		std::unique_ptr<VERTEX_3D> vertex(new VERTEX_3D);
		m_pVtxMeshList.emplace_back(std::move(vertex));
	}

	//法線情報
	for (int nCnt = 0; nCnt < (m_BlockNum.nX * m_BlockNum.nY) * 2; nCnt++)
	{
		//メモリ確保
		std::unique_ptr<D3DXVECTOR3> normal(new D3DXVECTOR3);
		m_apNormalList.emplace_back(std::move(normal));
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックス情報の作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;
	//インデックスデータへのポインタ
	WORD * pIdx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fHeight = 0.0f;

	//縦
	for (int nCntVertical = 0; nCntVertical < m_BlockNum.nY + 1; nCntVertical++)
	{	//横
		for (int nCntHorizon = 0; nCntHorizon < m_BlockNum.nX + 1; nCntHorizon++)
		{
			//頂点番号
			int nID = (nCntVertical * (m_BlockNum.nX + 1)) + nCntHorizon;

			//頂点情報設定
			//派生クラスがオーバーライドで処理を記入
			pVtx[nID].pos = SetVtx(INTEGER2(nCntHorizon,nCntVertical),m_BlockNum);
			m_pVtxMeshList[nID]->pos = pVtx[nID].pos + m_pos;

			//法線は初期化　あとで計算
			pVtx[nID].nor = ZeroVector3;

			//初期化
			pVtx[nID].col = WhiteColor;

			//一定間隔
			pVtx[nID].tex = D3DXVECTOR2((float)nCntHorizon, (float)nCntVertical);		//(テクスチャのUV座標)

		}
	}

	//頂点データをアンロック
	m_pVtxBuff->Unlock();

	//インデックスバッファのロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//インデックス設定
	//縦
	for (int nCntVertical = 0; nCntVertical < m_BlockNum.nY; nCntVertical++)
	{	//横
		for (int nCntHorizon = 0; nCntHorizon < m_BlockNum.nX + 1; nCntHorizon++)
		{
			//インデックス設定
			pIdx[0] = (m_BlockNum.nX + 1) + nCntHorizon + nCntVertical * (m_BlockNum.nX + 1);
			pIdx[1] = nCntHorizon + nCntVertical * (m_BlockNum.nX + 1);

			//ポインタ進める
			pIdx += 2;

			//縮退ポリゴンの分
			if ((nCntHorizon + 1) % (m_BlockNum.nX + 1) == 0 && nCntVertical < m_BlockNum.nY - 1)
			{
				pIdx[0] = nCntHorizon + nCntVertical * (m_BlockNum.nX + 1);
				pIdx[1] = (m_BlockNum.nX + 1) + nCntHorizon + nCntVertical * (m_BlockNum.nX + 1) + 1;

				//ポインタ進める
				pIdx += 2;
			}
		}
	}

	//インデックスのバッファのアンロック
	m_pIdxBuff->Unlock();

	//法線設定
	SetNormal();
}
