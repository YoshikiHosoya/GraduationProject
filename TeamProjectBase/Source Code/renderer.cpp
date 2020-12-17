//------------------------------------------------------------------------------
//
//レンダラー処理  [renderer.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include <stdio.h>
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "camera.h"
#include "basemode.h"
#include "particle.h"
#include "light.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define BACK_COLOR (D3DCOLOR_RGBA(50, 50, 100, 0))
#define FOG_COLOR (D3DCOLOR_RGBA(0,0,0,0))

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
D3DXCOLOR CRenderer::m_BackColor = BACK_COLOR;

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CRenderer::CRenderer()
{
	//初期化
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_pCamera = nullptr;
	m_pDebugProc = nullptr;
	m_pLight = nullptr;
	m_bDispDebugProcInfo = false;
	m_bDispImGuiInfo = false;
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CRenderer::~CRenderer()
{
	if (m_pD3DDevice)
	{	//Direct3Dデバイスの開放
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	if (m_pD3D)
	{	//Direct3Dオブジェクトの開放
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
	m_pCamera.reset();
	m_pDebugProc.reset();
	m_pLight.reset();


}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	m_pDebugProc.reset(new CDebugProc);
	m_pCamera.reset(new CCamera);
	m_pLight.reset(new CLight);

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));							// ワークをゼロクリア
	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	m_d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	m_d3dpp.BackBufferCount = 1;									// バックバッファの数
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// ダブルバッファの切り替え(映像信号に同期)
	m_d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファとして24bitを使う	ステンシルバッファを利用する為 Zバッファに24bit ステンシルバッファに8bit
	m_d3dpp.Windowed = bWindow;										// ウィンドウモード
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート(現在の速度に合わせる)
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT	;	// インターバル(VSyncを待って描画)
	//	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// クライアント領域を直ちに更新する


	//デプスステンシルフォーマットを変更
	// Direct3Dデバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			// ディスプレイアダプタ
		D3DDEVTYPE_HAL,											// デバイスタイプ
		hWnd,													// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,					// デバイス作成制御の組み合わせ
		&m_d3dpp,													// デバイスのプレゼンテーションパラメータ
		&m_pD3DDevice)))										// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				//初期化失敗
				return E_FAIL;
			}
		}
	}
	//レンダラーステート設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);							// 裏面(左回り)をカリングする
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);									// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);									// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);							// スペキュラーの有効
	m_pD3DDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);		// エミッションの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);							// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);					// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);				// αデスティネーションカラーの指定.
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);						// 塗りつぶしモード設定

	// ステンシルバッファの設定
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//フォグの設定
	float fStart = 300.0f;
	float fEnd = 800.0f;
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, FOG_COLOR);					// カラー設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);				// 頂点モード
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);			// テーブルモード
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fStart));			// 開始位置
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fEnd));				// 終了位置

	// アルファ
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);


	//フェードの生成
	m_pFade = CFade::Create();

	//初期化
	m_pDebugProc->Init(hWnd);
	m_pCamera->Init();
	m_pLight->Init();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CRenderer::Update()
{
#ifdef _DEBUG

	//ImGuiの更新
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	//Sceneで管理してるやつの情報
	ImGui::Begin("SceneInfo");

	//オブジェクトの更新
	CScene::ShowDebugInfoAll();

	//scene関係の終了
	ImGui::End();

	//BaseModeで管理してるやつの情報
	ImGui::Begin("BaseMode");

	CBaseMode *pBase = CManager::GetBaseMode();

	//nullcheck
	if (pBase)
	{
		//デバッグ表記
		pBase->DebugCommand();
	}

	//BaseMode関係の終了
	ImGui::End();

	//情報
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));

	//レンダラーで管理してるやつの情報
	ImGui::Begin("DebugInfo");

#endif	//DEBUG
	//オブジェクトの更新
	CScene::UpdateAll();
	m_pCamera->Update();

	//更新
	m_pFade->Update();
	m_pLight->Update();

#ifdef _DEBUG
	//Imguiのデバッグ情報表示
	RendererDebugInfo();

	//デバッグのコマンド
	CHossoLibrary::ShowDebugInfo();

	//デバッグ
	CScene::ShowNumAll();

	//レンダラー関係の終了
	ImGui::End();

	//ポップの色情報初期化
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::EndFrame();//更新処理の終わりに
#endif	//DEBUG
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CRenderer::Draw()
{
	//バックバッファ & Zバッファ & ステンシルバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), m_BackColor, 1.0f, 0);

	//Direct3Dによる描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//カメラセット
		m_pCamera->SetCamera();

		//オブジェクトの描画
		CScene::DrawAll();

		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);						// フォグoff

		//フェードの描画
		m_pFade->Draw();


