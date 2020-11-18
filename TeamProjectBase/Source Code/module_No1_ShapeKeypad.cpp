//------------------------------------------------------------------------------
//
//モジュールの図形のキーパッド処理  [module_No1_ShapeKeypad.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_No1_ShapeKeypad.h"
#include "module_parts_No1_ShapeKey.h"
#include "module_parts_ProgressLamp.h"
#include "renderer.h"
#include "manager.h"
#include "modelinfo.h"
#include "timer.h"
#include "keyboard.h"
#include "game.h"
#include "Mylibrary.h"
#include "scene3D.h"
#include "mouse.h"
#include "sound.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define KEYPAD_NUM						(4)									//パッドの総数
#define KEYPAD_INTERVAL					(D3DXVECTOR3(25.0f,0.0f,0.0f))		//パッドの間隔
#define KEYPAD_OFFSET					(D3DXVECTOR3(1.0f,-15.0f,-25.0f))	//パッドのオフセット座標
#define DISPLAY_SHAPE_OFFSET			(D3DXVECTOR3(-18.0f,18.0f,-25.0f))	//ディスプレイの記号のオフセット間隔
#define DISPLAY_SHAPE_SIZE				(D3DXVECTOR3(20.0f,20.0f,0.0f))		//ディスプレイの記号のサイズ
#define PROGRESS_LAMP_OFFSET			(D3DXVECTOR3(29.0f,7.0f,-25.0f))	//進捗度ランプのオフセット

#define QUESTION_CHANGE_TIME			(40)								//問題変更時にかかる時間

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_No1_ShapeKeyPad::CModule_No1_ShapeKeyPad()
{
	//初期化
	m_pKeyPadList = {};
	m_pProgressLamp = {};

	m_nSelectNum = PLACE::LEFT_TO_1ST;
	m_NowQuestion = QUESTION::Q_1;
	m_DisplayShape = SHAPE::CIRCLE;
	m_state = STATE::NORMAL;
	m_nQuestionChangeCnt = 0;

	//配列の大きさ設定
	m_Memories.resize((int)CModule_No1_ShapeKeyPad::QUESTION::MAX);
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_No1_ShapeKeyPad::~CModule_No1_ShapeKeyPad()
{
	m_pKeyPadList.clear();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_No1_ShapeKeyPad::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_NO1));

	//ランプ生成
	CModule_Base::CreateLamp();

	//キーパッド生成
	CreateKeyPad();

	//ディスプレイの図形生成
	CreateDisplayShape();

	//進捗度のランプ生成
	CreateProgressLamp();

	//図形シャッフル
	ShapeShuffle();

	//次押すボタンセット
	SetNextPushKey();

	CSceneX::Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::Update()
{
	CSceneX::Update();

	//ステート更新
	UpdateState();

	//nullcheck
	if (m_pDisplayShape)
	{
		m_pDisplayShape->Update();
	}

}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::Draw()
{
	CSceneX::Draw();

	//nullcheck
	if (m_pDisplayShape)
	{
		m_pDisplayShape->Draw();
	}
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::ShowDebugInfo()
{
#ifdef _DEBUG

	//CDebugProc::Print(CDebugProc::PLACE_LEFT, "State >> [%d]\n", m_state);
	//CDebugProc::Print(CDebugProc::PLACE_LEFT, "CntState >> [%d]\n", m_nQuestionChangeCnt);


	//ImGui::DragFloat3("pos1", m_pKeyPadList[0]->GetPos(),0.05f);
	//ImGui::DragFloat3("pos2", m_pKeyPadList[1]->GetPos(), 0.05f);
	//ImGui::DragFloat3("pos3", m_pKeyPadList[2]->GetPos(), 0.05f);
	//ImGui::DragFloat3("pos4", m_pKeyPadList[3]->GetPos(), 0.05f);

	//選択されているとき
	if (GetSelect())
	{
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "-------------Module_No_2----------------\n");

		CDebugProc::Print(CDebugProc::PLACE_LEFT, "Flag >> ");

		for (size_t nCnt = 0; nCnt < m_pKeyPadList.size(); nCnt++)
		{
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "[%d] ", m_pKeyPadList[nCnt]->GetClearFlag());
		}

		CDebugProc::Print(CDebugProc::PLACE_LEFT, NEWLINE);

		for (size_t nCnt = 0; nCnt < m_Memories.size(); nCnt++)
		{
			CDebugProc::Print(CDebugProc::PLACE_LEFT, "Memories >> Place[%d] Shape[%d]\n", m_Memories[nCnt].place, m_Memories[nCnt].shape);
		}
	}

