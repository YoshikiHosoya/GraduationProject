//------------------------------------------------------------------------------
//
//モジュールのボタン処理  [Module_No0_SymbolKeypad.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_No0_SymbolKeyPad.h"
#include "module_parts_No0_SymbolKey.h"
#include "renderer.h"
#include "manager.h"
#include "modelinfo.h"
#include "timer.h"
#include "Mylibrary.h"
#include "sound.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define MAX_KEYPAD						(9)									//パッドの総数
#define KEYPAD_INTERVAL					(D3DXVECTOR3(20.0f,20.0f,0.0f))		//パッドの間隔
#define KEYPAD_OFFSET					(D3DXVECTOR3(-10.0f,0.0f,-20.0f))	//パッドのオフセット座標
#define KEYPAD_NEED_CLEAR_KEY_NUM		(4)									//クリアに必要なキー数

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_No0_SymbolKeyPad::CModule_No0_SymbolKeyPad()
{
	m_pKeyPadList = {};
	m_nNextSymbolNum = 0;
	m_nSelectNum = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_No0_SymbolKeyPad::~CModule_No0_SymbolKeyPad()
{
	m_pKeyPadList.clear();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_No0_SymbolKeyPad::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_NO0));

	//ランプ生成
	CModule_Base::CreateLamp();

	//答えのパターンをランダムで設定
	m_Answer = (CModule_No0_SymbolKeyPad::ANSWER_PATTERN)(rand() & CModule_No0_SymbolKeyPad::ANSWER_6);

	//キーパッド生成
	CreateKeyPad(m_Answer);

	//次の答え設定
	m_nNextSymbolNum = m_Answer * KEYPAD_NEED_CLEAR_KEY_NUM;

	CSceneX::Init();
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_No0_SymbolKeyPad::Update()
{
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_No0_SymbolKeyPad::Draw()
{
	CSceneX::Draw();
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModule_No0_SymbolKeyPad::ShowDebugInfo()
{
#ifdef _DEBUG

	//選択されているとき
	if (GetSelect())
	{

		CDebugProc::Print(CDebugProc::PLACE_LEFT, "-------------Module_No_1----------------\n");

		CDebugProc::Print(CDebugProc::PLACE_LEFT, "KeyPad >> [%d] [%d] [%d]\n", m_pKeyPadList[0]->GetSymbolNum(), m_pKeyPadList[1]->GetSymbolNum(), m_pKeyPadList[2]->GetSymbolNum());
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "KeyPad >> [%d] [%d] [%d]\n", m_pKeyPadList[3]->GetSymbolNum(), m_pKeyPadList[4]->GetSymbolNum(), m_pKeyPadList[5]->GetSymbolNum());
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "KeyPad >> [%d] [%d] [%d]\n", m_pKeyPadList[6]->GetSymbolNum(), m_pKeyPadList[7]->GetSymbolNum(), m_pKeyPadList[8]->GetSymbolNum());

		CDebugProc::Print(CDebugProc::PLACE_LEFT, "NextSymbolNum >> [%d] \n", m_nNextSymbolNum);

	}

#endif //DEBUG
}
//------------------------------------------------------------------------------
//キーパッド操作
//------------------------------------------------------------------------------
void CModule_No0_SymbolKeyPad::Operation_Keyboard()
{
	int nSelectNumOld = m_nSelectNum;
	int nCntClearPad = 0;

	//選択処理
	//入力が無かった時はbreak
	CHossoLibrary::Selecting(m_nSelectNum, nSelectNumOld, 3, 3);

	//選択している状態のモノを設定
	CModule_Base::ModuleParts_Select<CModule_Parts_No0_SymbolKey>(m_pKeyPadList, m_nSelectNum);

	//キー操作
	CModule_Base::Operation_Keyboard();
}

//------------------------------------------------------------------------------
//モジュール操作　マウス
//------------------------------------------------------------------------------
void CModule_No0_SymbolKeyPad::Operation_Mouse()
{
	//レイの判定
	CHossoLibrary::RayCollision_ModuleSelect(m_pKeyPadList.begin(),m_pKeyPadList.end(), m_nSelectNum);

	//マウス操作
	CModule_Base::Operation_Mouse();
}

