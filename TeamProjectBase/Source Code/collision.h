//------------------------------------------------------------------------------
//
//当たり判定処理  [collision.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _COLLISION_H_
#define _COLLISION_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"

//------------------------------------------------------------------------------
//前方宣言
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CMesh;

class CCollision
{
public:
	CCollision();
	~CCollision();

	//コリジョンの種類
	enum COLLISION_TYPE
	{
		COLLISION_CHARACTER,		//キャラクター
		COLLISION_WEAPON,			//武器
		COLLISION_EXPLOSION,		//爆発
		COLLISION_OBJECT,			//オブジェクト
		COLLISION_CAMERA,			//カメラ
	};
	void ShowDebugInfo();																				//デバッグ情報表記
	static std::unique_ptr<CCollision> Create(D3DXMATRIX *pMtx, COLLISION_TYPE ColisionType);			//生成処理
	void SetCollisionInfo(float fRadius,float fLength);													//情報設定　半径と長さ
	void SetCollisionVtx(MODEL_VTX &modelVtx);															//情報設定　頂点
	bool CollisionCylinder(CCollision *pTargetCollision);												//シリンダーのコリジョン関数
	bool CollisionCharacter(CCollision *pTargetCollision,D3DXVECTOR3 *pOut);							//キャラクター同士の衝突判定　シリンダーで取る
	bool CollisionExplosion(CCollision *pTargetCollision);												//爆発用の判定
	bool CollisionObject(CCollision *pTargetCollision, D3DXVECTOR3 &rPos,D3DXVECTOR3 &rPosOld,D3DXVECTOR3 &rMove);			//オブジェクトとの衝突判定　□と□
	bool CollisionObjectToCamera(CCollision *pTargetCollision, D3DXVECTOR3 &rPos, D3DXVECTOR3 &rPosOld);					//オブジェクトとの衝突判定　カメラ用

private:
	D3DXMATRIX *m_pParentMtx;			//持ち主のマトリックス
	MODEL_VTX m_ModelVtx;				//□で判定取るときに使う　最大頂点と最小頂点
	float m_fRadius;					//半径
	float m_fLength;					//判定の長さ

#ifdef _DEBUG
	std::shared_ptr<CMesh> m_pDebugMesh;		//メッシュのポインタ 描画用
	void CreateMesh();							//メッシュ生成
#endif // _DEBUG

};
#endif