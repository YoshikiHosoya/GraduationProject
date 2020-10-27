//------------------------------------------------------------------------------
//
//モジュールの図形のキーパッド処理  [module_ShapeKeypad.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "module_No2_ShapeKeypad.h"
#include "module_parts_No2_ShapeKey.h"
#include "renderer.h"
#include "manager.h"
#include "modelinfo.h"
#include "timer.h"
#include "keyboard.h"
#include "game.h"
#include "Mylibrary.h"
#include "scene3D.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define KEYPAD_NUM						(4)									//パッドの総数
#define KEYPAD_INTERVAL					(D3DXVECTOR3(25.0f,0.0f,0.0f))		//パッドの間隔
#define KEYPAD_OFFSET					(D3DXVECTOR3(0.0f,-15.0f,-20.0f))	//パッドのオフセット座標
#define DISPLAY_SHAPE_OFFSET			(D3DXVECTOR3(-7.5f,20.0f,-25.0f))	//ディスプレイの記号のオフセット間隔
#define DISPLAY_SHAPE_SIZE				(D3DXVECTOR3(20.0f,20.0f,0.0f))		//ディスプレイの記号のサイズ


//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModule_No2_ShapeKeyPad::CModule_No2_ShapeKeyPad()
{
	m_pKeyPadList = {};
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModule_No2_ShapeKeyPad::~CModule_No2_ShapeKeyPad()
{
	m_pKeyPadList.clear();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModule_No2_ShapeKeyPad::Init()
{
	//モデル情報設定
	BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MODULE_NO2));

	//ランプ生成
	CModule_Base::CreateLamp();

	////答えのパターンをランダムで設定
	//m_Answer = (CModule_No2_ShapeKeyPad::ANSWER_PATTERN)(rand() & CModule_No2_ShapeKeyPad::ANSWER_6);

	//キーパッド生成
	CreateKeyPad();

	//ディスプレイの記号生成
	m_pDisplayShape = CSceneBase::ScenePolygonCreateSelfManagement<CScene3D>
		(DISPLAY_SHAPE_OFFSET, DISPLAY_SHAPE_SIZE, GreenColor, CTexture::GetSeparateTexture(CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01));

	//親マトリックス設定
	m_pDisplayShape->SetParentMtxPtr(GetMtxWorldPtr());


	ShapeShuffle();
	//ＵＶ設定

	////次の答え設定
	//m_nNextSymbolNum = m_Answer * KEYPAD_NEED_CLEAR_KEY_NUM;

	CSceneX::Init();
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::Uninit()
{
	CSceneX::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::Update()
{
	CSceneX::Update();

	//nullcheck
	if (m_pDisplayShape)
	{
		m_pDisplayShape->Update();
	}
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::Draw()
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
void CModule_No2_ShapeKeyPad::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}
//------------------------------------------------------------------------------
//キーパッド操作
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::Operation()
{
	static int nSelectNum = 0;

	int nSelectNumOld = nSelectNum;
	int nCntClearPad = 0;

	//入力が無かった時はbreak
	CHossoLibrary::Selecting(nSelectNum, nSelectNumOld, 4, 1);

	for (size_t nCnt = 0; nCnt < m_pKeyPadList.size(); nCnt++)
	{
		//nullcheck
		if (m_pKeyPadList[nCnt].get())
		{
			//現在の選択番号と同じモノだけtrueにしておく
			nCnt == nSelectNum ?
				m_pKeyPadList[nCnt]->SetSelect(true) :
				m_pKeyPadList[nCnt]->SetSelect(false);
		}
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN))
	{
		//if (m_pKeyPadList[nSelectNum].get())
		//{
		//	//次のシンボルと同じシンボルだった時
		//	if (m_nNextSymbolNum == m_pKeyPadList[nSelectNum]->GetSymbolNum())
		//	{
		//		m_pKeyPadList[nSelectNum]->SetKeypadState(CModule_Parts_No1_SymbolKey::KEYPAD_STATE::CLEAR);
		//		m_nNextSymbolNum++;

		//		//クリアしたかチェック
		//		CheckClear();
		//	}
		//	else
		//	{	//失敗
		//		m_pKeyPadList[nSelectNum]->SetKeypadState(CModule_Parts_No1_SymbolKey::KEYPAD_STATE::FAILED);
		//		CModule_Base::Module_Failed();
		//	}
		//}

		//クリアしたかチェック
		CheckClear();

	}

	//nullcheck
	if (CManager::GetKeyboard()->GetTrigger(DIK_BACKSPACE))
	{
		for (size_t nCnt = 0; nCnt < m_pKeyPadList.size(); nCnt++)
		{
			if (m_pKeyPadList[nCnt].get())
			{
				//現在の選択番号と同じモノだけtrueにしておく
				m_pKeyPadList[nCnt]->SetSelect(false);

				CManager::GetGame()->SetGaze(CGame::GAZE_BOMB);
			}
		}
	}

}

//------------------------------------------------------------------------------
//キーパッド生成
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::CreateKeyPad()
{
	//全てのキーに割り当てる
	for (size_t nCnt = 0; nCnt < KEYPAD_NUM; nCnt++)
	{
		//キー生成
		m_pKeyPadList.emplace_back(CModule_Parts_Base::Create_ModuleParts<CModule_Parts_No2_ShapeKey>
			(KEYPAD_OFFSET + D3DXVECTOR3(-((KEYPAD_INTERVAL.x) * (KEYPAD_NUM / 2)) + (KEYPAD_INTERVAL.x * 0.5f) + (KEYPAD_INTERVAL.x * nCnt), 0.0f, 0.0f), GetMtxWorldPtr()));

		m_pKeyPadList[m_pKeyPadList.size() - 1]->SetShape(nCnt);
	}
}

//------------------------------------------------------------------------------
//クリアしたかチェック
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::CheckClear()
{
	////クリアに必要なキー数に達したかどうか
	//if (std::count_if(m_pKeyPadList.begin(), m_pKeyPadList.end(),
	//	[](std::shared_ptr<CModule_Parts_No1_SymbolKey> &vec) {return (vec->GetClearFlag()); }) >= KEYPAD_NEED_CLEAR_KEY_NUM)
	//{
		//モジュールクリア
		CModule_Base::Module_Clear();
	//}
}
//------------------------------------------------------------------------------
//図形をシャッフル
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::ShapeShuffle()
{
	//番号が入ってる配列
	std::vector<int> nShapeList = { 0, 1, 2, 3 };

	//番号シャッフル
	CHossoLibrary::Vec_Shuffle(nShapeList);

	for (size_t nCnt = 0; nCnt < nShapeList.size(); nCnt++)
	{
		//図形設定
		m_pKeyPadList[nCnt]->SetShape(nShapeList[nCnt]);
	}

	SetDisplayShape(rand() % KEYPAD_NUM);

}
//------------------------------------------------------------------------------
//アニメーション設定
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::SetDisplayShape(int nShape)
{
	m_pDisplayShape->SetAnimation(CHossoLibrary::CalcUV_StaticFunc(nShape, CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01),
		CTexture::GetSparateTex_UVSize(CTexture::SEPARATE_TEX_MODULEPARTS_MODULE01));
}

//------------------------------------------------------------------------------
//次の押すキー設定
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::SetNextPushKey()
{


}

//------------------------------------------------------------------------------
//次の押すキー設定　図形を基に設定
//------------------------------------------------------------------------------
void CModule_No2_ShapeKeyPad::SetNextPushKey_FromShape(SHAPE shape)
{
	//入力されたshapeと同じキー検索
	auto itr = std::find_if(m_pKeyPadList.begin(), m_pKeyPadList.end(),
		[shape](std::shared_ptr<CModule_Parts_No2_ShapeKey> pPtr) {return (SHAPE)pPtr->GetShape() == shape; });

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
void CModule_No2_ShapeKeyPad::SetNextPushKey_FromPlace(int nPlace)
{
	m_pKeyPadList[nPlace]->SetClearFlag(true);
}
