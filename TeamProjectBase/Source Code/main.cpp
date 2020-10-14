//------------------------------------------------------------------------------
//
//メイン処理  [main.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#define _CRTDBG_MAP_ALLOC

//------------------------------------------------------------------------------
//インクルードファイル
//------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
//------------------------------------------------------------------------------
//マクロ定義
//------------------------------------------------------------------------------
#define CLASS_NAME		("WindowClass")			//ウィンドウクラスの名前
#define WINDOW_NAME		("Electric Chain")		//ウィンドウの名前（キャプション表示）
#define WINDOW_MODE		(TRUE)					//ウィンドウモードかどうか (FALSEでフルスクリーン）
#define WINDOW_POS		(INTEGER2(230,50))		//ウィンドウの左上の座標
//------------------------------------------------------------------------------
//プロトタイプ宣言
//------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMSG, WPARAM wParam, LPARAM lParam);

//デバッグ用
#ifdef _DEBUG
int	g_nCountFPS = 0;
int	g_nCountInterval = 0;
void DispConsol();
#endif

//------------------------------------------------------------------------------
//
//メイン関数
//
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int CmdShow)
{
	//CRTライブラリを使ってメモリーリークチェック
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	//_CrtSetBreakAlloc(8768);

	CManager *pManager;
	pManager = new CManager;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),									//WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,											//表示するウィンドウのスタイルを設定
		WindowProc,											//ウィンドウプロシージャのアドレス(関数名)を指定
		0,													//通常は使用しないので0
		0,													//通常は使用しないので0
		hInstance,											//windowsの因数のインスタンスハンドル指定
		LoadIcon(hInstance,MAKEINTRESOURCE(IDI_SWORDICON)),		//タスクバーに使用者のアイコン設定
		LoadCursor(NULL,IDC_ARROW),							//使用するマウスカーソル指定
		(HBRUSH)(COLOR_WINDOW + 1),							//ウィンドウクライアント領域の色指定
		NULL,												//メニューを指定
		CLASS_NAME,											//ウィンドウクラスの名前
		LoadIcon(NULL,MAKEINTRESOURCE(IDI_SWORDICON))			//小さいアイコンの指定
	};

	HWND hWnd;				//ウィンドウハンドル
	MSG msg;				//メッセージを格納する変数
	DWORD dwCurrentTime;	//現在時間
	DWORD dwExeclastTime;	//最後に処理した時間
	DWORD dwFrameCount;		//描画回数をカウント
	DWORD dwFPSLastTime;	//JPSを最後に測定した時間

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	RECT rect = { 0 , 0 , SCREEN_WIDTH,SCREEN_HEIGHT };

	//指定されたクライアント領域を確保する為の計算を行う
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hWnd = CreateWindowEx(0,			//拡張ウィンドウスタイル
		CLASS_NAME,						//ウィンドウクラス名
		WINDOW_NAME,					//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,			//ウィンドウスタイル
		WINDOW_POS.x,					//ウィンドウ左上のX座標
		WINDOW_POS.y,					//ウィンドウ左上のY座標
		(rect.right - rect.left),		//ウィンドウの幅
		(rect.bottom - rect.top),		//ウィンドウの高さ ウィンドウのバーの分の修正
		NULL,							//親ウィンドウのハンドル
		NULL,						    //メニューもしくは子ウィンドウID
		hInstance,						//インスタンスハンドル
		NULL);							//ウィンドウ作成データ

#ifdef _DEBUG
	DispConsol();
#endif
	//初期化処理（ウィンドウの生成後に行う）
	if (FAILED(pManager->Init(hInstance, hWnd, WINDOW_MODE)))
	{
		MessageBox(hWnd, "初期化失敗　終了します", "Manager", MB_OK | MB_ICONHAND);
		return -1;
	}

	//ウィンドウの表示
	ShowWindow(hWnd, CmdShow);

	//分解能の設定
	timeBeginPeriod(1);

	//初期化
	dwCurrentTime = 0;

	//現在時間を取得　(起動してからの時間)
	dwExeclastTime = timeGetTime();

	//初期化
	dwFrameCount = 0;

	//現在時間を取得
	dwFPSLastTime = timeGetTime();

	//メッセージループ(メッセージキューからのメッセージ取得)
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) //メッセージを取得しなかった場合0を返す
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{	//directXの処理
			dwCurrentTime = timeGetTime();

			//前回のタイムからある程度たった時
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
#ifdef _DEBUG
				//FPSの計算
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
#endif
				//FPS計算した時の時間の保存
				dwFPSLastTime = dwCurrentTime;

				//カウントリセット
				dwFrameCount = 0;
			}

			//60秒に1回通過
			if ((dwCurrentTime - dwExeclastTime) >= (1000 / 60))
			{
				//最後に更新した時間を保存
				dwExeclastTime = dwCurrentTime;

#ifdef _DEBUG
				DWORD StartTime = timeGetTime();
#endif //_DEBUG

				//更新処理
				pManager->Update();

				//描画処理
				pManager->Draw();

#ifdef _DEBUG
				DWORD EndTime = timeGetTime();

				g_nCountInterval = EndTime - StartTime;
#endif //_DEBUG

				//回数をカウント
				dwFrameCount++;

			}
		}
	}

	//NULLチェック
	if (pManager != NULL)
	{
		//終了処理
		pManager->Uninit();

		//メモリ開放
		delete pManager;
		pManager = NULL;
	}

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;

}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//------------------------------------------------------------------------------
//ウィンドウプロシージャ関数
//------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_DESTROY:	 //ウィンドウ破棄メッセージ
		//WM_QUIT メッセージを返す
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	//キー押された際のメッセージ
		//キー情報をみる
		switch (wParam)
		{
		case VK_ESCAPE:	//ESCキーを押された際のメッセージ
			//WM_DESTROYを発生
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);  //規定の処理を提供
}

//デバッグ用
#ifdef _DEBUG
//------------------------------------------------------------------------------
//コンソール画面出力
//------------------------------------------------------------------------------
void DispConsol()
{
	//変数宣言
	FILE *pFile = NULL;					//ファイルのポインタ
	char aConsoleWindowTitle[512];		//コンソールウィンドウのタイトル
	HWND ConsoleWindow;					//コンソールウィンドウ
	RECT WindowRect;					//ウィンドウの大きさ

	//コンコールを開く
	AllocConsole();

	//出力先に設定
	freopen_s(&pFile, "CONOUT$", "w", stdout);

	// コンソールウインドウのタイトルを取得
	GetConsoleTitle(aConsoleWindowTitle, sizeof(aConsoleWindowTitle));

	// タイトルからウインドウを検索してウインドウハンドルを取得
	ConsoleWindow = FindWindow(NULL, aConsoleWindowTitle);

	// 現在のウインドウのRectを取得
	GetWindowRect(ConsoleWindow, &WindowRect);

	// ウインドウの座標設定
	MoveWindow(ConsoleWindow, 0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, TRUE);
}

//------------------------------------------------------------------------------
//FPS情報取得処理
//------------------------------------------------------------------------------
int GetFPS(void)
{
	return g_nCountFPS;
}

//------------------------------------------------------------------------------
//FPS情報取得処理
//------------------------------------------------------------------------------
int GetFPSInterval(void)
{
	return g_nCountInterval;
}

#endif
