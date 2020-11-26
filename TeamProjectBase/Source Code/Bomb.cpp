//------------------------------------------------------------------------------
//
//爆弾の処理  [Bomb.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "Bomb.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "keyboard.h"
#include "mouse.h"
#include "game.h"
#include "modelinfo.h"
#include "Bomb_Exterior.h"
#include "modelinfo.h"
#include "module_none.h"
#include "module_timer.h"
#include "module_No0_SymbolKeyPad.h"
#include "module_No1_ShapeKeypad.h"
#include "module_No2_LampAndWire.h"
#include "module_No4_4ColButton.h"
#include "sound.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
#ifdef _DEBUG
bool CBomb::m_bCanExplosion = false;
#endif // _DEBUG


//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define MODULE_NUM_EASY		(2)
#define MODULE_NUM_NORMAL	(3)
#define MODULE_NUM_HARD		(5)


//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CBomb::CBomb()
{
	m_nModuleNum = 0;
	m_nSelectModuleNum = 0;
	m_pModuleList = {};
	m_pBombExterior.reset();
	m_bCameraDir = true;
	m_RotDest = GetRot();
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CBomb::~CBomb()
{
	m_pModuleList = {};
	m_pBombExterior.reset();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CBomb::Init()
{
	CSceneX::Init();
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CBomb::Update()
{
	CSceneX::Update();

	fabsf(GetRot().y) >= D3DX_PI * 0.5f ?
		m_bCameraDir = true :
		m_bCameraDir = false;

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "bCamera >> %d\n", m_bCameraDir);

	int nMin = 6 * (m_bCameraDir);
	int nMax = nMin + 5;

	//裏表切り替わった時
	if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
	{
		SearchHeadCanSelectNum(nMin);
		CManager::GetGame()->SetGaze(CGame::GAZE_BOMB);
	}


	//キーボード操作
	//CBomb::Operation_Keyboard();

	//マウス操作
	CBomb::Operation_Mouse();

	//////std::cout << typeid(m_pModuleList[0].get());
	//std::cout << "m_pModuleList[0]" << "Name >> " << typeid(m_pModuleList[0]).name() << NEWLINE;
	//std::cout << "m_pModuleList[0]" << "RawName >> " << typeid(m_pModuleList[0]).raw_name() << NEWLINE;

	//std::cout << "m_pModuleList[0].get()" << "Name >> " <<typeid(m_pModuleList[0].get()).name() << NEWLINE;
	//std::cout << "m_pModuleList[0].get()" << "RawName > " << typeid(m_pModuleList[0].get()).raw_name() << NEWLINE;

	//std::cout << "*m_pModuleList[0]" << "Name >> " << typeid(*m_pModuleList[0]).name() << NEWLINE;
	//std::cout << "*m_pModuleList[0]" << "RawName >> " << typeid(*m_pModuleList[0]).raw_name() << NEWLINE;

	//std::cout << "*m_pModuleList[0].get()" << "Name >> " << typeid(*m_pModuleList[0].get()).name() << NEWLINE;
	//std::cout << "*m_pModuleList[0].get()" << "RawName >> " << typeid(*m_pModuleList[0].get()).raw_name() << NEWLINE;

	//std::cout << NEWLINE;
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CBomb::Draw()
{
	CSceneX::Draw();

}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CBomb::ShowDebugInfo()
{
#ifdef _DEBUG

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "-------------操作方法----------------\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[↑][↓][←][→] カーソル移動 \n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[Enter] 決定ボタン \n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[BackSpace] 戻るボタン \n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[F7] 爆発するかどうか設定 >> [%d] \n",m_bCanExplosion);


	ImGui::DragFloat3("BombPos", GetPos());

	if (CManager::GetKeyboard()->GetTrigger(DIK_F7))
	{
		m_bCanExplosion ^= 1;
	}

#endif //DEBUG
}

//------------------------------------------------------------------------------
//生成関数
//------------------------------------------------------------------------------
S_ptr<CBomb> CBomb::CreateBomb(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot, DIFFICULTY const difficulty)
{
	//メモリ確保
	S_ptr<CBomb> pBomb = std::make_shared<CBomb>();

	//初期化
	pBomb->Init();

	//座標とサイズ設定
	pBomb->SetPos(pos);
	pBomb->SetRot(rot);

	pBomb->m_difficulty = difficulty;

	//モデル情報設定
	pBomb->BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_BOMBBOX));

	//ボムの外装生成
	pBomb->m_pBombExterior = CBomb_Exterior::CreateBombExterior(pBomb->GetMtxWorldPtr());

	//モジュール生成
	pBomb->CreateModule();


	//Scene側で管理
	pBomb->SetObjType(CScene::OBJTYPE_BOMB);
	pBomb->AddSharedList(pBomb);

	return pBomb;
}

