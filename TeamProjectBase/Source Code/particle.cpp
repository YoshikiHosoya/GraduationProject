//------------------------------------------------------------------------------
//
//パーティクル処理  [praticle.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "ParticleManager.h"
//#include "collision.h"
#include "sound.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 CParticle::m_pVtxBuff = nullptr;
int CParticle::m_nVertexID = 0;
int COneParticle::nNumParticleAll = 0;

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CParticle::CParticle()
{
	//初期化
	m_bDeleteFlag = false;
	m_posOrigin = ZeroVector3;
	m_posEndPoint = ZeroVector3;
	m_rotOrigin = ZeroVector3;
	m_CollisionOrigin = ZeroVector3;
	m_pPosOriginPtr = nullptr;
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CParticle::~CParticle()
{
	//リストの開放
	m_pParticleList.clear();

	////nullcheck
	//if (m_pCollision)
	//{
	//	//コリジョンの開放
	//	delete m_pCollision;
	//	m_pCollision = nullptr;
	//}
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CParticle::Init()
{
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CParticle::Uninit()
{

}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CParticle::Update()
{
	//ライフを減らす
	m_pParticleParam->UpdateParam();

	////当たり判定
	//if (m_pParticleParam->GetCollision())
	//{
	//	//nullcheck
	//	if (m_pCollision)
	//	{
	//		//当たり判定処理
	//		Collision();

	//		//カウントダウン
	//		m_pParticleParam->GetCollisionCnt()--;

	//		//当たり判定のカウントが0になったら消す
	//		if (m_pParticleParam->GetCollisionCnt() <= 0)
	//		{
	//			//メモリ開放
	//			delete m_pCollision;
	//			m_pCollision = nullptr;
	//		}
	//	}
	//}

	//アニメーションの処理をする場合
	if (m_pParticleParam->GetAnimation())
	{
		//アニメーション更新処理
		UpdateAnimation();
	}

	//ライフが0以下になった時かアニメーションが終了した時
	if (m_pParticleParam->GetLife() <= 0 || CTexAnimationBase::GetEndFlag())
	{
		//消す
		m_bDeleteFlag = true;
		return;
	}
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CParticle::Draw()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//描画する為の設定
	RendererSetting();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	//アニメーションするかどうかで変化
	m_pParticleParam->GetAnimation() ?
		pDevice->SetTexture(0, CTexture::GetSeparateTexture(m_pParticleParam->GetSeparateTex())) :
		pDevice->SetTexture(0, CTexture::GetTexture(m_pParticleParam->GetTex()));

	//ワールドマトリックス計算
	CHossoLibrary::CalcMatrix(&m_WorldMtx, m_posOrigin, ZeroVector3,OneVector3);

	//パーティクルのリストの数分

	for (size_t nCnt = 0; nCnt < m_pParticleList.size(); nCnt++)
	{
		//マトリック計算
		CHossoLibrary::CalcMatrix(&m_pParticleList[nCnt]->m_Mtx, m_pParticleList[nCnt]->m_pos, m_pParticleList[nCnt]->m_rot,OneVector3);

		//原点のマトリックスを掛け合わせる
		m_pParticleList[nCnt]->m_Mtx *= m_WorldMtx;

		if (m_pParticleParam->GetBillboard())
		{
			//ビルボード設定
			CHossoLibrary::SetBillboard(&m_pParticleList[nCnt]->m_Mtx);
		}


		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_pParticleList[nCnt]->m_Mtx);

		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			(4 * m_nVertexID) + nCnt * 4,					//開始するインデックス(頂点) (前回までの分で使った頂点) + 現在使う分
			2);												//ポリゴンの枚数
	}

	//頂点ID加算
	m_nVertexID += m_pParticleList.size();

	//Zテスト通常
	CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_ZTEST_DEFAULT);

	//通常合成
	CManager::GetRenderer()->SetRendererCommand(CRenderer::REDNERER_ALPHABLEND_DEFAULT);

	//ライティングON
	CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_LIGHTING_ON);

}

