//------------------------------------------------------------------------------
//
//シーン処理  [scene.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "scene.h"
#include "scene2D.h"
#include "particle.h"
//------------------------------------------------------------------------------
//静的変数の初期化
//------------------------------------------------------------------------------
Vec<U_ptr<CScene>> CScene::m_UniqueSceneList[CScene::OBJTYPE_MAX] = {};
Vec<S_ptr<CScene>> CScene::m_SharedSceneList[CScene::OBJTYPE_MAX] = {};

bool CScene::m_bStop = false;
bool CScene::m_b1FAction = false;

int CScene::m_nNumAll = 0;

#ifdef _DEBUG

Vec<int> CScene::m_fUpdateTimeList(CScene::OBJTYPE_MAX);
Vec<int> CScene::m_fDrawTimeList(CScene::OBJTYPE_MAX);
#endif //_DEBUG

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CScene::CScene()
{
	//初期化
	m_bDeadFlag = false;
	m_objtype = OBJTYPE_NONE;
	m_nNumAll++;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CScene::~CScene()
{
	//全体数加算
	m_nNumAll--;
}

//------------------------------------------------------------------------------
//メモリ開放
//------------------------------------------------------------------------------
void CScene::ReleaseAll()
{
	for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
	{
		//全要素削除
		m_UniqueSceneList[nCntObjType].clear();
		m_SharedSceneList[nCntObjType].clear();
	}
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CScene::UpdateAll()
{
#ifdef _DEBUG
	//現在の時間を保存
	int nBeforeUpdateTime = timeGetTime();
#endif	//_DEBUG

	//Stop状態じゃない時 か1Fアクションがtrueの時
	if (!m_bStop || m_b1FAction)
	{
		for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
		{
			//Shared_ptrの配列の要素分繰り返す
			for (auto &ptr : m_UniqueSceneList[nCntObjType])
			{
				//更新
				ptr->Update();
			}

			//Shared_ptrの配列の要素分繰り返す
			for (auto &ptr : m_SharedSceneList[nCntObjType])
			{
				//更新
				ptr->Update();
			}

#ifdef _DEBUG
			//配列の値を書き換え
			//現在の時間 - 前回の更新が終わった時間
			m_fUpdateTimeList[nCntObjType] = timeGetTime() - nBeforeUpdateTime;

			//更新の時間を保存
			nBeforeUpdateTime = timeGetTime();
#endif	//_DEBUG
		}

		for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
		{
			//フラグが立っているモノを削除
			std::remove_if(m_UniqueSceneList[nCntObjType].begin(), m_UniqueSceneList[nCntObjType].end(),
				[](U_ptr<CScene> &ptr) {return ptr->GetDeleteFlag(); });

			//フラグが立っているモノを削除
			std::remove_if(m_SharedSceneList[nCntObjType].begin(), m_SharedSceneList[nCntObjType].end(),
				[](S_ptr<CScene> ptr) {return ptr->GetDeleteFlag(); });
		}
	}
	//パーティクルの頂点IDリセット
	CParticle::ResetVertexID();

	//1F更新の時
	m_b1FAction = false;
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CScene::DrawAll()
{
#ifdef _DEBUG
	//現在の時間を保存
	int nBeforeUpdateTime = timeGetTime();
#endif	//_DEBUG

	//オブジェタイプ分繰り返す
	for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
	{
		//Shared_ptrの配列の要素分繰り返す
		for (auto &ptr : m_UniqueSceneList[nCntObjType])
		{
			//描画処理
			ptr->Draw();
		}

		//Shared_ptrの配列の要素分繰り返す
		for (auto &ptr : m_SharedSceneList[nCntObjType])
		{
			//描画処理
			ptr->Draw();
		}

#ifdef _DEBUG
		//配列の値を書き換え
		//現在の時間 - 前回の更新が終わった時間
		m_fDrawTimeList[nCntObjType] = timeGetTime() - nBeforeUpdateTime;

		//更新の時間を保存
		nBeforeUpdateTime = timeGetTime();
#endif	//_DEBUG

	}
}
//------------------------------------------------------------------------------
//デバッグ用の情報表示
//------------------------------------------------------------------------------
void CScene::ShowDebugInfoAll()
{
#ifdef _DEBUG

	//グラフ用の配列
	Vec<float> OutputDataList(OBJTYPE_MAX);

	//更新と描画にかかったフレーム
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "FPS_IntervalCount >> %d\n", GetFPSInterval());

	if(ImGui::TreeNode("Graph"))
	{
		for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
		{
			//リストに追加
			OutputDataList[nCntObjType] = (m_fUpdateTimeList[nCntObjType]) / (float)GetFPSInterval();
		}
		//更新のグラフ生成
		ImGui::PlotHistogram("Update!", OutputDataList.data(), OutputDataList.size(), 0, NULL, 0.0f, 1.0f, ImVec2(0, 100));

		for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
		{
			//リストに追加
			OutputDataList[nCntObjType] = (m_fDrawTimeList[nCntObjType]) / (float)GetFPSInterval();
		}

		//描画に関するグラフ
		ImGui::PlotHistogram("Renderer", OutputDataList.data(), OutputDataList.size(), 0, NULL, 0.0f, 1.0f, ImVec2(0, 100));

		//配列クリア
		OutputDataList.clear();

		//ツリーノード終了
		ImGui::TreePop();
	}

	for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
	{
		//Shared_ptrの配列の要素分繰り返す
		for (auto &ptr : m_UniqueSceneList[nCntObjType])
		{
			//デバッグ情報表記
			ptr->ShowDebugInfo();
		}

		//Shared_ptrの配列の要素分繰り返す
		for (auto &ptr : m_SharedSceneList[nCntObjType])
		{
			//デバッグ情報表記
			ptr->ShowDebugInfo();
		}
	}

#endif	//_DEBUG
}

//----------------------------------------------------------------------------
//特定のオブジェクトタイプのものをすべて消去
//----------------------------------------------------------------------------
void CScene::ReleaseSpecificObject(OBJTYPE objtype)
{
	//全要素削除
	m_UniqueSceneList[objtype].clear();
	m_SharedSceneList[objtype].clear();
}

//----------------------------------------------------------------------------
//シーン情報取得 配列ごと渡す
//----------------------------------------------------------------------------
void CScene::GetSceneList(OBJTYPE objtype, std::vector<std::shared_ptr<CScene>> &SceneList)
{
	SceneList = m_SharedSceneList[objtype];
}
//----------------------------------------------------------------------------
//それぞれの描画数表示
//----------------------------------------------------------------------------
void CScene::ShowNumAll()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Scene_NumAll"))
	{
		for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
		{
			ImGui::Text("NumAll [%d] -> [%d]", nCntObjType, m_SharedSceneList[nCntObjType].size() + m_UniqueSceneList[nCntObjType].size());
		}
		ImGui::TreePop();
	}
#endif // _DEBUG
}
