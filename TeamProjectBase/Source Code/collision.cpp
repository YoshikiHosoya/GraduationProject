//------------------------------------------------------------------------------
//
//当たり判定処理  [collision.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "collision.h"
#include "manager.h"
#include "renderer.h"
#include "meshcylinder.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CCollision::CCollision()
{
	//初期化
	m_ModelVtx.VtxMax = ZeroVector3;
	m_ModelVtx.VtxMin = ZeroVector3;
	m_fRadius = 0.0f;
	m_fLength = 0.0f;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CCollision::~CCollision()
{
#ifdef  _DEBUG

	if (m_pDebugMesh)
	{
		m_pDebugMesh->Release();
		m_pDebugMesh.reset();
	}
#endif //  _DEBUG
}

//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CCollision::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}
//------------------------------------------------------------------------------
//生成処理
//------------------------------------------------------------------------------
std::unique_ptr<CCollision> CCollision::Create(D3DXMATRIX * pMtx, COLLISION_TYPE ColisionType)
{
	//変数宣言
	std::unique_ptr<CCollision> pCollision(new CCollision);

	//nullcheck
	if (pCollision)
	{
		//元のマトリックス取得
		pCollision->m_pParentMtx = pMtx;

		switch (ColisionType)
		{
		case CCollision::COLLISION_CHARACTER:
		case CCollision::COLLISION_WEAPON:
#ifdef _DEBUG
			//メッシュ生成
			pCollision->CreateMesh();
#endif // _DEBUG
			break;
		}

		//生成した情報
		return pCollision;
	}
	return nullptr;
}

//------------------------------------------------------------------------------
//情報設定
//------------------------------------------------------------------------------
void CCollision::SetCollisionInfo(float fRadius, float fLength)
{
	//情報代入
	m_fRadius = fRadius;
	m_fLength = fLength;

	//最少頂点と最大頂点設定
	m_ModelVtx.VtxMax = D3DXVECTOR3(fRadius, fLength, fRadius);
	m_ModelVtx.VtxMin = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);

#ifdef  _DEBUG

	if (m_pDebugMesh)
	{
		//頂点情報再設定
		m_pDebugMesh->ResetVtx(D3DXVECTOR3(m_fRadius, m_fLength * 0.5f, m_fRadius));
	}
#endif //_DEBUG
}
//------------------------------------------------------------------------------
//情報設定
//------------------------------------------------------------------------------
void CCollision::SetCollisionVtx(MODEL_VTX & modelVtx)
{
	m_ModelVtx = modelVtx;
}
//------------------------------------------------------------------------------
//シリンダーの当たり判定処理
//------------------------------------------------------------------------------
bool CCollision::CollisionCylinder(CCollision * pTargetCollision)
{
	//半径の和
	float fSumRadius = m_fRadius + pTargetCollision->m_fRadius;

	//それぞれの始点と終点,始点から終点へのベクトル
	D3DXVECTOR3 posStart1, posStart2, posEnd1, posEnd2, vecStart_End1, vecStart_End2;

	//始点
	posStart1 = D3DXVECTOR3(m_pParentMtx->_41, m_pParentMtx->_42, m_pParentMtx->_43);
	posStart2 = D3DXVECTOR3(pTargetCollision->m_pParentMtx->_41, pTargetCollision->m_pParentMtx->_42, pTargetCollision->m_pParentMtx->_43);

	//終点
	D3DXVec3TransformCoord(&posEnd1, &D3DXVECTOR3(0.0f, m_fLength, 0.0f), m_pParentMtx);
	D3DXVec3TransformCoord(&posEnd2, &D3DXVECTOR3(0.0f, pTargetCollision->m_fLength, 0.0f), pTargetCollision->m_pParentMtx);

	//それぞれの始点から終点へのベクトル
	vecStart_End1 = posEnd1 - posStart1;
	vecStart_End2 = posEnd2 - posStart2;

	//正規化
	D3DXVec3Normalize(&vecStart_End1, &vecStart_End1);
	D3DXVec3Normalize(&vecStart_End2, &vecStart_End2);

	//法線
	D3DXVECTOR3 nor;
	D3DXVec3Cross(&nor, &vecStart_End1, &vecStart_End2);

	//正規化
	D3DXVec3Normalize(&nor, &nor);

	//それぞれの始点から始点へのベクトル
	D3DXVECTOR3 vec1_2 = posStart2 - posStart1;
	D3DXVECTOR3 vec2_1 = posStart1 - posStart2;

	//最短距離
	float fCollisionLength = fabsf(D3DXVec3Dot(&nor, &vec1_2));

	//始点から終点　と　始点から始点　の内積
	float fDotDirection = D3DXVec3Dot(&vecStart_End1, &vec1_2);

	//X軸とZ軸のみでみた時の距離　それぞれの始点
	float f2DDirection = D3DXVec2Length(&D3DXVECTOR2(vec1_2.x, vec1_2.z));

	//Y軸で見た時　対象のシリンダーの間に自分の始点か終点が入っていた時
	if ((posStart2.y <= posStart1.y && posEnd2.y >= posStart1.y) || (posStart2.y <= posEnd1.y && posEnd2.y >= posEnd1.y))
	{
		//半径の和と最短距離を比較 && 内積と半径の和が０以上 && X軸とZ軸でみた時の距離が剣の長さよりも短い場合
		if (fCollisionLength < fSumRadius && fDotDirection + pTargetCollision->m_fRadius > 0 && f2DDirection < m_fLength)
		{
#ifdef _DEBUG
			//HIT
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "HIT\n");

			//メッシュの色
			m_pDebugMesh->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f));

