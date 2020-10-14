//------------------------------------------------------------------------------
//
//パーティクルの管理処理  [ParticleManager.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _PARTICLE_MANAGER_H_
#define _PARTICLE_MANAGER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CParticle;

//パーティクルの管理
class CParticleManager : public CScene
{
public:
	CParticleManager();									//コンストラクタ
	~CParticleManager();								//デストラクタ

	HRESULT Init();									//初期化
	void Uninit();									//終了
	void Update();									//更新
	void Draw();									//描画
	void ShowDebugInfo();							//デバッグ情報表記

	static void Create();
	static void Release();
	static void AddParticleList(std::unique_ptr<CParticle> pParticle);

private:
	static std::vector<std::unique_ptr<CParticle>> m_pParticleList;		//パーティクルの構造体のリスト
};

#endif