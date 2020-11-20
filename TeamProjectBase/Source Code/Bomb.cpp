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

	const D3DXVECTOR3 &CameraRot = CManager::GetRenderer()->GetCamera()->GetCameraRot();

	fabsf(CameraRot.y) >= D3DX_PI * 0.5f ?
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
S_ptr<CBomb> CBomb::CreateBomb(D3DXVECTOR3 const pos, D3DXVECTOR3 const rot, int const nModuleNum)
{
	//メモリ確保
	S_ptr<CBomb> pBomb = std::make_shared<CBomb>();

	//初期化
	pBomb->Init();

	//座標とサイズ設定
	pBomb->SetPos(pos);
	pBomb->SetRot(rot);

	//モデル情報設定
	pBomb->BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_BOMBBOX));

	//モジュール生成
	pBomb->CreateModule(nModuleNum);

	//ボムの外装生成
	pBomb->m_pBombExterior = CBomb_Exterior::CreateBombExterior(pBomb->GetMtxWorldPtr());

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
void CBomb::CreateModule(int const nModuleNum)
{
	//モデル数代入
	m_nModuleNum = nModuleNum;

	//もしモジュールを表示できる範囲外の時は収める
	CHossoLibrary::RangeLimit_Equal(m_nModuleNum, 0, MAX_MODULE_NUM);

	//モジュールランダム生成
	//CreateModule_Random();

//Debug用
#ifdef _DEBUG
	CreateModuleDebug();
#endif //_DEBUG

	////1番目
	//CBomb::CreateModuleOne<CModule_Timer>();
	////2番目
	//CBomb::CreateModuleOne<CModule_No0_SymbolKeyPad>();
	////3番目
	//CBomb::CreateModuleOne<CModule_No1_ShapeKeyPad>();
	////4番目
	//CBomb::CreateModuleOne<CModule_None>();
	////5番目
	//CBomb::CreateModuleOne<CModule_None>();
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

	//モジュールが入らない分はNONEのモジュールを入れておく
	while ((int)LocalList.size() < MAX_MODULE_NUM - m_nModuleNum - 1)
	{
		LocalList.emplace_back(CModule_Base::MODULE_TYPE::NONE);
	}
	//最大数になるまでモジュールを入れる
	while (LocalList.size() < MAX_MODULE_NUM - 1)
	{
		//Buttonから4Buttonまでのランダム
		LocalList.emplace_back((CModule_Base::MODULE_TYPE)CHossoLibrary::RandomRangeUnsigned((int)CModule_Base::MODULE_TYPE::NO0_SYMBOL, (int)CModule_Base::MODULE_TYPE::MAX));
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