#endif //_DEBUG
			return true;

		}
	}
#ifdef _DEBUG

	//当たってない時は通常色
	m_pDebugMesh->SetCol(D3DXCOLOR(0.0f, 0.3f, 1.0f, 0.3f));
#endif //_DEBUG

	return false;

}

//------------------------------------------------------------------------------
//キャラクター同士の判定 XとZの値のみ使う
//------------------------------------------------------------------------------
bool CCollision::CollisionCharacter(CCollision * pTargetCollision, D3DXVECTOR3 *pOut)
{
	//座標取得
	//参照で貰う
	D3DXVECTOR2 &rMyPos = D3DXVECTOR2(m_pParentMtx->_41, m_pParentMtx->_43);
	D3DXVECTOR2 &rTargetPos = D3DXVECTOR2(pTargetCollision->m_pParentMtx->_41, pTargetCollision->m_pParentMtx->_43);

	//自分からターゲットに向かうベクトル
	D3DXVECTOR2 Vec_My_Target = rTargetPos - rMyPos;

	//自分からターゲットまでの距離
	float fLength = D3DXVec2Length(&Vec_My_Target);

	//半径の和
	float fSumRadius = (m_fRadius * 1.2f) + pTargetCollision->m_fRadius;

	//半径の和と距離を測る
	//半径の和より距離が短かった場合
	if (fSumRadius > fLength)
	{
		//角度
		float fAngle = atan2f(Vec_My_Target.x, Vec_My_Target.y);

		//移動後の座標設定
		pOut->x = rMyPos.x + sinf(fAngle) * fSumRadius;
		pOut->z = rMyPos.y + cosf(fAngle) * fSumRadius;

		//true
		return true;
	}

	//false
	return false;
}
//------------------------------------------------------------------------------
//爆発用の判定　半径を使った判定
//------------------------------------------------------------------------------
bool CCollision::CollisionExplosion(CCollision * pTargetCollision)
{
	//座標取得
	//参照で貰う
	D3DXVECTOR2 &rMyPos = D3DXVECTOR2(m_pParentMtx->_41, m_pParentMtx->_43);
	D3DXVECTOR2 &rTargetPos = D3DXVECTOR2(pTargetCollision->m_pParentMtx->_41, pTargetCollision->m_pParentMtx->_43);

	//自分からターゲットに向かうベクトル
	D3DXVECTOR2 Vec_My_Target = rTargetPos - rMyPos;

	//自分からターゲットまでの距離
	float fLength = fabsf(D3DXVec2Length(&Vec_My_Target));

	//半径の和
	float fSumRadius = m_fRadius + pTargetCollision->m_fRadius;

	//半径の和と距離を測る
	//半径の和より距離が短かった場合
	if (fSumRadius > (fLength))
	{
		return true;
	}
	return false;

}