#ifdef _DEBUG
		//DebugProcの情報表示する時
		if (m_bDispDebugProcInfo)
		{
			//デバッグテキストの表示
			m_pDebugProc->Draw();
		}

		//ImGuiの情報表示する時
		if(m_bDispImGuiInfo)
		{
			//ワイヤーフレームoffにする
			SetRendererCommand(RENDERER_WIRE_OFF);

			//通常描画
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			//ImGui描画
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		}

		//debugproc用の文字列開放
		m_pDebugProc->ResetStr();

		//ワイヤーフレーム描画の時は元に戻す
		CHossoLibrary::CheckWireMode();
#endif	//DEBUG

		//Direct3Dによる描画、終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	HRESULT result = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

#ifdef _DEBUG

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && m_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ResetDevice();
	}
#endif	//DEBUG
}
void CRenderer::SetRendererCommand(RENDERER_COMMAND Command)
{
	switch (Command)
	{
		//ライティングON
	case CRenderer::RENDERER_LIGHTING_ON:
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		break;

		//ラインティングOFF
	case CRenderer::RENDERER_LIGHTING_OFF:
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		break;

		//カリングしない
	case CRenderer::RENDERER_CULLING_NONE:
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		break;

		//裏面(左回り)をカリングする
	case CRenderer::RENDERER_CULLING_CCW:
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		break;

		//表面(右回り)をカリングする
	case CRenderer::RENDERER_CULLING_CW:
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		break;

		//加算合成
	case CRenderer::RENDERER_ALPHABLEND_ADD:
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

		//減算合成 影とか
	case CRenderer::RENDERER_ALPHABLEND_SUB:
		m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

		//通常合成
	case CRenderer::REDNERER_ALPHABLEND_DEFAULT:
		m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;

		//Zテスト通常
	case CRenderer::RENDERER_ZTEST_DEFAULT:
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		break;

		//Zテスト無効
	case CRenderer::RENDERER_ZTEST_OFF:
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		break;

		//Zテスト無効&&Zライティング有効
	case CRenderer::RENDERER_ZTEST_OFF_ZWRITING_ON:
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		break;

		//Zテスト有効&&Zライティング無効
	case CRenderer::RENDERER_ZTEST_ON_ZWRITING_OFF:
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		break;

		//フォグON
	case CRenderer::RENDERER_FOG_ON:
		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		break;

		//フォグOFF
	case CRenderer::RENDERER_FOG_OFF:
		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		break;

		//ワイヤーON
	case CRenderer::RENDERER_WIRE_ON:
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		break;

		//ワイヤーOFF
	case CRenderer::RENDERER_WIRE_OFF:
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		break;

	default:
		break;
	}
}
//------------------------------------------------------------------------------
//モードが遷移したときにレンダラーに関する情報の初期化をする
//------------------------------------------------------------------------------
void CRenderer::ResetRenderer()
{
	//カメラの情報リセット 初期情報入れる場合はMode側でやって
	m_pCamera->ResetCamera();

	//シーンの一時停止とか解除
	CScene::SetStop(false);

	//デフォルトのライト設定
	m_pLight->SetDefaultLight();

	//レンダラー設定
	SetRendererCommand(CRenderer::RENDERER_LIGHTING_ON);
	SetRendererCommand(CRenderer::RENDERER_CULLING_CCW);
	SetRendererCommand(CRenderer::RENDERER_ZTEST_DEFAULT);
	SetRendererCommand(CRenderer::REDNERER_ALPHABLEND_DEFAULT);
	SetRendererCommand(CRenderer::RENDERER_WIRE_OFF);

}
//------------------------------------------------------------------------------
//デバイスリセット
//------------------------------------------------------------------------------
void CRenderer::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = m_pD3DDevice->Reset(&m_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

//------------------------------------------------------------------------------
//レンダラーのデバッグ情報設定
//------------------------------------------------------------------------------
void CRenderer::RendererDebugInfo()
{

	ImGui::Begin("RendererInfo");

	m_pCamera->ShowCameraInfo();
	m_pLight->ShowLightInfo();

	//背景色
	//D3DXCLORではImGui対応していないのでfloat型にキャスト
	float *pCol = m_BackColor;

	//項目の大きさ設定
	ImGui::SetNextItemWidth(250);

	//色の設定
	ImGui::ColorEdit4("BackColor", pCol);

	ImGui::End();	//RendererInfo
}