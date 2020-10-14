//-----------------------------------------------------------------------------
//
// モードの規定クラス [basemode.cpp]
// Author : Yoshiki Hosoya
//
//-----------------------------------------------------------------------------
#include "basemode.h"
#include "particle.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "keyboard.h"
#include "sound.h"
#include "modelinfo.h"
//-----------------------------------------------------------------------------
//デバッグコマンド
//-----------------------------------------------------------------------------
void CBaseMode::DebugCommand()
{
	//キーボード取得
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	//使い方説明
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "--------------------Debug Command--------------------\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[F3] : 一時停止\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[F4] : 一時停止中に1Fだけ更新\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[F5] : DebugProc表記 on/off\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[F6] : ImGui表記 on/off\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[F8] : ミュート on/off\n");

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[Ctrl] + テンキー [0] : 現在のModeに遷移\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[Ctrl] + テンキー [1] : Titleに遷移\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[Ctrl] + テンキー [2] : Gameに遷移\n");
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "[Ctrl] + テンキー [4] : Resultに遷移\n");
	//CDebugProc::Print(CDebugProc::PLACE_LEFT, "[Ctrl] + テンキー [7] : MapEditorに遷移\n");
	//CDebugProc::Print(CDebugProc::PLACE_LEFT, "[Ctrl] + テンキー [8] : EffectViewerに遷移\n");
	//CDebugProc::Print(CDebugProc::PLACE_LEFT, "[Ctrl] + テンキー [9] : MotionViewerに遷移\n");


	//一時停止
	if (pKeyboard->GetTrigger(DIK_F3))
	{
		CScene::SetStop(CScene::GetStop() ^ 1);
	}

	//1Fだけ更新
	if (pKeyboard->GetTrigger(DIK_F4))
	{
		CScene::Set1FAction();
	}

	//デバッグ表記on/off
	if (pKeyboard->GetTrigger(DIK_F5))
	{
		CManager::GetRenderer()->SetDispDebugProcInfo(CManager::GetRenderer()->CheckShowDebugProcInfo() ^ 1);
	}

	//ImGui表記on/off
	if (pKeyboard->GetTrigger(DIK_F6))
	{
		CManager::GetRenderer()->SetDispImGuiInfo(CManager::GetRenderer()->CheckShowImGuiInfo() ^ 1);
	}

	//ミュート表記on/off
	if (pKeyboard->GetTrigger(DIK_F8))
	{
		CManager::GetSound()->SetMute(CManager::GetSound()->GetMute() ^ 1);
	}

	//Ctrl押しながら
	if (pKeyboard->GetPress(DIK_LCONTROL))
	{
		//現在のモード再始動
		if (pKeyboard->GetTrigger(DIK_NUMPAD0))
		{
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::GetMode());
		}
		//タイトル
		if (pKeyboard->GetTrigger(DIK_NUMPAD1))
		{
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_TITLE);
		}
		//ゲーム
		if (pKeyboard->GetTrigger(DIK_NUMPAD2))
		{
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_GAME);
		}
		//リザルト
		if (pKeyboard->GetTrigger(DIK_NUMPAD4))
		{
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RESULT);
		}

		////エフェクトビューワ
		//if (pKeyboard->GetTrigger(DIK_NUMPAD8))
		//{
		//	CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_TITLE);
		//}

	}

	//デバッグ情報表記
	//オーバーライド用　それぞれのモードで処理
	ShowDebugInfo();
}
//-----------------------------------------------------------------------------
//使うテクスチャやモデルの読み込み
//-----------------------------------------------------------------------------
void CBaseMode::BaseLoad(HWND hWnd)
{
	//テクスチャロード
	CTexture::TexLoad(hWnd);
	CModelInfo::ModelLoad(hWnd);

	//パーティクルのテクスチャと頂点確報
	if (FAILED(CParticle::MakeVertex()))
	{
		//失敗
		MessageBox(hWnd, "テクスチャ読み込み失敗", "CParticle", MB_OK | MB_ICONHAND);
	}
}

//-----------------------------------------------------------------------------
//生成したものを全て破棄
//----------------------------------------------------------------------------
void CBaseMode::BaseUnload()
{
	//テクスチャ破棄
	CTexture::TexUnload();

	//パーティクルの頂点情報開放
	CParticle::ReleaseVertex();

}
