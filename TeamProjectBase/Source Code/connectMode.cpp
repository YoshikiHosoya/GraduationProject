//==========================================================================================================================================================
//
// 接続モード処理 [ connectMode.cpp ]
// Author : KANAN NAGANAWA
//
//==========================================================================================================================================================
#include "connectMode.h"
#include "connectBase.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "camera.h"
#include "client.h"
#include "chatBase.h"

//==========================================================================================================================================================
// マクロ定義
//==========================================================================================================================================================

//==========================================================================================================================================================
// 静的メンバ変数の初期化
//==========================================================================================================================================================

//==========================================================================================================================================================
// コンストラクタ
//==========================================================================================================================================================
CConnectMode::CConnectMode()
{
}

//==========================================================================================================================================================
// デストラクタ
//==========================================================================================================================================================
CConnectMode::~CConnectMode()
{

}

//==========================================================================================================================================================
// 初期化
//==========================================================================================================================================================
HRESULT CConnectMode::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	m_pConnectBase = CConnectBase::Create();
	// チャットの生成
	m_pChatBase = CChatBase::Create();

	return S_OK;
}

//==========================================================================================================================================================
// 更新
//==========================================================================================================================================================
void CConnectMode::Update()
{
	//なんかボタン押されたとき
	if (CHossoLibrary::CheckAnyButton())
	{
		//チュートリアルに遷移
		CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_GAME);
	}
}

//==========================================================================================================================================================
// 描画
//==========================================================================================================================================================
void CConnectMode::Draw()
{

}

//==========================================================================================================================================================
// ImGuiの更新
//==========================================================================================================================================================
void CConnectMode::ShowDebugInfo()
{
}