//------------------------------------------------------------------------------
//デバッグ情報表示
//------------------------------------------------------------------------------
void CParticle::ShowDebugInfo()
{

#ifdef _DEBUG

#endif // _DEBUG
}

//------------------------------------------------------------------------------
//頂点更新
//------------------------------------------------------------------------------
void CParticle::UpdateVertex()
{
	//nullcheck
	//原点のポインタのnullcheck
	if (m_pPosOriginPtr)
	{
		//コリジョンのサイズに調整する時
		if (m_pParticleParam->GetCollisionSizeCalc())
		{
			//マトリックス計算
			CHossoLibrary::CalcMatrix(&m_WorldMtx, *m_pPosOriginPtr, m_rotOrigin, OneVector3);

			//当たり判定用の原点作成　ちょっとキャラクター側とかに寄せる
			D3DXVec3TransformCoord(&m_posOrigin, &ZeroVector3, &m_WorldMtx);

		}
		//追従する時
		else if(m_pParticleParam->GetFollowing())
		{
			m_posOrigin = *m_pPosOriginPtr;
		}
	}

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点ID分進める
	pVtx += (4 * m_nVertexID);

	for (size_t nCnt = 0; nCnt < m_pParticleList.size(); nCnt++)
	{
		//重力をかける場合
		if (m_pParticleParam->GetGravity())
		{
			//↓方向に力を加算
			m_pParticleList[nCnt]->m_move.y -= m_pParticleParam->GetGravityPower();
		}
		//移動
		m_pParticleList[nCnt]->m_pos += m_pParticleList[nCnt]->m_move;

		//中心が原点じゃない場合
		if (m_pParticleParam->GetCollisionSizeCalc())
		{
			//頂点の座標
			pVtx[0].pos = D3DXVECTOR3(-m_pParticleParam->GetSize().x, +m_pParticleParam->GetSize().y * 2.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+m_pParticleParam->GetSize().x, +m_pParticleParam->GetSize().y * 2.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-m_pParticleParam->GetSize().x, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+m_pParticleParam->GetSize().x, 0.0f, 0.0f);
		}
		else
		{
			//頂点の座標
			pVtx[0].pos = D3DXVECTOR3(-m_pParticleParam->GetSize().x, m_pParticleParam->GetSize().y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(m_pParticleParam->GetSize().x, m_pParticleParam->GetSize().y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-m_pParticleParam->GetSize().x, -m_pParticleParam->GetSize().y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(m_pParticleParam->GetSize().x, -m_pParticleParam->GetSize().y, 0.0f);
		}
		//頂点の座標
		pVtx[0].col = m_pParticleParam->GetCol();
		pVtx[1].col = m_pParticleParam->GetCol();
		pVtx[2].col = m_pParticleParam->GetCol();
		pVtx[3].col = m_pParticleParam->GetCol();

		//アニメーションする時はUV計算
		if (m_pParticleParam->GetAnimation())
		{
			D3DXVECTOR2 UV = CTexAnimationBase::CalcUV();
			D3DXVECTOR2 UVsize = CTexture::GetSparateTex_UVSize(CTexAnimationBase::GetEffectTex());

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(UV.x, UV.y);
			pVtx[1].tex = D3DXVECTOR2(UV.x + UVsize.x, UV.y);
			pVtx[2].tex = D3DXVECTOR2(UV.x, UV.y + UVsize.y);
			pVtx[3].tex = D3DXVECTOR2(UV.x + UVsize.x, UV.y + UVsize.y);
		}
		//アニメーションしてない時は通常
		else
		{
			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}


		pVtx += 4;
	}
	//頂点データをアンロック
	m_pVtxBuff->Unlock();

	//頂点ID加算
	m_nVertexID += m_pParticleList.size();
}

//------------------------------------------------------------------------------
//生成
//パーティクルのクラスを用いて生成
//------------------------------------------------------------------------------
void CParticle::CreateFromParam(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CParticleParam *pInputParam)
{
	//メモリ確保
	std::unique_ptr<CParticle> pParticle(new CParticle);

	//nullcheck
	if (pParticle)
	{
		//初期化
		pParticle->Init();

		//パーティクルのパラメータのメモリ確保
		CParticleParam *pParam = new CParticleParam;

		//nullcheck
		if (pParam)
		{
			//情報設定
			//Uniqueptrを使うとオペレータできなかったから普通のポインタ同士でオペレータ
			*pParam = pInputParam;

			//メンバのポインタに格納
			pParticle->m_pParticleParam.reset(std::move(pParam));

			//パーティクルの設定
			pParticle->SetParticle(pos, rot, pParam);

			//オブジェタイプ設定してSceneに所有権を渡す
			CParticleManager::AddParticleList(std::move(pParticle));
		}
	}
}
//------------------------------------------------------------------------------
//テクスチャ破棄
//------------------------------------------------------------------------------
void CParticle::ReleaseVertex()
{
	//nullcheck
	if (m_pVtxBuff)
	{
		//バッファの開放
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//------------------------------------------------------------------------------
//頂点IDリセット
//------------------------------------------------------------------------------
void CParticle::ResetVertexID()
{
	m_nVertexID = 0;
}

//------------------------------------------------------------------------------
//テキスト情報を元にパーティクル作成
//------------------------------------------------------------------------------
void CParticle::CreateFromText(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CParticleParam::PARTICLE_TYPE type, D3DXCOLOR col, D3DXVECTOR3 * PosPtr, TAG tag, int nAttack)
{
	//メモリ確保
	std::unique_ptr<CParticle> pParticle(new CParticle);

	//nullcheck
	if (pParticle)
	{
		//初期化
		pParticle->Init();
		pParticle->SetTag(tag);

		//パーティクルのパラメータのメモリ確保
		CParticleParam *pParam = new CParticleParam;

		//nullcheck
		if (pParam)
		{
			//情報設定
			//Uniqueptrを使うとオペレータできなかったから普通のポインタ同士でオペレータ
			*pParam = *CParticleParam::GetDefaultParam(type);

			//攻撃力が入力されていた場合
			if (nAttack > 0)
			{
				pParam->GetCollisionAttackValue() = nAttack;
			}

			//メンバのポインタに格納
			pParticle->m_pParticleParam.reset(std::move(pParam));

			//原点のポインタ
			pParticle->m_pPosOriginPtr = PosPtr;

			//パーティクルの設定
			pParticle->SetParticle(pos, rot, pParam);

			//a値が初期値じゃないとき
			if (col.a > 0)
			{
				//色設定
				pParticle->m_pParticleParam->GetCol() = col;
			}

			//ParticleManagerに所有権を渡す
			CParticleManager::AddParticleList(std::move(pParticle));
		}
	}
}
//------------------------------------------------------------------------------
//コリジョンのサイズ計算
//------------------------------------------------------------------------------
void CParticle::CalcCollisionSize(D3DXVECTOR3 size)
{
	D3DXMATRIX RotationMatrix;

	//マトリックス計算
	CHossoLibrary::CalcMatrix(&RotationMatrix, m_posOrigin, m_rotOrigin, OneVector3);

	//頂点座標
	D3DXVECTOR3 VtxPos[4] = {};

	//頂点座標算出
	D3DXVec3TransformCoord(&VtxPos[0], &D3DXVECTOR3(-size.x, +size.y * 2.0f, 0.0f), &RotationMatrix);
	D3DXVec3TransformCoord(&VtxPos[1], &D3DXVECTOR3(+size.x, +size.y * 2.0f, 0.0f), &RotationMatrix);
	D3DXVec3TransformCoord(&VtxPos[2], &D3DXVECTOR3(-size.x, 0.0f, 0.0f), &RotationMatrix);
	D3DXVec3TransformCoord(&VtxPos[3], &D3DXVECTOR3(+size.x, 0.0f, 0.0f), &RotationMatrix);

	//終点座標計算
	D3DXVec3TransformCoord(&m_posEndPoint, &D3DXVECTOR3(0.0f, +size.y * 2.0f, 0.0f), &RotationMatrix);

	//当たり判定用のローカル座標計算
	D3DXVec3TransformCoord(&m_CollisionOrigin, &D3DXVECTOR3(0.0f, m_pParticleParam->GetCollisionSize().y, 0.0f), &RotationMatrix);

	//計算用の変数　当たり判定のサイズ
	D3DXVECTOR3 Max = D3DXVECTOR3(-1000.0f, -1000.0f, -0.0f);

	//4頂点分
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//頂点情報を取得
		D3DXVECTOR3 vtx = VtxPos[nCnt];
		//最大の頂点座標を比較して出す
		if (Max.x < vtx.x)
		{
			Max.x = vtx.x;
		}
		if (Max.y < vtx.y)
		{
			Max.y = vtx.y;
		}
	}

	////当たり判定の設定
	//m_pCollision->SetPos(&m_CollisionOrigin);
	//m_pCollision->SetSize(D3DXVECTOR3(fabsf(m_CollisionOrigin.x - Max.x), fabsf(m_CollisionOrigin.y - Max.y), 0.0f) * 2.0f);
}
//------------------------------------------------------------------------------
//頂点バッファ確保
//------------------------------------------------------------------------------
HRESULT CParticle::MakeVertex()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_PARTICLE * 4,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	//頂点情報初期化
	ResetVertex();

	return S_OK;
}
//------------------------------------------------------------------------------
//頂点情報初期化
//------------------------------------------------------------------------------
void CParticle::ResetVertex()
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点の同次座標
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点の色
		pVtx[0].col = WhiteColor;
		pVtx[1].col = WhiteColor;
		pVtx[2].col = WhiteColor;
		pVtx[3].col = WhiteColor;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}
//------------------------------------------------------------------------------
//パーティクル設定
//------------------------------------------------------------------------------
void CParticle::SetParticle(D3DXVECTOR3 & pos, D3DXVECTOR3 const & rot, CParticleParam * pParam)
{
	//変数宣言
	float fAngleX, fAngleY;
	D3DXVECTOR3 move;
	float fSpeed;

	//ローカル座標加算
	pos += pParam->GetLocalPos();

	//原点に保存
	m_CollisionOrigin = m_posOrigin = pos;
	m_rotOrigin = rot;

	//生成する個数分
	for (int nCnt = 0; nCnt < pParam->GetNumber(); nCnt++)
	{
		//ランダム座標を計算
		D3DXVECTOR3 randompos = D3DXVECTOR3(
			CHossoLibrary::Random(pParam->GetLocalRandomPosRange().x),
			CHossoLibrary::Random(pParam->GetLocalRandomPosRange().y),
			CHossoLibrary::Random(pParam->GetLocalRandomPosRange().z));

		//0除算防止
		if (pParam->GetSpeed() == 0)
		{
			move = ZeroVector3;
		}
		else
		{
			pParam->GetSpeedRandom() ?
				fSpeed = float(rand() % 1000 / 1000.0f) * pParam->GetSpeed() :
				fSpeed = pParam->GetSpeed();

			//パーティクルの飛ぶ距離をランダム生成
			switch (pParam->GetShape())
			{
			case CParticleParam::SHAPE_SPHERE:

				//360度ランダム 3.14 - 3.14
				fAngleX = CHossoLibrary::Random_PI();
				fAngleY = CHossoLibrary::Random_PI();

				//移動の方向を設定
				move = D3DXVECTOR3(sinf(fAngleY) * sinf(fAngleX) * fSpeed,
					sinf(fAngleY) * cosf(fAngleX) * fSpeed,
					cosf(fAngleY) * cosf(fAngleX) * fSpeed);
				break;
			case CParticleParam::SHAPE_CIRCLE_XY:
				//360度ランダム 3.14 - 3.14
				fAngleX = CHossoLibrary::Random_PI();
				fAngleY = CHossoLibrary::Random_PI();

				//移動の方向を設定
				move = D3DXVECTOR3(sinf(fAngleX) *  fSpeed,
					cosf(fAngleX) * fSpeed,
					0.0f);
				break;

			case CParticleParam::SHAPE_CONE:
				//移動の方向を設定
				//視点の目的地の計算
				fAngleX = pParam->GetRot().x + CHossoLibrary::Random(pParam->GetRange());
				fAngleY = pParam->GetRot().y + CHossoLibrary::Random(pParam->GetRange());

				fAngleX += rot.x;
				fAngleY += rot.y;

				move = D3DXVECTOR3(-sinf(fAngleY) * cosf(fAngleX) * fSpeed,
					sinf(fAngleX) * fSpeed,
					-cosf(fAngleY) * cosf(fAngleX) * fSpeed);


				break;
			case CParticleParam::SHAPE_LINE:
				//移動の方向を設定
				//視点の目的地の計算
				fAngleX = pParam->GetRot().x;
				fAngleY = pParam->GetRot().y;

				fAngleX += rot.x;
				fAngleY += rot.y;

				//move = D3DXVECTOR3(-sinf(fAngleY) * cosf(fAngleX) * fSpeed,
				//	sinf(fAngleX) * fSpeed,
				//	-cosf(fAngleY) * cosf(fAngleX) * fSpeed);


				move = D3DXVECTOR3(-sinf(rot.z) * fSpeed,
									cosf(rot.z) * fSpeed,
									0.0f);

				break;
			default:
				break;
			}
		}


		//パーティクル生成
		std::unique_ptr<COneParticle>pOneParticle = COneParticle::Create(randompos, move, rot);
		//配列に追加
		m_pParticleList.emplace_back(std::move(pOneParticle));
	}

	//アニメーションのパラメータ設定
	if (m_pParticleParam->GetAnimation())
	{
		SetAnimationParam();
	}

	//判定があるときは当たり判定のポインタ作成
	if (m_pParticleParam->GetCollision())
	{
		SetCollsionParam();
	}
	//頂点の更新
	UpdateVertex();

	//音再生
	SetSound();
}

//------------------------------------------------------------------------------
//アニメーションに関する情報設定
//------------------------------------------------------------------------------
void CParticle::SetAnimationParam()
{
	CTexAnimationBase::SetLife(m_pParticleParam->GetLife());
	CTexAnimationBase::SetTex(m_pParticleParam->GetSeparateTex());
	CTexAnimationBase::SetLoop(m_pParticleParam->GetAnimationLoop());
	CTexAnimationBase::SetCntSwitch(m_pParticleParam->GetAnimationCntSwitch());

}

//------------------------------------------------------------------------------
//コリジョンの情報設定
//------------------------------------------------------------------------------
void CParticle::SetCollsionParam()
{

	////当たり判定生成
	//m_pCollision = CCollision::Create();

	////nullcheck
	//if (m_pCollision)
	//{

	//	// ゲームオブジェクト( タグ )の設定
	//	m_pCollision->SetGameObject(this);

	//	//中央が原点かどうか
	//	if (!m_pParticleParam->GetCollisionSizeCalc())
	//	{
	//		m_pCollision->SetPos(&m_posOrigin);
	//		m_pCollision->SetSize(m_pParticleParam->GetCollisionSize() * 2);
	//	}
	//	else
	//	{
	//		//コリジョンの大きさを基に当たり判定の設定
	//		//回転した時にも対応するように
	//		CalcCollisionSize(m_pParticleParam->GetCollisionSize());
	//	}

	//	//デバッグの線表示
	//	m_pCollision->DeCollisionCreate(CCollision::COLLISIONTYPE_NORMAL);
	//}
}


//------------------------------------------------------------------------------
//判定
//------------------------------------------------------------------------------
void CParticle::Collision()
{
//
////nullcheck
//	if (m_pCollision)
//	{
//		//レーザーの場合
//		if (m_pParticleParam->GetType() == CParticleParam::EFFECT_LAZER)
//		{
//			//コリジョンの長さが変わったかどうかのフラグ
//			bool bChangeLength = false;
//
//			//レイによる判定
//			//床との判定
//			if (m_pCollision->RayCollisionGetLength(m_posOrigin, m_posEndPoint, m_pParticleParam->GetSize().y))
//			{
//				//サイズを合わせる
//				m_pParticleParam->GetCollisionSize().y = m_pParticleParam->GetSize().y;
//
//				//当たり判定のサイズ計算
//				CalcCollisionSize(m_pParticleParam->GetSize());
//
//				//フラグをtrueにする
//				bChangeLength = true;
//			}
//
//
//			//盾かオブジェクトまでの距離を求める
//			if (m_pCollision->LazerCollisionGetLength(m_posOrigin, m_pParticleParam->GetSize().y))
//			{
//				//サイズを合わせる
//				m_pParticleParam->GetCollisionSize().y = m_pParticleParam->GetSize().y;
//
//				//当たり判定のサイズ計算
//				CalcCollisionSize(m_pParticleParam->GetSize());
//
//				//フラグをtrueにする
//				bChangeLength = true;
//			}
//
//			//フラグがtrueの時
//			if (bChangeLength)
//			{
//				//終点にエフェクト発生
//				CParticle::CreateFromText(m_posEndPoint, ZeroVector3, CParticleParam::EFFECT_LAZERGRASE);
//			}
//		}
//
//		//プレイヤーの攻撃だった場合
//		if (GetTag() == TAG::PLAYER_1 || GetTag() == TAG::PLAYER_2)
//		{
//			if (CManager::GetMode() == CManager::MODE_GAME)
//			{
//				//当たり判定　敵、捕虜、オブジェクトに対して
//				m_pCollision->ForPlayerBulletCollision(m_pParticleParam->GetCollisionAttackValue(), m_pParticleParam->GetCollisionAttackValue(), m_pParticleParam->GetPenetration());
//			}
//		}
//		//プレイヤーの攻撃だった場合
//		if (GetTag() == TAG::ENEMY)
//		{
//			if (CManager::GetMode() == CManager::MODE_GAME)
//			{
//				//当たり判定　プレイヤーに対して
//				m_pCollision->ForEnemyCollision(m_pParticleParam->GetCollisionAttackValue(), m_pParticleParam->GetCollisionAttackValue(), m_pParticleParam->GetPenetration());
//			}
//		}
//	}
}

//------------------------------------------------------------------------------
//レンダラーの設定1
//------------------------------------------------------------------------------
void CParticle::RendererSetting()
{
	//ライティングOFF
	CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_LIGHTING_OFF);

	//加算合成
	if (m_pParticleParam->GetAlphaBlend_Add())
	{
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_ALPHABLEND_ADD);
	}
	//減算合成
	else if (m_pParticleParam->GetAlphaBlend_Sub())
	{
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_ALPHABLEND_SUB);
	}

	//Zテスト無効　Zライティング有効
	if (!m_pParticleParam->GetZTest() && m_pParticleParam->GetZWrite())
	{
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_ZTEST_OFF);
	}
	//Zテスト有効　Zライティング無効
	else if (m_pParticleParam->GetZTest() && !m_pParticleParam->GetZWrite())
	{
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_ZTEST_ON_ZWRITING_OFF);

	}
	//Zテストもライティングも無効
	else if (!m_pParticleParam->GetZTest() && !m_pParticleParam->GetZWrite())
	{
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_ZTEST_OFF);
	}

}

//------------------------------------------------------------------------------
//エフェクトに合わせた音再生
//------------------------------------------------------------------------------
void CParticle::SetSound()
{
	//switch (m_pParticleParam->GetType())
	//{

	//}
}