//------------------------------------------------------------------------------
//モジュールアクション
//------------------------------------------------------------------------------
void CModule_No0_SymbolKeyPad::ModuleAction()
{
	//選択番号が-1とかだった時
	if (m_nSelectNum < 0)
	{
		return;
	}
	//nullcheck
	if (m_pKeyPadList[m_nSelectNum].get())
	{
		//クリア状態のボタンには何もできない
		if (m_pKeyPadList[m_nSelectNum]->GetKeyPadState() == CModule_Parts_No0_SymbolKey::KEYPAD_STATE::CLEAR)
		{
			return;
		}

		//音再生
		CManager::GetSound()->Play(CSound::LABEL_SE_MODULE_PUSH);

		//次のシンボルと同じシンボルだった時
		if (m_nNextSymbolNum == m_pKeyPadList[m_nSelectNum]->GetSymbolNum())
		{
			m_pKeyPadList[m_nSelectNum]->SetKeypadState(CModule_Parts_No0_SymbolKey::KEYPAD_STATE::CLEAR);
			m_nNextSymbolNum++;

			//クリアしたかチェック
			CheckClear();
		}
		else
		{	//失敗
			m_pKeyPadList[m_nSelectNum]->SetKeypadState(CModule_Parts_No0_SymbolKey::KEYPAD_STATE::FAILED);
			CModule_Base::Module_Failed();
		}
	}
}

//------------------------------------------------------------------------------
//モジュールの選択解除
//------------------------------------------------------------------------------
void CModule_No0_SymbolKeyPad::ModuleCancel()
{
	//選択解除
	CModule_Base::ModuleParts_Select<CModule_Parts_No0_SymbolKey>(m_pKeyPadList, -1);
}

//------------------------------------------------------------------------------
//キーパッド生成
//------------------------------------------------------------------------------
void CModule_No0_SymbolKeyPad::CreateKeyPad(ANSWER_PATTERN answer)
{
	//シンボルのナンバーのリスト
	Vec<int> nAllSymbolPatternList(24);
	Vec<int> nSymbolNumList = {};

	//初期化
	for (int nCnt = 0; nCnt < (int)nAllSymbolPatternList.size(); nCnt++)
	{
		nAllSymbolPatternList[nCnt] = nCnt;
	}

	//答え設定
	//Answerの番号から4つ抜き取る
	for (int nCnt = 0; nCnt < KEYPAD_NEED_CLEAR_KEY_NUM; nCnt++)
	{
		//イテレータ取得
		auto itr = std::find_if(nAllSymbolPatternList.begin(), nAllSymbolPatternList.end(),
			[nCnt, answer](int &rValue) {return (rValue == nCnt + answer * KEYPAD_NEED_CLEAR_KEY_NUM); });

		//イテレータがend以外だった時
		if (itr != nAllSymbolPatternList.end())
		{
			//配列に追加
			nSymbolNumList.emplace_back(*itr);

			//使った番号は削除
			nAllSymbolPatternList.erase(itr);
		}
	}

	//9個になるまで繰り返す
	while (nSymbolNumList.size() < MAX_KEYPAD)
	{
		//ランダムで数字設定
		int nNum = rand() % nAllSymbolPatternList.size();

		//ランダムで番号設定
		nSymbolNumList.emplace_back(nAllSymbolPatternList[nNum]);

		//使った番号は削除
		nAllSymbolPatternList.erase(nAllSymbolPatternList.begin() + nNum);
	}

	//シンボルをシャッフル
	CHossoLibrary::Vec_Shuffle(nSymbolNumList);

	//全てのキーに割り当てる
	for (size_t nCnt = 0; nCnt < nSymbolNumList.size(); nCnt++)
	{
		//座標計算用
		INTEGER2 Value;
		Value.nX = nCnt % 3;
		Value.nY = nCnt / 3;

		//キー生成
		m_pKeyPadList.emplace_back(CModule_Parts_Base::Create_ModuleParts<CModule_Parts_No0_SymbolKey>
			(KEYPAD_OFFSET + D3DXVECTOR3(	-KEYPAD_INTERVAL.x + KEYPAD_INTERVAL.x * Value.nX,
											KEYPAD_INTERVAL.y - KEYPAD_INTERVAL.y * Value.nY, 0.0f), GetMtxWorldPtr()));

		//シンボル設定
		m_pKeyPadList[m_pKeyPadList.size() - 1]->SetSymbol(nSymbolNumList[nCnt]);

	}
}

//------------------------------------------------------------------------------
//クリアしたかチェック
//------------------------------------------------------------------------------
void CModule_No0_SymbolKeyPad::CheckClear()
{
	//クリアに必要なキー数に達したかどうか
	if (std::count_if(m_pKeyPadList.begin(), m_pKeyPadList.end(),
		[](S_ptr<CModule_Parts_No0_SymbolKey> &vec) {return (vec->GetClearFlag()); }) >= KEYPAD_NEED_CLEAR_KEY_NUM)
	{
		//モジュールクリア
		CModule_Base::Module_Clear();

		//次の答えを消しとく
		m_nNextSymbolNum = -1;
	}
}