#endif //DEBUG
}
//------------------------------------------------------------------------------
//キーパッド操作
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::Operation_Keyboard()
{
	int nSelectNumOld = m_nSelectNum;

	//入力が無かった時はbreak
	CHossoLibrary::Selecting((int&)m_nSelectNum, nSelectNumOld, 4, 1);

	//選択している状態のモノを設定
	CModule_Base::ModuleParts_Select<CModule_Parts_No1_ShapeKey>(m_pKeyPadList, m_nSelectNum);

	//モジュール操作
	CModule_Base::Operation_Keyboard();

	//ステートがNORMALじゃない時
	if (m_state != STATE::NORMAL)
	{
		//選択解除
		CModule_Base::ModuleParts_Select<CModule_Parts_No1_ShapeKey>(m_pKeyPadList, -1);
	}
}

//------------------------------------------------------------------------------
//モジュール操作　マウス
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::Operation_Mouse()
{
	//レイの判定
	CHossoLibrary::RayCollision_ModuleSelect(m_pKeyPadList.begin(), m_pKeyPadList.end(), (int&)m_nSelectNum);

	//マウス操作
	CModule_Base::Operation_Mouse();

	//ステートがNORMALじゃない時
	if (m_state != STATE::NORMAL)
	{
		//選択解除
		CModule_Base::ModuleParts_Select<CModule_Parts_No1_ShapeKey>(m_pKeyPadList, -1);
	}
}


//------------------------------------------------------------------------------
//モジュール
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::ModuleAction()
{
	//選択番号が-1とかだった時
	if (m_nSelectNum < 0 || m_state != STATE::NORMAL)
	{
		return;
	}

	if (m_pKeyPadList[m_nSelectNum].get())
	{

		//音再生
		CManager::GetSound()->Play(CSound::LABEL_SE_MODULE_PUSH);

		//押したボタンがクリアボタンだった場合
		if (m_pKeyPadList[m_nSelectNum]->GetClearFlag())
		{
			//クリア
			QuestionClear();

			//クリアしたかチェック
			CheckClear();
		}
		else
		{
			//失敗
			Module_Failed();
		}
	}
}

//------------------------------------------------------------------------------
//モジュールの選択解除
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::ModuleCancel()
{
	//選択解除
	CModule_Base::ModuleParts_Select<CModule_Parts_No1_ShapeKey>(m_pKeyPadList, -1);

}

//------------------------------------------------------------------------------
//ステート更新
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::UpdateState()
{

	switch (m_state)
	{
	case STATE::NORMAL:

		break;

	case STATE::DISAPPEAR:
		//カウントダウン
		m_nQuestionChangeCnt--;

		for (int nCnt = 0; nCnt < (int)m_pKeyPadList.size(); nCnt++)
		{
			//左から順番に
			if ((QUESTION_CHANGE_TIME - m_nQuestionChangeCnt) / (QUESTION_CHANGE_TIME / 4) >= nCnt)
			{
				//奥に移動
				m_pKeyPadList[nCnt]->GetPos().z += 1.0f;

				//移動範囲設定
				CHossoLibrary::RangeLimit_Equal(m_pKeyPadList[nCnt]->GetPos().z, DISPLAY_SHAPE_OFFSET.z,-10.0f );
			}
		}

		if (m_nQuestionChangeCnt <= 0)
		{
			//ステート切り替え
			SetState(STATE::APPEAR);

			//図形シャッフル
			ShapeShuffle();

			//次の問題設定
			SetNextPushKey();
		}

		break;

	case STATE::APPEAR:

		//カウントダウン
		m_nQuestionChangeCnt--;

		for (int nCnt = 0; nCnt < (int)m_pKeyPadList.size(); nCnt++)
		{
			//左から順番に
			if ((QUESTION_CHANGE_TIME - m_nQuestionChangeCnt) / (QUESTION_CHANGE_TIME / 4) >= nCnt)
			{
				//手前に移動
				m_pKeyPadList[nCnt]->GetPos().z -= 1.0f;

				//移動範囲制限
				CHossoLibrary::RangeLimit_Equal(m_pKeyPadList[nCnt]->GetPos().z, DISPLAY_SHAPE_OFFSET.z, -10.0f);
			}
		}

		if (m_nQuestionChangeCnt <= 0)
		{
			SetState(STATE::NORMAL);
		}
		break;
	}

}

