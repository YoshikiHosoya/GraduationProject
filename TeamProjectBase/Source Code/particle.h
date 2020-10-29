//------------------------------------------------------------------------------
//
//パーティクル処理  [particle.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "ParticleParamater.h"
#include "TexAnimationBase.h"
#include "GameObject.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define MAX_PARTICLE (100000)									//パーティクルの最大数
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CParticleParam;
//パーティクルの粒子
class COneParticle
{
	public:
		COneParticle() { nNumParticleAll++; };		//コンストラクタ
		~COneParticle() { nNumParticleAll--; };		//デストラクタ

	//生成関数
	static std::unique_ptr<COneParticle> Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot)
	{
		//メモリ確保
		std::unique_ptr<COneParticle> pOneParticle(new COneParticle);

		//情報代入
		pOneParticle->m_pos = pos;
		pOneParticle->m_move = move;
		pOneParticle->m_rot = rot;

		//return
		return pOneParticle;
	}
	static int GetNumAll() {return nNumParticleAll;};		//総数取得

	D3DXVECTOR3 m_pos;				//座標
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_rot;				//回転量
	D3DXMATRIX m_Mtx;				//マトリックス
	static int nNumParticleAll;		//総数
};

//パーティクルのまとまり
//テクスチャアニメーション継承
class CParticle : public CTexAnimationBase, public CGameObject
{
public:
	CParticle();									//コンストラクタ
	~CParticle();									//デストラクタ

	HRESULT Init();									//初期化
	void Uninit();									//終了
	void Update();									//更新
	void Draw();									//描画
	void ShowDebugInfo();							//デバッグ情報表記
	void UpdateVertex();							//頂点の更新
	static void ResetVertex();						//頂点リセット
	static HRESULT MakeVertex();					//頂点生成
	static void ReleaseVertex();					//頂点破棄
	static void ResetVertexID();					//頂点IDリセット　画面が停止してもパーティクルの処理を行う為

	static void CreateFromParam(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CParticleParam *pInputParam);
	static void CreateFromText(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CParticleParam::PARTICLE_TYPE type, D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, -1.0f), D3DXVECTOR3 *PosPtr = nullptr, TAG tag = TAG::PLAYER_1, int nAttack = -1);

	bool GetDeleteFlag() { return m_bDeleteFlag; };
	void CalcCollisionSize(D3DXVECTOR3 size);						//コリジョンのサイズ計算

private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;						//頂点バッファへのポインタ
	static int m_nVertexID;											//頂点ID

	D3DXVECTOR3 m_posOrigin;										//原点座標
	D3DXVECTOR3 m_posEndPoint;										//終点座標
	D3DXVECTOR3 m_rotOrigin;										//原点回転量
	D3DXVECTOR3 m_CollisionOrigin;									//当たり判定用の原点
	D3DXMATRIX m_WorldMtx;											//ワールドマトリックス
	D3DXVECTOR3 *m_pPosOriginPtr;									//原点座標のポインタ 追従する時はこれを使う

	//CCollision *m_pCollision;										//コリジョンのポインタ
	U_ptr<CParticleParam> m_pParticleParam;				//パーティクルの現在のパラメータのポインタ

	Vec<U_ptr<COneParticle>> m_pParticleList;		//パーティクルの構造体のリスト
	bool m_bDeleteFlag;												//消去フラグ

	void SetParticle(D3DXVECTOR3 &pos, D3DXVECTOR3 const &rot, CParticleParam *pParam);				//パーティクル設定
	void SetCollsionParam();										//当たり判定のパラメータ設定
	void SetAnimationParam();										//アニメーションのパラメータ設定
	void Collision();												//判定
	void RendererSetting();											//描画の設定
	void SetSound();												//エフェクトに応じた音再生
};

#endif
