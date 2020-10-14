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
std::vector<std::unique_ptr<CScene>> CScene::m_UniqueSceneList[CScene::OBJTYPE_MAX] = {};
std::vector<std::shared_ptr<CScene>> CScene::m_SharedSceneList[CScene::OBJTYPE_MAX] = {};

bool CScene::m_bStop = false;
bool CScene::m_b1FAction = false;

int CScene::m_nNumAll = 0;

#ifdef _DEBUG

std::vector<int> CScene::m_fUpdateTimeList(CScene::OBJTYPE_MAX);
std::vector<int> CScene::m_fDrawTimeList(CScene::OBJTYPE_MAX);
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
		//要素数分繰り返す
		for (size_t nCntElement = 0; nCntElement < m_UniqueSceneList[nCntObjType].size(); nCntElement++)
		{
			//nullcheck
			if (m_UniqueSceneList[nCntObjType][nCntElement])
			{
				//更新処理
				m_UniqueSceneList[nCntObjType][nCntElement]->Uninit();
			}
		}
		//要素数分繰り返す
		for (size_t nCntElement = 0; nCntElement < m_SharedSceneList[nCntObjType].size(); nCntElement++)
		{
			//nullcheck
			if (m_SharedSceneList[nCntObjType][nCntElement])
			{
				//更新処理
				m_SharedSceneList[nCntObjType][nCntElement]->Uninit();
			}
		}
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
			//要素数分繰り返す
			for (size_t nCntElement = 0; nCntElement < m_UniqueSceneList[nCntObjType].size(); nCntElement++)
			{
				//nullcheck
				if (m_UniqueSceneList[nCntObjType][nCntElement])
				{
					//更新処理
					m_UniqueSceneList[nCntObjType][nCntElement]->Update();
				}
			}
			//要素数分繰り返す
			for (size_t nCntElement = 0; nCntElement < m_SharedSceneList[nCntObjType].size(); nCntElement++)
			{
				//nullcheck
				if (m_SharedSceneList[nCntObjType][nCntElement])
				{
					//更新処理
					m_SharedSceneList[nCntObjType][nCntElement]->Update();
				}
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
			//要素数分繰り返す
			for (size_t nCntElement = 0; nCntElement < m_UniqueSceneList[nCntObjType].size(); nCntElement++)
			{
				//nullcheck
				if (m_UniqueSceneList[nCntObjType][nCntElement])
				{
					//フラグ経ってるか確認
					if (m_UniqueSceneList[nCntObjType][nCntElement]->m_bDeadFlag)
					{
						//要素の削除
						m_UniqueSceneList[nCntObjType][nCntElement]->Uninit();
						m_UniqueSceneList[nCntObjType].erase(m_UniqueSceneList[nCntObjType].begin() + nCntElement);
					}
				}
			}
			//要素数分繰り返す
			for (size_t nCntElement = 0; nCntElement < m_SharedSceneList[nCntObjType].size(); nCntElement++)
			{
				//nullcheck
				if (m_SharedSceneList[nCntObjType][nCntElement])
				{
					//フラグ経ってるか確認
					if (m_SharedSceneList[nCntObjType][nCntElement]->m_bDeadFlag)
					{
						//要素の削除
						m_SharedSceneList[nCntObjType][nCntElement]->Uninit();
						m_SharedSceneList[nCntObjType].erase(m_SharedSceneList[nCntObjType].begin() + nCntElement);
					}
				}
			}
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

	for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
	{
		//要素数分繰り返す
		for (size_t nCntElement = 0; nCntElement < m_UniqueSceneList[nCntObjType].size(); nCntElement++)
		{
			//nullcheck
			if (m_UniqueSceneList[nCntObjType][nCntElement])
			{
				//描画処理
				m_UniqueSceneList[nCntObjType][nCntElement]->Draw();
			}
		}
		//要素数分繰り返す
		for (size_t nCntElement = 0; nCntElement < m_SharedSceneList[nCntObjType].size(); nCntElement++)
		{
			//nullcheck
			if (m_SharedSceneList[nCntObjType][nCntElement])
			{
				//描画処理
				m_SharedSceneList[nCntObjType][nCntElement]->Draw();
			}
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
	std::vector<float> OutputDataList(OBJTYPE_MAX);

	//更新と描画にかかったフレーム
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "FPS_IntervalCount >> %d\n", GetFPSInterval());

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

	for (int nCntObjType = 0; nCntObjType < OBJTYPE_MAX; nCntObjType++)
	{
		//要素数分繰り返す
		for (size_t nCntElement = 0; nCntElement < m_UniqueSceneList[nCntObjType].size(); nCntElement++)
		{
			//nullcheck
			if (m_UniqueSceneList[nCntObjType][nCntElement])
			{
				//デバッグ情報表記
				m_UniqueSceneList[nCntObjType][nCntElement]->ShowDebugInfo();
			}
		}
		//要素数分繰り返す
		for (size_t nCntElement = 0; nCntElement < m_SharedSceneList[nCntObjType].size(); nCntElement++)
		{
			//nullcheck
			if (m_SharedSceneList[nCntObjType][nCntElement])
			{
				//デバッグ表記
				m_SharedSceneList[nCntObjType][nCntElement]->ShowDebugInfo();
			}
		}
	}

#endif	//_DEBUG
}

//----------------------------------------------------------------------------
//特定のオブジェクトタイプのものをすべて消去
//----------------------------------------------------------------------------
void CScene::ReleaseSpecificObject(OBJTYPE objtype)
{
	//要素数分繰り返す
	for (size_t nCntElement = 0; nCntElement < m_UniqueSceneList[objtype].size(); nCntElement++)
	{
		//nullcheck
		if (m_UniqueSceneList[objtype][nCntElement])
		{
			//更新処理
			m_UniqueSceneList[objtype][nCntElement]->Uninit();
		}
	}
	//要素数分繰り返す
	for (size_t nCntElement = 0; nCntElement < m_SharedSceneList[objtype].size(); nCntElement++)
	{
		//nullcheck
		if (m_SharedSceneList[objtype][nCntElement])
		{
			//更新処理
			m_SharedSceneList[objtype][nCntElement]->Uninit();
		}
	}
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