//------------------------------------------------------------------------------
//キーボード操作
//------------------------------------------------------------------------------
void CBomb::Operation_Keyboard()
{
	//配列が空か選択番号が-1だった場合
	if (m_pModuleList.empty() || m_nSelectModuleNum < 0)
	{
		//return
		return;
	}

	//キーボードのポインタ
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//Gameの視点によって判定の相手を変える
	switch (CManager::GetGame()->GetGaze())
	{
	case CGame::GAZE_DEFAULT:
		break;

	case CGame::GAZE_BOMB:

		//モジュール選択
		ModuleSelect();

		for (int nCnt = 0; nCnt < (int)m_pModuleList.size(); nCnt++)
		{
			//nullcheck
			if (m_pModuleList[nCnt].get())
			{
				//現在の選択番号と同じモノだけtrueにしておく
				nCnt == m_nSelectModuleNum ?
					m_pModuleList[nCnt]->SetSelect(true) :
					m_pModuleList[nCnt]->SetSelect(false);
			}
		}


		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			//モジュール注視
			CManager::GetGame()->SetGaze(CGame::GAZE::GAZE_MODULE);

			//カメラを近づける
			m_pModuleList[m_nSelectModuleNum]->CameraApproach();
		}

		break;

	case CGame::GAZE_MODULE:
		for (auto & rPtr : m_pModuleList)
		{
			//選択解除
			rPtr->SetSelect(false);
			rPtr->SetOperating(false);
		}

		//nullcheck
		if (m_pModuleList[m_nSelectModuleNum].get())
		{

			//現在選択されているモジュールを操作
			m_pModuleList[m_nSelectModuleNum]->Operation_Keyboard();
		}

		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------
//マウス操作
//------------------------------------------------------------------------------
void CBomb::Operation_Mouse()
{

	Operation_Camera();

	switch (CManager::GetGame()->GetGaze())
	{

		//通常時
	case CGame::GAZE_DEFAULT:

		break;

		//爆弾を見てる時
	case CGame::GAZE_BOMB:

		//レイの判定
		//裏と表で判定の範囲を変更
		CHossoLibrary::RayCollision_ModuleSelect(m_pModuleList.begin() + (m_bCameraDir * 6), m_pModuleList.end() - ((!m_bCameraDir) * 6), m_nSelectModuleNum);

		//何も選択されてない時はbreak
		if (m_nSelectModuleNum < 0)
		{
			break;
		}

		//裏表反転
		m_nSelectModuleNum += (m_bCameraDir * 6);

		//マウスクリックされた時
		if (CManager::GetMouse()->GetTrigger(0))
		{
			//モジュール注視
			CManager::GetGame()->SetGaze(CGame::GAZE::GAZE_MODULE);

			//回転を元に戻す
			m_RotDest = ZeroVector3 + D3DXVECTOR3(0.0f, m_bCameraDir * D3DX_PI, 0.0f);

			//選択状態解除
			m_pModuleList[m_nSelectModuleNum]->SetSelect(false);

			//カメラ近づける
			m_pModuleList[m_nSelectModuleNum]->CameraApproach();
		}
		break;

		//モジュールを見ているとき
	case CGame::GAZE_MODULE:
		//nullcheck
		if (m_pModuleList[m_nSelectModuleNum].get())
		{
			//現在選択されているモジュールを操作
			m_pModuleList[m_nSelectModuleNum]->Operation_Mouse();
		}
		break;
	}


}

//-------------------------------------------------------------------------------------------------------------
// 操作
//-------------------------------------------------------------------------------------------------------------
void CBomb::Operation_Camera()
{

	// キーボードの取得
	//Ckeyboard *pKeyboard = &CManager::GetKeyboard();

	// マウスの取得
	CMouse *pMouse = CManager::GetMouse();
	// マウスの状態を取得
	DIMOUSESTATE2* pMouseState = &pMouse->GetMouseState();


	static D3DXVECTOR3 m_MouseRotSave = ZeroVector3;

	D3DXVECTOR2 NewRotation = D3DXVECTOR2(0.0f, 0.0f);

	if (pMouse->GetPress(1) == true)
	{
		// ヨー回転
		NewRotation.y = ((float)pMouseState->lX) / (D3DX_PI*2.0f) *0.02f;
		m_RotDest.y -= NewRotation.y;

		// 回転量を360度ないに直す
		CMylibrary::SetFixTheRotation(&m_RotDest.y);

		// ピッチロー回転
		NewRotation.x = ((float)pMouseState->lY) / (D3DX_PI*2.0f) *0.02f;

		// 回転を90度未満に抑える
		if (NewRotation.x >= D3DX_PI*0.49f)
		{
			NewRotation.x = D3DX_PI*0.49f;
		}
		else if (NewRotation.x <= -D3DX_PI*0.49f)
		{
			NewRotation.x = -D3DX_PI*0.49f;
		}
		m_RotDest.x -= NewRotation.x;
	}

	if (pMouse->GetRelease(1))
	{
		m_MouseRotSave.y = m_RotDest.y;
		m_MouseRotSave.x = m_RotDest.x;
	}

	//// カメラの公転
	//if (pKeyboard->GetPress(DIK_RIGHTARROW))
	//{
	//	m_rot.y += CAMERA_ROTATION_SPEED;
	//}
	//else if (pKeyboard->GetPress(DIK_LEFTARROW))
	//{
	//	m_rot.y -= CAMERA_ROTATION_SPEED;
	//}
	//if (pKeyboard->GetPress(DIK_UPARROW))
	//{
	//	m_rot.x += CAMERA_ROTATION_SPEED*0.3f;
	//}
	//else if (pKeyboard->GetPress(DIK_DOWNARROW))
	//{
	//	m_rot.x -= CAMERA_ROTATION_SPEED*0.3f;
	//}
	//// 距離の倍率変更
	//if (pKeyboard->GetPress(DIK_4))
	//{
	//	m_fMagnificat += 0.01f;
	//}
	//else if (pKeyboard->GetPress(DIK_5))
	//{
	//	m_fMagnificat -= 0.01f;
	//}
	//else if (pKeyboard->GetPress(DIK_1))
	//{
	//	m_fMagnificat = 1.0f;
	//}
	//else if (pKeyboard->GetPress(DIK_2))
	//{
	//	m_fMagnificat = CAMERA_MAGNIFICAT_MAX;
	//}
	//else if (pKeyboard->GetPress(DIK_3))
	//{
	//	m_fMagnificat = CAMERA_MAGNIFICAT_MIN;
	//}

	//// 拡大率の制限
	//if (m_fMagnificat >= CAMERA_MAGNIFICAT_MIN)
	//{
	//	m_fMagnificat = CAMERA_MAGNIFICAT_MIN;
	//}
	//else if (m_fMagnificat <= CAMERA_MAGNIFICAT_MAX)
	//{
	//	m_fMagnificat = CAMERA_MAGNIFICAT_MAX;
	//}

	// カメラの回転を90度未満に抑える
	if (m_RotDest.x >= D3DX_PI*0.49f)
	{
		m_RotDest.x = D3DX_PI*0.49f;
	}
	else if (m_RotDest.x <= -D3DX_PI*0.49f)
	{
		m_RotDest.x = -D3DX_PI*0.49f;
	}
	// 回転量を360度ないに直す
	CHossoLibrary::CalcRotation(m_RotDest.y);

	//回転の差分
	D3DXVECTOR3 rotDiff = m_RotDest - GetRot();

	//3.14の中に収める
	CHossoLibrary::CalcRotation_XYZ(rotDiff);

	//ちょっとづつ回転
	GetRot() += (rotDiff * 0.15f);

}

//------------------------------------------------------------------------------
//モジュールクリアしたかチェック
//------------------------------------------------------------------------------
void CBomb::ModuleClearCheck()
{
	//クリアフラグの個数がモジュール数以上になった時
	if (std::count_if(m_pModuleList.begin(), m_pModuleList.end(),
		[](S_ptr<CModule_Base> pModule) {return pModule->GetModuleClearFlag(); }) >= m_nModuleNum)
	{
		//ゲームクリア
		CManager::GetGame()->SetState(CGame::STATE_GAMECLEAR);
	}
}

//------------------------------------------------------------------------------
//モジュールミスる
//------------------------------------------------------------------------------
void CBomb::ModuleMiss()
{
#ifdef _DEBUG
	if (!m_bCanExplosion) return;
#endif // _DEBUG

	//タイマーのクラスのイテレータ取得
	auto itr = std::find_if(m_pModuleList.begin(), m_pModuleList.end(),
		[](S_ptr<CModule_Base> const ptr) {return typeid(*ptr.get()) == typeid(CModule_Timer); });

	//イテレータが入ったかチェック
	if (itr != m_pModuleList.end())
	{
		//タイマー型に変換
		CModule_Timer *pTimer = dynamic_cast<CModule_Timer*>(itr->get());  // ダイナミックキャスト

		//ミスカウントアップ
		//全部ミスしたとき
		if (pTimer->MissCountUp())
		{
			//ゲームオーバー
			CManager::GetGame()->SetState(CGame::STATE_GAMEOVER);
		}
		else
		{
			//音再生
			CManager::GetSound()->Play(CSound::LABEL_SE_MISS);
		}
	}
}

//------------------------------------------------------------------------------
//モジュール生成
//------------------------------------------------------------------------------
void CBomb::CreateModule()
{

	//もしモジュールを表示できる範囲外の時は収める
	CHossoLibrary::RangeLimit_Equal(m_nModuleNum, 0, MAX_MODULE_NUM);

	//モジュールランダム生成
	CreateModule_Random();

//Debug用
#ifdef _DEBUG
	//CreateModuleDebug();
#endif //_DEBUG


	////モデル数代入
	//m_nModuleNum = 4;


	////1番目
	//CBomb::CreateModuleOne<CModule_Timer>();
	////2番目
	//CBomb::CreateModuleOne<CModule_No0_SymbolKeyPad>();
	////3番目
	//CBomb::CreateModuleOne<CModule_No1_ShapeKeyPad>();
	////4番目
	//CBomb::CreateModuleOne<CModule_No2_LampAndWire>();
	////5番目
	//CBomb::CreateModuleOne<CModule_No4_4ColButton>();
	////6番目
	//CBomb::CreateModuleOne<CModule_None>();

	////7番目
	//CBomb::CreateModuleOne<CModule_None>();
	////8番目
	//CBomb::CreateModuleOne<CModule_None>();
	////9番目
	//CBomb::CreateModuleOne<CModule_None>();
	////10番目
	//CBomb::CreateModuleOne<CModule_None>();
	////11番目
	//CBomb::CreateModuleOne<CModule_None>();
	////12番目
	//CBomb::CreateModuleOne<CModule_None>();


	//生成したリスト全てに
	for (auto &ptr : m_pModuleList)
	{
		//ボムのポインタ設定
		ptr->SetBombPtr(shared_from_this());
	}

	//選択可能なモジュール検索
	SearchHeadCanSelectNum(0);
}

//------------------------------------------------------------------------------
//モジュール生成　ランダム生成
//------------------------------------------------------------------------------
void CBomb::CreateModule_Random()
{

	//ローカルのリスト
	Vec<CModule_Base::MODULE_TYPE> LocalList = {};

	//タイマー生成
	CBomb::CreateModuleOne<CModule_Timer>();

	switch (m_difficulty)
	{
	case CBomb::EASY:
		m_nModuleNum = MODULE_NUM_EASY;
		LocalList.emplace_back(CModule_Base::MODULE_TYPE::NO0_SYMBOL);
		LocalList.emplace_back(CModule_Base::MODULE_TYPE::NO1_SHAPE);

		break;

	case CBomb::NORMAL:
		m_nModuleNum = MODULE_NUM_NORMAL;

		break;

	case CBomb::HARD:
		m_nModuleNum = MODULE_NUM_HARD;
		break;
	}

	//モジュール数に達するまでランダムでモジュール設定
	while ((int)LocalList.size() < m_nModuleNum)
	{
		//Buttonから4Buttonまでのランダム
		LocalList.emplace_back((CModule_Base::MODULE_TYPE)CHossoLibrary::RandomRangeUnsigned((int)CModule_Base::MODULE_TYPE::NO0_SYMBOL, (int)CModule_Base::MODULE_TYPE::MAX));
	}

	//最大数分までNONEのモジュールを入れておく
	while (LocalList.size() < MAX_MODULE_NUM - 1)
	{
		LocalList.emplace_back(CModule_Base::MODULE_TYPE::NONE);
	}

	//要素のシャッフル
	CHossoLibrary::Vec_Shuffle(LocalList);

	//モジュール生成
	for(auto const &ModuleType : LocalList)
	{
		//モジュールタイプに応じて生成
		switch (ModuleType)
		{
			//モジュール無し
		case CModule_Base::MODULE_TYPE::NONE:
			CBomb::CreateModuleOne<CModule_None>();

			break;

			//シンボル
		case CModule_Base::MODULE_TYPE::NO0_SYMBOL:
			CBomb::CreateModuleOne<CModule_No0_SymbolKeyPad>();

			break;

			//図形
		case CModule_Base::MODULE_TYPE::NO1_SHAPE:
			CBomb::CreateModuleOne<CModule_No1_ShapeKeyPad>();

			break;

			//ワイヤー
		case CModule_Base::MODULE_TYPE::NO2_WIRE:
			CBomb::CreateModuleOne<CModule_No2_LampAndWire>();

			break;

			//4色ボタン
		case CModule_Base::MODULE_TYPE::NO4_4COLBUTTON:
			CBomb::CreateModuleOne<CModule_No4_4ColButton>();

			break;
		}
	}
}

//------------------------------------------------------------------------------
//モジュール選択
//選択不可なモジュールがあるのでライブラリにあるやつとは別関数
//------------------------------------------------------------------------------
void CBomb::ModuleSelect()
{
	//選択番号
	//1F前の選択番号
	int m_SelectNumOld = m_nSelectModuleNum;


	//キーボードのポインタ
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	int nNumX = 3;
	int nNumY = 2;

	int nMin = (nNumX * nNumY) * m_bCameraDir;
	int nMax = nMin + (nNumX * nNumY) - 1;

	//モジュール選択処理
	//入力が無かった時はbreak
	//←→↑↓どれか入力されていた場合
	if (pKeyboard->GetTrigger(DIK_LEFT) || pKeyboard->GetTrigger(DIK_RIGHT) ||
		pKeyboard->GetTrigger(DIK_UP) || pKeyboard->GetTrigger(DIK_DOWN))
	{
		while (1)
		{
			//←
			if (pKeyboard->GetTrigger(DIK_LEFT))
			{
				//選択番号--
				m_nSelectModuleNum--;

				//範囲内に抑える
				if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
				{
					//元の値に戻す
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//列が変わるとき
				if (m_SelectNumOld / nNumX != m_nSelectModuleNum / nNumX)
				{
					//元の値に戻す
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//選択できる時
				if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
				{
					//break
					break;
				}
			}

			//→
			else if (pKeyboard->GetTrigger(DIK_RIGHT))
			{
				//選択番号++
				m_nSelectModuleNum++;


				//範囲内に抑える
				if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
				{
					//元の値に戻す
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//列が変わるとき
				if (m_SelectNumOld / nNumX != m_nSelectModuleNum / nNumX)
				{
					//元の値に戻す
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//選択できる時
				if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
				{
					//break
					break;
				}
			}

			//↑
			else if (pKeyboard->GetTrigger(DIK_UP))
			{
				//選択番号 -= 横幅分
				m_nSelectModuleNum -= nNumX;

				//範囲内に抑える
				if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
				{
					//元の値に戻す
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//選択可能なモジュールだった時
				if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
				{
					//break
					break;
				}
				else
				{
					//移動先の列の左端に戻る
					int nValue = m_nSelectModuleNum / nNumX;
					m_nSelectModuleNum = nValue * nNumX;

					while (m_nSelectModuleNum <= nValue * nNumX + nNumX)
					{
						//選択可能な時
						if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
						{
							//抜ける
							break;
						}

						//カウントアップ
						m_nSelectModuleNum++;
					}

					//次の列に突入してしまった場合
					if (nValue != m_nSelectModuleNum / nNumX)
					{
						//戻す
						m_nSelectModuleNum = m_SelectNumOld;
					}
					break;
				}
			}

			//↓
			else if (pKeyboard->GetTrigger(DIK_DOWN))
			{
				//選択番号 += 横幅分
				m_nSelectModuleNum += nNumX;

				//範囲内に抑える
				if (CHossoLibrary::RangeLimit_Equal(m_nSelectModuleNum, nMin, nMax))
				{
					//元の値に戻す
					m_nSelectModuleNum = m_SelectNumOld;
					break;
				}

				//選択可能なモジュールだった時
				if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
				{
					//break
					break;
				}

				else
				{
					//移動先の列の左端に戻る
					int nValue = m_nSelectModuleNum / nNumX;
					m_nSelectModuleNum = nValue * nNumX;

					while (m_nSelectModuleNum <= nValue * nNumX + nNumX)
					{
						//選択可能な時
						if (m_pModuleList[m_nSelectModuleNum]->GetCanModuleSelect())
						{
							//抜ける
							break;
						}

						//カウントアップ
						m_nSelectModuleNum++;
					}

					//次の列に突入してしまった場合
					if (nValue != m_nSelectModuleNum / nNumX)
					{
						//戻す
						m_nSelectModuleNum = m_SelectNumOld;
					}
					break;
				}

			}
		}
	}
}

//------------------------------------------------------------------------------
//選択可能なモジュール検索
//一番最初に生成したときと裏表切り替え時に使う
//------------------------------------------------------------------------------
void CBomb::SearchHeadCanSelectNum(int nStartNum)
{
	//最初の選択番号設定
	for (int nCnt = nStartNum; nCnt < (int)m_pModuleList.size(); nCnt++)
	{
		//選択可能だった時
		if (m_pModuleList[nCnt]->GetCanModuleSelect())
		{
			//現在の選択番号に設定
			m_nSelectModuleNum = nCnt;
			return;
		}
	}
	//選択可能なモジュールが存在しなかった場合
	m_nSelectModuleNum = -1;
}


#ifdef _DEBUG
//------------------------------------------------------------------------------
//モジュール生成　デバッグ用　自分でモジュールを決めれる
//------------------------------------------------------------------------------
void CBomb::CreateModuleDebug()
{
	//モジュール数をここで決める debug用
	m_nModuleNum = 3;

	//1番目
	CBomb::CreateModuleOne<CModule_Timer>();
	//2番目
	CBomb::CreateModuleOne<CModule_No0_SymbolKeyPad>();
	//3番目
	CBomb::CreateModuleOne<CModule_No1_ShapeKeyPad>();
	//4番目
	CBomb::CreateModuleOne<CModule_No2_LampAndWire>();
	//5番目
	CBomb::CreateModuleOne<CModule_No4_4ColButton>();
	//6番目
	CBomb::CreateModuleOne<CModule_None>();

	//7番目
	CBomb::CreateModuleOne<CModule_None>();
	//8番目
	CBomb::CreateModuleOne<CModule_No0_SymbolKeyPad>();
	//9番目
	CBomb::CreateModuleOne<CModule_No1_ShapeKeyPad>();
	//10番目
	CBomb::CreateModuleOne<CModule_No2_LampAndWire>();
	//11番目
	CBomb::CreateModuleOne<CModule_No4_4ColButton>();
	//12番目
	CBomb::CreateModuleOne<CModule_None>();

	//モジュール数確認
	m_nModuleNum = std::count_if(m_pModuleList.begin(), m_pModuleList.end(),
		[](S_ptr<CModule_Base> const &ptr) {return ptr->GetCanModuleSelect(); });

}
#endif //_DEBUG