//------------------------------------------------------------------------------
//問題クリア
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::QuestionClear()
{
	//押したボタンを保存
	m_Memories[(int)m_NowQuestion].place = m_nSelectNum;
	m_Memories[(int)m_NowQuestion].shape = m_pKeyPadList[m_nSelectNum]->GetShape();

	for (size_t nCnt = 0; nCnt < m_pKeyPadList.size(); nCnt++)
	{
		//フラグ初期化
		m_pKeyPadList[nCnt]->SetClearFlag(false);
	}

	//次の問題に移行
	int nValue = (int)m_NowQuestion;
	nValue++;
	m_NowQuestion = (QUESTION)nValue;

	//進捗ランプの更新
	m_pProgressLamp->SetProgress(nValue);

	//問題全て終えていた場合
	if (m_NowQuestion >= QUESTION::MAX)
	{
		return;
	}


	//ステート切り替え
	SetState(STATE::DISAPPEAR);
}

//------------------------------------------------------------------------------
//ディスプレイの図形生成
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::CreateDisplayShape()
{
	//ディスプレイの記号生成
	m_pDisplayShape = CSceneBase::ScenePolygonCreateSelfManagement<CScene3D>
		(DISPLAY_SHAPE_OFFSET, DISPLAY_SHAPE_SIZE, GreenColor, CTexture::GetSeparateTexture(CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01));

	//親マトリックス設定
	m_pDisplayShape->SetParentMtxPtr(GetMtxWorldPtr());

	//ライティングoff
	m_pDisplayShape->SetLighting(false);
}

//------------------------------------------------------------------------------
//キーパッド生成
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::CreateKeyPad()
{
	//全てのキーに割り当てる
	for (size_t nCnt = 0; nCnt < KEYPAD_NUM; nCnt++)
	{
		//キー生成
		m_pKeyPadList.emplace_back(CModule_Parts_Base::Create_ModuleParts<CModule_Parts_No1_ShapeKey>
			(KEYPAD_OFFSET + D3DXVECTOR3(-((KEYPAD_INTERVAL.x) * (KEYPAD_NUM / 2)) + (KEYPAD_INTERVAL.x * 0.5f) + (KEYPAD_INTERVAL.x * nCnt), 0.0f, 0.0f), GetMtxWorldPtr()));
	}
}

//------------------------------------------------------------------------------
//進捗度のランプ生成
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::CreateProgressLamp()
{
	m_pProgressLamp = CModule_Parts_Base::Create_ModuleParts<CModule_Parts_ProgressLamp>(PROGRESS_LAMP_OFFSET, GetMtxWorldPtr());
}

//------------------------------------------------------------------------------
//クリアしたかチェック
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::CheckClear()
{
	//問題を全て終えた時
	if (m_NowQuestion >= QUESTION::MAX)
	{
		//モジュールクリア
		CModule_Base::Module_Clear();
	}
}
//------------------------------------------------------------------------------
//図形をシャッフル
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::ShapeShuffle()
{
	//番号が入ってる配列
	Vec<SHAPE> nShapeList = { SHAPE::CIRCLE,SHAPE::CROSS ,SHAPE::TRIANGLE ,SHAPE::SQUARE };

	//シャッフル
	CHossoLibrary::Vec_Shuffle(nShapeList);

	for (size_t nCnt = 0; nCnt < nShapeList.size(); nCnt++)
	{
		//図形設定
		m_pKeyPadList[nCnt]->SetShape(nShapeList[nCnt]);
	}

	//ディスプレイの図形もランダムで設定
	SetDisplayShape(rand() % KEYPAD_NUM);

}
//------------------------------------------------------------------------------
//アニメーション設定
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::SetDisplayShape(int nShape)
{
	//図形設定
	m_DisplayShape = (SHAPE)nShape;

	//UV設定
	m_pDisplayShape->SetAnimation(CHossoLibrary::CalcUV_StaticFunc(nShape, CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01),
		CTexture::GetSparateTex_UVSize(CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01));
}

