//------------------------------------------------------------------------------
//
//デバッグ　エフェクトビューワ用の処理  [Debug_EffectViewer.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "Debug_EffectViewer.h"
#include "../Scene.h"
#include "../manager.h"
#include "../renderer.h"
#include "../keyboard.h"
#include "../camera.h"
#include "../fade.h"
#include "../particle.h"
#include "../ParticleManager.h"
#include "../MeshField.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define EFFECT_CREATE_POS (D3DXVECTOR3(0.0f, 50.0f, 100.0f))
#define PARAMATER_SAVE_FILENAME ("data/Load/EffectParam/SaveParamater.txt")
#define LINE_LENGTH (500.0f)
#define LINE_OFFSET (D3DXVECTOR3(0.0f,LINE_LENGTH,0.0f))


//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CDebug_EffectViewer::CDebug_EffectViewer()
{
	m_bLoop = false;
	m_nLoopInterval = 60;
	m_nCnt = 0;
	m_pParticleParam.reset();
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CDebug_EffectViewer::~CDebug_EffectViewer()
{
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CDebug_EffectViewer::Init(HWND hWnd)
{
	//床
	CMeshField::Create(ZeroVector3, D3DXVECTOR3(50.0f, 50.0f, 50.0f), INTEGER2(20, 20));

	//パーティクルのマネージャ
	CParticleManager::Create();

	//パーティクルのパラメータのメモリ確保
	m_pParticleParam.reset(new CParticleParam);

	return S_OK;
}

//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CDebug_EffectViewer::Uninit()
{

}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CDebug_EffectViewer::Update()
{

}

//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CDebug_EffectViewer::Draw()
{

}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CDebug_EffectViewer::ShowDebugInfo()
{
#ifdef _DEBUG
	//キャラクター情報情報
	ImGui::Begin("EffectViewer");

	////Tab
	//if (ImGui::BeginTabBar("Viewer", m_bModel))
	//{
	//	//Tab
	//	if (ImGui::BeginTabItem("MotionViewer"))
	//	{
			//エフェクトビューワ
			ParticleParamaterViewer();
			//ImGui::EndTabItem();
	//	}

	//	//TabEnd
	//	ImGui::EndTabBar();
	//}

	ImGui::End();

#endif
}

//------------------------------------------------------------------------------
//パーティクルのパラメータのビューワ
//------------------------------------------------------------------------------
void CDebug_EffectViewer::ParticleParamaterViewer()
{
	//読み込むテキストの番号
	static CParticleParam::PARTICLE_TYPE NowText = CParticleParam::PARTICLE_TYPE::EFFECT_DEFAULT;
	static FILENAME_LIST aShapeName = { "Sphere","Cone","Circle_XY","Line" };
	static bool bSquare = false;	//正方形を保つか
	//キーボードのポインタ取得
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//止まってるかどうか
	if (!CScene::GetStop())
	{
		//カウンタ++
		m_nCnt++;
	}


	//Debug
	CDebugProc::Print(CDebugProc::PLACE_LEFT,"[Enter] パーティクル発生\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT,"[Ctrl] + [Enter] Loop %d \n", m_bLoop);

	//コンボボックス　ファイル名
	if (CHossoLibrary::ImGui_Combobox(CParticleParam::GetFileNameList(), "ParamFileName", (int&)NowText))
	{
		//メモリ確保
		CParticleParam *pParam = new CParticleParam;

		//パラメータをコピー
		//unique_ptrだと代入できない
		*pParam = *CParticleParam::GetDefaultParam(NowText);

		//メモリ確保しなおし
		m_pParticleParam.reset(std::move(pParam));
	}

	//コンボボックス　テクスチャ
	m_pParticleParam->GetAnimation() ?
		CHossoLibrary::ImGui_Combobox(CTexture::GetSeparateFileName(), "SeparateTex", (int&)m_pParticleParam->GetSeparateTex()) :	//真
		CHossoLibrary::ImGui_Combobox(CTexture::GetTexFileName(), "Texture", (int&)m_pParticleParam->GetTex());						//偽

	//項目の大きさ設定
	ImGui::PushItemWidth(200);

	//改行
	ImGui::Separator();

	//アニメーションするかどうか
	ImGui::Checkbox("bAnimation", &m_pParticleParam->GetAnimation());

	//アニメーションする時
	if (m_pParticleParam->GetAnimation())
	{
		//ツリー
		if(ImGui::TreeNode("AnimationParam"))
		{
			//アニメーションパラメータ設定

			//ループするか
			ImGui::Checkbox("bAnimationLoop", &m_pParticleParam->GetAnimationLoop());

			//アニメーション切り替えのカウント
			ImGui::InputInt("AnimationCntSwitch", &m_pParticleParam->GetAnimationCntSwitch(), 1, 1, 20);

			//ツリー終了
			ImGui::TreePop();
		}
	}

	//改行
	ImGui::Separator();

	//当たり判定があるかどうか
	ImGui::Checkbox("bCollision", &m_pParticleParam->GetCollision());

	//当たり判定がある時
	if (m_pParticleParam->GetCollision())
	{
		//ツリー
		if (ImGui::TreeNode("CollisionParam"))
		{
			//当たり判定があるかどうか
			ImGui::Checkbox("m_bCollisionSizeCalc", &m_pParticleParam->GetCollisionSizeCalc());

			//アニメーションパラメータ設定
			ImGui::DragFloat3("CollisionSize", m_pParticleParam->GetCollisionSize(), 0.5f, 1.0f, 250.0f);

			//ボタン　テクスチャのサイズにする
			if(ImGui::Button("TexSize"))
			{
				//コリジョンのサイズをテクスチャのサイズにする
				m_pParticleParam->GetCollisionSize() = m_pParticleParam->GetSize();
			}

			//コリジョンの継続時間
			ImGui::InputInt("CollisionCnt", &m_pParticleParam->GetCollisionCnt());

			//攻撃力
			ImGui::InputInt("CollisionAttackValue", &m_pParticleParam->GetCollisionAttackValue());

			//ツリー終了
			ImGui::TreePop();
		}
	}

	//改行
	ImGui::Separator();

	//ツリー
	if (ImGui::TreeNode("RendererSetting"))
	{
		//加算合成するか
		ImGui::Checkbox("bAlphaBlend_Add", &m_pParticleParam->GetAlphaBlend_Add());
		ImGui::SameLine();

		//減算合成するか
		ImGui::Checkbox("bAlphaBlend_Sub", &m_pParticleParam->GetAlphaBlend_Sub());

		//Zテストするか
		ImGui::Checkbox("bZTest", &m_pParticleParam->GetZTest());
		ImGui::SameLine();

		//Zライティングするか
		ImGui::Checkbox("bZWrite", &m_pParticleParam->GetZWrite());

		//ビルボードするか
		ImGui::Checkbox("bBillboard", &m_pParticleParam->GetBillboard());

		//追従するか
		ImGui::Checkbox("bFollowing", &m_pParticleParam->GetFollowing());

		//ツリー終了
		ImGui::TreePop();
	}

	//改行
	ImGui::Separator();

	//コンボボックス　Shape
	CHossoLibrary::ImGui_Combobox(aShapeName, "Shape", (int&)m_pParticleParam->GetShape());

	//回転量
	if (ImGui::DragFloat3("rot", m_pParticleParam->GetRot(), 0.005f, -D3DX_PI, D3DX_PI))
	{
		//
		D3DXVECTOR3 TargetPos = D3DXVECTOR3(-sinf(m_pParticleParam->GetRot().y) * cosf(m_pParticleParam->GetRot().x) * LINE_LENGTH,
									sinf(m_pParticleParam->GetRot().x) * LINE_LENGTH,
									-cosf(m_pParticleParam->GetRot().y) * cosf(m_pParticleParam->GetRot().x) * LINE_LENGTH);

	}
	ImGui::DragFloat("fRange", &m_pParticleParam->GetRange(), 0.01f, -D3DX_PI, D3DX_PI);

	//改行
	ImGui::Separator();

	//パラメータ設定
	ImGui::DragInt("Life", &m_pParticleParam->GetLife(), 1, 1, 300);
	ImGui::DragInt("ParticleNum", &m_pParticleParam->GetNumber(), 1, 1, 300);
	ImGui::DragFloat("Speed", &m_pParticleParam->GetSpeed(), 0.5f, 0.0f, 250.0f);

	//同じ行
	ImGui::SameLine();

	//スピードランダムか
	ImGui::Checkbox("bSpeedRandom", &m_pParticleParam->GetSpeedRandom());

	ImGui::DragFloat3("LocalPos", m_pParticleParam->GetLocalPos(), 0.5f, 1.0f, 300.0f);
	ImGui::DragFloat3("LocalPos_Range", m_pParticleParam->GetLocalRandomPosRange(), 0.5f, 1.0f, 300.0f);



	//正方形を保つとき
	if (bSquare)
	{
		//サイズで設定した値をxとyにちゃんと入れる
		//xのパラメータをいじってる
		float fSize = m_pParticleParam->GetSize().x;
		float fSizeDamping = m_pParticleParam->GetSizeDamping().x;

		ImGui::DragFloat("Size", &fSize, 0.5f, 1.0f, 250.0f);
		ImGui::DragFloat("SizeDamping", &fSizeDamping, 0.001f, 0.5f, 1.0f);

		m_pParticleParam->GetSize().x = m_pParticleParam->GetSize().y = fSize;
		m_pParticleParam->GetSizeDamping().x = m_pParticleParam->GetSizeDamping().y = fSizeDamping;
	}
	else
	{
		//x.y.zそれぞれのパラメータ
		ImGui::DragFloat3("Size", m_pParticleParam->GetSize(), 0.5f, 1.0f, 250.0f);
		ImGui::DragFloat3("SizeDamping", m_pParticleParam->GetSizeDamping(), 0.001f, 0.5f, 1.0f);
	}
	//同じ行
	ImGui::SameLine();

	//正方形を保つか
	ImGui::Checkbox("Square", &bSquare);

	//a値の減衰量
	ImGui::DragFloat("AlphaDamping", &m_pParticleParam->GetAlphaDamping(), 0.001f, 0.5f, 1.0f);

	//重力かけるか
	ImGui::Checkbox("bGravity", &m_pParticleParam->GetGravity());
	//重力がONの時
	if (m_pParticleParam->GetGravity())
	{
		//同じ行
		ImGui::SameLine();

		//重力の大きさ
		ImGui::DragFloat("GravityPower", &m_pParticleParam->GetGravityPower(), 0.005f, 0.1f, 2.0f);
	}

	//軌跡の色　float型にキャスト
	float *rCol = m_pParticleParam->GetCol();

	//項目の大きさ設定
	ImGui::SetNextItemWidth(250);

	//色の設定
	ImGui::ColorEdit4("Color", rCol);

	//改行
	ImGui::Separator();

	//パーティクル情報保存
	if (ImGui::Button("Save"))
	{
		//セーブ
		CParticleParam::SaveParticleDefaultParam(m_pParticleParam.get(), PARAMATER_SAVE_FILENAME);
	}

	if (ImGui::TreeNode("OverRideSave"))
	{

		//パーティクル情報保存
		if (ImGui::Button("OverRideSave"))
		{
			//セーブ
			CParticleParam::SaveParticleDefaultParam(m_pParticleParam.get(), m_pParticleParam->GetFileNameList()[NowText]);
		}

		ImGui::TreePop();
	}


	//[Ctrl] + [Enter]
	if ((pKeyboard->GetPress(DIK_LCONTROL) && pKeyboard->GetTrigger(DIK_RETURN)))
	{
		//onoff設定
		m_bLoop ^= 1;
	}

	//( [Enter]を押したとき　または　ループする時 ) なおかつSceneが止まってない時
	if (pKeyboard->GetTrigger(DIK_RETURN) || ((m_bLoop && m_nCnt % m_nLoopInterval == 0) && !CScene::GetStop()))
	{
		CParticle::CreateFromParam(EFFECT_CREATE_POS,ZeroVector3 ,m_pParticleParam.get());
	}

}