//-----------------------------------------------------------------------------
// モデルの当たり判定処理
//-----------------------------------------------------------------------------
bool CCollision::CollisionObject(CCollision * pTargetCollision, D3DXVECTOR3 &rPos, D3DXVECTOR3 &rPosOld, D3DXVECTOR3 & rMove)
{
	//参照で貰う
	D3DXVECTOR3 &rMyPos = rPos;
	D3DXVECTOR3 &rTargetPos = D3DXVECTOR3(pTargetCollision->m_pParentMtx->_41, pTargetCollision->m_pParentMtx->_42, pTargetCollision->m_pParentMtx->_43);

	//上にのったか
	bool bLand = false;

	//わかりやすくするため
	MODEL_VTX &rObjectVtx = pTargetCollision->m_ModelVtx;

	//ブロックの幅の中かどうか
	if (rTargetPos.x + rObjectVtx.VtxMax.x > rMyPos.x + m_ModelVtx.VtxMin.x &&
		rTargetPos.x + rObjectVtx.VtxMin.x < rMyPos.x + m_ModelVtx.VtxMax.x &&
		rTargetPos.y + rObjectVtx.VtxMax.y > rMyPos.y &&
		rTargetPos.y + rObjectVtx.VtxMin.y < rMyPos.y + m_ModelVtx.VtxMax.y)
	{
		//ブロックの奥側
		if (rTargetPos.z + rObjectVtx.VtxMax.z <= rPosOld.z + m_ModelVtx.VtxMin.z &&
			rTargetPos.z + rObjectVtx.VtxMax.z > rMyPos.z + m_ModelVtx.VtxMin.z)
		{
			rMyPos.z = rTargetPos.z + rObjectVtx.VtxMax.z + m_ModelVtx.VtxMax.z;
			rMove.z = 0.0f;
		}

		//ブロックの手前
		else if (rTargetPos.z + rObjectVtx.VtxMin.z >= rPosOld.z + m_ModelVtx.VtxMax.z &&
				rTargetPos.z + rObjectVtx.VtxMin.z < rMyPos.z + m_ModelVtx.VtxMax.z)
		{
			rMyPos.z = rTargetPos.z + rObjectVtx.VtxMin.z + m_ModelVtx.VtxMin.z;
			rMove.z = 0.0f;
		}
	}
	if (rTargetPos.z +rObjectVtx.VtxMax.z > rMyPos.z + m_ModelVtx.VtxMin.z &&
		rTargetPos.z +rObjectVtx.VtxMin.z < rMyPos.z + m_ModelVtx.VtxMax.z &&
		rTargetPos.y +rObjectVtx.VtxMax.y > rMyPos.y &&
		rTargetPos.y +rObjectVtx.VtxMin.y < rMyPos.y + m_ModelVtx.VtxMax.y)
	{
		//左側にぶつかる
		if (rTargetPos.x +rObjectVtx.VtxMax.x <= rPosOld.x + m_ModelVtx.VtxMin.x &&
			rTargetPos.x +rObjectVtx.VtxMax.x > rMyPos.x + m_ModelVtx.VtxMin.x)
		{
			rMyPos.x = rTargetPos.x + rObjectVtx.VtxMax.x + m_ModelVtx.VtxMax.x;
			rMove.x = 0.0f;
		}
		//右側にぶつかる
		else if (rTargetPos.x + rObjectVtx.VtxMin.x >= rPosOld.x + m_ModelVtx.VtxMax.x &&
				 rTargetPos.x + rObjectVtx.VtxMin.x < rMyPos.x + m_ModelVtx.VtxMax.x)
		{
			rMyPos.x = rTargetPos.x + rObjectVtx.VtxMin.x + m_ModelVtx.VtxMin.x;
			rMove.x = 0.0f;
		}
	}

	//上に乗る
	if (rTargetPos.x + rObjectVtx.VtxMax.x > rMyPos.x + m_ModelVtx.VtxMin.x &&
		rTargetPos.x + rObjectVtx.VtxMin.x < rMyPos.x + m_ModelVtx.VtxMax.x &&
		rTargetPos.z + rObjectVtx.VtxMax.z> rMyPos.z + m_ModelVtx.VtxMin.z &&
		rTargetPos.z + rObjectVtx.VtxMin.z < rMyPos.z + m_ModelVtx.VtxMax.z)
	{
		//上に乗る
		if (rTargetPos.y + rObjectVtx.VtxMax.y <= rPosOld.y && rTargetPos.y + rObjectVtx.VtxMax.y > rMyPos.y)
		{

			rMyPos.y = rTargetPos.y + rObjectVtx.VtxMax.y;
			rMove.y = 0.0f;

			bLand = true;
		}
	}
	return bLand;

}
//-----------------------------------------------------------------------------
//カメラと壁との判定
//-----------------------------------------------------------------------------
bool CCollision::CollisionObjectToCamera(CCollision * pTargetCollision, D3DXVECTOR3 & rPos, D3DXVECTOR3 & rPosOld)
{
	//参照で貰う
	D3DXVECTOR3 &rMyPos = rPos;
	D3DXVECTOR3 &rTargetPos = D3DXVECTOR3(pTargetCollision->m_pParentMtx->_41, pTargetCollision->m_pParentMtx->_42, pTargetCollision->m_pParentMtx->_43);

	//上にのったか
	bool bHit = false;

	//わかりやすくするため
	MODEL_VTX &rObjectVtx = pTargetCollision->m_ModelVtx;

	//ブロックの幅の中かどうか
	if (rTargetPos.x + rObjectVtx.VtxMax.x > rMyPos.x + m_ModelVtx.VtxMin.x &&
		rTargetPos.x + rObjectVtx.VtxMin.x < rMyPos.x + m_ModelVtx.VtxMax.x &&
		rTargetPos.y + rObjectVtx.VtxMax.y * 15> rMyPos.y &&
		rTargetPos.y + rObjectVtx.VtxMin.y < rMyPos.y + m_ModelVtx.VtxMax.y)
	{
		//ブロックの奥側
		if (rTargetPos.z + rObjectVtx.VtxMax.z <= rPosOld.z + m_ModelVtx.VtxMin.z &&
			rTargetPos.z + rObjectVtx.VtxMax.z > rMyPos.z + m_ModelVtx.VtxMin.z)
		{
			rMyPos.z = rTargetPos.z + rObjectVtx.VtxMax.z + m_ModelVtx.VtxMax.z;
			bHit = true;
		}

		//ブロックの手前
		else if (rTargetPos.z + rObjectVtx.VtxMin.z >= rPosOld.z + m_ModelVtx.VtxMax.z &&
			rTargetPos.z + rObjectVtx.VtxMin.z < rMyPos.z + m_ModelVtx.VtxMax.z)
		{
			rMyPos.z = rTargetPos.z + rObjectVtx.VtxMin.z + m_ModelVtx.VtxMin.z;
			bHit = true;
		}
	}
	if (rTargetPos.z + rObjectVtx.VtxMax.z > rMyPos.z + m_ModelVtx.VtxMin.z &&
		rTargetPos.z + rObjectVtx.VtxMin.z < rMyPos.z + m_ModelVtx.VtxMax.z &&
		rTargetPos.y + rObjectVtx.VtxMax.y * 15 > rMyPos.y &&
		rTargetPos.y + rObjectVtx.VtxMin.y < rMyPos.y + m_ModelVtx.VtxMax.y)
	{
		//左側にぶつかる
		if (rTargetPos.x + rObjectVtx.VtxMax.x <= rPosOld.x + m_ModelVtx.VtxMin.x &&
			rTargetPos.x + rObjectVtx.VtxMax.x > rMyPos.x + m_ModelVtx.VtxMin.x)
		{
			rMyPos.x = rTargetPos.x + rObjectVtx.VtxMax.x + m_ModelVtx.VtxMax.x;
			bHit = true;
		}
		//右側にぶつかる
		else if (rTargetPos.x + rObjectVtx.VtxMin.x >= rPosOld.x + m_ModelVtx.VtxMax.x &&
			rTargetPos.x + rObjectVtx.VtxMin.x < rMyPos.x + m_ModelVtx.VtxMax.x)
		{
			rMyPos.x = rTargetPos.x + rObjectVtx.VtxMin.x + m_ModelVtx.VtxMin.x;
			bHit = true;
		}
	}
	return bHit;
}

#ifdef  _DEBUG
//------------------------------------------------------------------------------
//シリンダー生成
//------------------------------------------------------------------------------
void CCollision::CreateMesh()
{
	//メッシュ生成
	m_pDebugMesh = CMeshCylinder::Create(ZeroVector3, D3DXVECTOR3(m_fRadius, m_fLength * 0.5f, m_fRadius), { 10,2 }, m_pParentMtx);

	//nullcheck
	if (m_pDebugMesh)
	{
		//パラメータ設定
		m_pDebugMesh->SetCol(D3DXCOLOR(0.0f, 0.3f, 1.0f, 0.3f));
		m_pDebugMesh->SetTex(false);
	}

}
#endif //_DEBUG