//------------------------------------------------------------------------------
//次の押すキー設定
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::SetNextPushKey()
{
	//現在が何問目か
	switch (m_NowQuestion)
	{
	case CModule_No1_ShapeKeyPad::QUESTION::Q_1:
		switch (m_DisplayShape)
		{
		case CModule_No1_ShapeKeyPad::SHAPE::CIRCLE:
			SetNextPushKey_FromPlace(PLACE::LEFT_TO_2ND);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::CROSS:
			SetNextPushKey_FromPlace(PLACE::LEFT_TO_4TH);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::TRIANGLE:
			SetNextPushKey_FromShape(SHAPE::TRIANGLE);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::SQUARE:
			SetNextPushKey_FromShape(SHAPE::CROSS);
			break;
		}
		break;

	case CModule_No1_ShapeKeyPad::QUESTION::Q_2:
		switch (m_DisplayShape)
		{
		case CModule_No1_ShapeKeyPad::SHAPE::CIRCLE:
			SetNextPushKey_FromPlace(m_Memories[(int)QUESTION::Q_1].place);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::CROSS:
			SetNextPushKey_FromPlace(PLACE::LEFT_TO_4TH);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::TRIANGLE:
			SetNextPushKey_FromPlace(PLACE::LEFT_TO_2ND);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::SQUARE:
			SetNextPushKey_FromShape(SHAPE::CIRCLE);
			break;
		}
		break;

	case CModule_No1_ShapeKeyPad::QUESTION::Q_3:
		switch (m_DisplayShape)
		{
		case CModule_No1_ShapeKeyPad::SHAPE::CIRCLE:
			SetNextPushKey_FromPlace(PLACE::LEFT_TO_3RD);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::CROSS:
			SetNextPushKey_FromShape(m_Memories[(int)QUESTION::Q_1].shape);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::TRIANGLE:
			SetNextPushKey_FromShape(SHAPE::SQUARE);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::SQUARE:
			SetNextPushKey_FromPlace(PLACE::LEFT_TO_1ST);
			break;
		}
		break;

	case CModule_No1_ShapeKeyPad::QUESTION::Q_4:
		switch (m_DisplayShape)
		{
		case CModule_No1_ShapeKeyPad::SHAPE::CIRCLE:
			SetNextPushKey_FromPlace(m_Memories[(int)QUESTION::Q_2].place);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::CROSS:
			SetNextPushKey_FromPlace(PLACE::LEFT_TO_2ND);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::TRIANGLE:
			SetNextPushKey_FromShape(SHAPE::CIRCLE);
			break;

		case CModule_No1_ShapeKeyPad::SHAPE::SQUARE:
			SetNextPushKey_FromPlace(PLACE::LEFT_TO_4TH);
			break;
		}
		break;

	}
}

//------------------------------------------------------------------------------
//次の押すキー設定　図形を基に設定
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::SetNextPushKey_FromShape(SHAPE shape)
{
	//入力されたshapeと同じキー検索
	auto itr = std::find_if(m_pKeyPadList.begin(), m_pKeyPadList.end(),
		[shape](S_ptr<CModule_Parts_No1_ShapeKey> pPtr) {return pPtr->GetShape() == shape; });

	//nullcheck
	if (itr != m_pKeyPadList.end())
	{
		//クリアフラグ設定
		itr->get()->SetClearFlag(true);
	}
}

//------------------------------------------------------------------------------
//次の押すキー設定　場所を基に設定
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::SetNextPushKey_FromPlace(PLACE place)
{
	//nullcheck
	if (m_pKeyPadList[place])
	{
		//クリアフラグ設定
		m_pKeyPadList[place]->SetClearFlag(true);
	}
}


//------------------------------------------------------------------------------
//ステート設定
//------------------------------------------------------------------------------
void CModule_No1_ShapeKeyPad::SetState(STATE state)
{

	m_state = state;

	switch (m_state)
	{
	case STATE::NORMAL:

		break;

	case STATE::DISAPPEAR:
		m_nQuestionChangeCnt = QUESTION_CHANGE_TIME;
		break;

	case STATE::APPEAR:
		m_nQuestionChangeCnt = QUESTION_CHANGE_TIME;
		break;

	default:
		break;
	}

}
