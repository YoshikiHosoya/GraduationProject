//-------------------------------------------------------------------------------
//
//足し算の処理[main.cpp]
//Author:Yoshiki Hosoya
//
//-------------------------------------------------------------------------------
#include<windows.h>		//インクルードファイル 
//-------------------------------------------------------------------------------
//マクロ定義
//-------------------------------------------------------------------------------
#define CLASS_NAME		"WindowClass"			//ウィンドウクラスの名前      
#define WINDOW_NAME		"ウィンドウ表示処理"	//ウィンドウの名前（キャプション表示）
#define SCREEN_WIDTH	(500)					//ウィンドウの幅
#define SCREEN_HEIGHT	(400)					//ウィンドウの高さ
#define ID_BUTTON000	(101)					//ボタン000のウィンドウID
#define ID_BUTTON001	(102)					//ボタン001のウィンドウID
#define ID_BUTTON002	(103)					//ボタン002のウィンドウID
#define ID_BUTTON003	(104)					//ボタン003のウィンドウID
#define ID_BUTTON004	(105)					//ボタン004のウィンドウID
#define ID_BUTTON005	(106)					//ボタン005のウィンドウID
#define ID_BUTTON006	(107)					//ボタン006のウィンドウID
#define ID_BUTTON007	(108)					//ボタン007のウィンドウID
#define ID_EDIT000		(111)					//エディット000ウィンドウのID
#define ID_EDIT001		(112)					//エディット001ウィンドウのID
#define ID_EDIT002		(113)					//エディット002ウィンドウのID
#define ID_TIMER		(121)					//タイマーID
#define TIMER_INTERVAL	(1000/60)				//1フリップあたりの時間
#define RESET_COLOR		(300)					//色のリセットのタイミング
//-------------------------------------------------------------------------------
//プロトタイプ宣言
//-------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMSG, WPARAM wParam, LPARAM lParam);
//-------------------------------------------------------------------------------
//
//メイン関数
//
//-------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int CmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,							//表示するウィンドウのスタイルを設定
		WindowProc,							//ウィンドウプロシージャのアドレス(関数名)を指定
		0,									//通常は使用しないので0
		0,									//通常は使用しないので0
		hInstance,							//windowsの因数のインスタンスハンドル指定
		LoadIcon(NULL,IDI_APPLICATION),		//タスクバーに使用者のアイコン設定
		LoadCursor(NULL,IDC_ARROW),			//使用するマウスカーソル指定
		(HBRUSH)(COLOR_WINDOW + 1),			//ウィンドウクライアント領域の色指定
		NULL,								//メニューを指定
		CLASS_NAME,							//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)		//小さいアイコンの指定
	};

	HWND hWnd;		//ウィンドウハンドル
	MSG msg;		//メッセージを格納する変数

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//ウィンドウの生成
	hWnd = CreateWindowEx(0,					//拡張ウィンドウスタイル
						CLASS_NAME,				//ウィンドウクラス名
						WINDOW_NAME,			//ウィンドウの名前
						WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
						CW_USEDEFAULT,			//ウィンドウ左上のX座標
						CW_USEDEFAULT,			//ウィンドウ左上のY座標
						SCREEN_WIDTH,			//ウィンドウの幅
						SCREEN_HEIGHT,			//ウィンドウの高さ 
						NULL,					//親ウィンドウのハンドル
						NULL,					//メニューもしくは子ウィンドウID
						hInstance,				//インスタンスハンドル
						NULL);					//ウィンドウ作成データ

	//ウィンドウの表示
	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);

	//メッセージループ(メッセージキューからのメッセージ取得)
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		//メッセージの翻訳と送出
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;
}

//-------------------------------------------------------------------------------
//
//ウィンドウプロシージャ関数
//
//-------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//変数宣言
	int nID;									//YESかNOか
	HWND hWndButton000;							//終了ボタン
	//電卓関係
	HWND hWndButton001;							//[+]ボタン
	HWND hWndButton002;							//[-]ボタン
	HWND hWndButton003;							//[*]ボタン
	HWND hWndButton004;							//[/]ボタン
	HWND hWndButton005;							//消去ボタン1
	HWND hWndButton006;							//消去ボタン2
	HWND hWndButton007;							//全消去ボタン
	static HWND hWndEdit000;					//数値入力欄1
	static HWND hWndEdit001;					//数値入力欄2
	static HWND hWndEdit002;					//数値出力欄
	char aData000[64], aData001[64];			//入力された数値
	char aAnswer[64];							//出力する数値
	int nData000, nData001;						//計算用の変数
	int nAnswer;								//計算用の変数
	//画面描写関係
	HDC hDC;									//デバイスコンテキストのハンドル(GDI)
	PAINTSTRUCT ps;								//ウィンドウのクライアント領域を描画する時に使用する情報
	HPEN hPen000, hPenOld;						//ペンのハンドル
	static int aPosX[3] = {10,300,100};			//三角形の頂点のX座標
	static int aPosY[3] = {10,20,200};			//三角形の頂点のY座標
	static int aSpeedX[3] = {6, 5, 9};			//X方向の移動の速さ
	static int aSpeedY[3] = {3, 6, 2};			//Y方向の移動の速さ
	static int nRed = 0;						//赤
	static int nGreen = 0;						//緑
	static int nBlue = 0;						//青
	static int nColorCount = 0;					//色の変わった回数をカウント
	int nCount;									//for文のカウント

	const RECT rect = { 0,0,SCREEN_WIDTH , SCREEN_HEIGHT };

	switch (uMsg)
	{
		//一回だけ動く
	case WM_CREATE:
		//タイマーの設定(WM_TIMERメッセージを定期的に発生)
		SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, NULL);

		//ボタン000(終了ボタン)の作成
		hWndButton000 = CreateWindowEx(0,
									"BUTTON",
									" ",
									(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),		//子ウィンドウ|見える状態|押しボタン
									360, 300, 100, 40,								//座標と枠の大きさ	
									hWnd,											//親ウィンドウ
									(HMENU)ID_BUTTON000,							//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);
		//ボタン000ウィンドウに表示する文字列
		SetWindowText(hWndButton000, "終了");

		//ボタン001([+]ボタン)の作成
		hWndButton001 = CreateWindowEx(0,
									"BUTTON", 
									" ",
									(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),		//子ウィンドウ|見える状態|押しボタン
									50, 100, 50, 50,								//座標と枠の大きさ	
									hWnd,											//親ウィンドウ
									(HMENU)ID_BUTTON001,							//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);
		//ボタン001ウィンドウに表示する文字列
		SetWindowText(hWndButton001, "+");

		//ボタン002([-]ボタン)の作成
		hWndButton002 = CreateWindowEx(0,
									"BUTTON",
									" ",
									(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),		//子ウィンドウ|見える状態|押しボタン
									120, 100, 50, 50,								//座標と枠の大きさ	
									hWnd,											//親ウィンドウ
									(HMENU)ID_BUTTON002,							//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);
		//ボタン002ウィンドウに表示する文字列
		SetWindowText(hWndButton002, "-");

		//ボタン003([*]ボタン)の作成
		hWndButton003 = CreateWindowEx(0,
									"BUTTON",
									" ",
									(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),		//子ウィンドウ|見える状態|押しボタン
									190, 100, 50, 50,								//座標と枠の大きさ	
									hWnd,											//親ウィンドウ
									(HMENU)ID_BUTTON003,							//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);
		//ボタン003ウィンドウに表示する文字列
		SetWindowText(hWndButton003, "*");

		//ボタン004([/]ボタン)の作成
		hWndButton004 = CreateWindowEx(0,
									"BUTTON",
									" ",
									(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),		//子ウィンドウ|見える状態|押しボタン
									260, 100, 50, 50,								//座標と枠の大きさ	
									hWnd,											//親ウィンドウ
									(HMENU)ID_BUTTON004,							//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);
		//ボタン004ウィンドウに表示する文字列
		SetWindowText(hWndButton004, "/");

		//ボタン005(消去ボタン1)の作成
		hWndButton005 = CreateWindowEx(0,
									"BUTTON",
									" ",
									(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),		//子ウィンドウ|見える状態|押しボタン
									350, 50, 100, 25,								//座標と枠の大きさ	
									hWnd,											//親ウィンドウ
									(HMENU)ID_BUTTON005,							//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);
		//ボタン005ウィンドウに表示する文字列
		SetWindowText(hWndButton005, "←消去");

		//ボタン006(消去ボタン2)の作成
		hWndButton006 = CreateWindowEx(0,
									"BUTTON",
									" ",
									(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),		//子ウィンドウ|見える状態|押しボタン
									350, 175, 100, 25,								//座標と枠の大きさ	
									hWnd,											//親ウィンドウ
									(HMENU)ID_BUTTON006,							//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);
		//ボタン006ウィンドウに表示する文字列
		SetWindowText(hWndButton006, "←消去");

		//ボタン007(全消去ボタン)の作成
		hWndButton007 = CreateWindowEx(0,
									"BUTTON",
									" ",
									(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),		//子ウィンドウ|見える状態|押しボタン
									350, 250, 100, 25,								//座標と枠の大きさ	
									hWnd,											//親ウィンドウ
									(HMENU)ID_BUTTON007,							//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);
		//ボタン007ウィンドウに表示する文字列
		SetWindowText(hWndButton007, "全消去");


		//エディット000(数値入力欄1)を作成
		hWndEdit000 = CreateWindowEx(0,
									"EDIT",
									" ",
									(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT),	//子ウィンドウ|見える状態|境界線|右寄せ
									100, 50, 200, 25,								//座標と枠の大きさ
									hWnd,											//親ウィンドウ
									(HMENU)ID_EDIT000,								//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);

		//エディット001(数値入力欄2)を作成
		hWndEdit001 = CreateWindowEx(0,
									"EDIT",
									" ",
									(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT),	//子ウィンドウ|見える状態|境界線|右寄せ
									100, 175, 200, 25,								//座標と枠の大きさ
									hWnd,											//親ウィンドウ
									(HMENU)ID_EDIT001,								//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),	//インスタンスハンドル(アプリの識別子)
									NULL);

		//エディット002(数値出力欄)を作成
		hWndEdit002 = CreateWindowEx(0,
									"EDIT",
									" ",
									(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED |ES_RIGHT),	//子ウィンドウ|見える状態|境界線|書き換え不可|右寄せ
									100, 250, 200, 25,												//座標と枠の大きさ
									hWnd,															//親ウィンドウ
									(HMENU)ID_EDIT002,												//ID
									(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),					//インスタンスハンドル(アプリの識別子)
									NULL);
		break;

		//ウィンドウ破棄メッセージ
	case WM_DESTROY:
		//WM_QUIT メッセージを返す (プログラム終了)
		PostQuitMessage(0);
		break;

		//キー押された際のメッセージ
	case WM_KEYDOWN:
		//キー情報をみる
		switch (wParam)
		{
			//ESCキーを押された際のメッセージ
		case VK_ESCAPE:
			//メッセージの表示
			nID = MessageBox(hWnd, "終了？", "終了しますか？", MB_YESNO);
			//[YES]を選択された場合
			if (nID == IDYES)
			{
				//ウィンドウを破棄(WM_DESTROYを発生)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

		//ボタンを押された際のメッセージ
	case WM_COMMAND:
		//終了ボタンが押された場合
		if (LOWORD(wParam) == ID_BUTTON000)
		{
			//メッセージの表示
			nID = MessageBox(hWnd, "終わり？", "終わりますか？", MB_YESNO);
			//[YES]を選択された場合
			if (nID == IDYES)
			{
				//ウィンドウを破棄(WM_DESTROYを発生)
				DestroyWindow(hWnd);
			}
		}
		else if (LOWORD(wParam) == ID_BUTTON001 || LOWORD(wParam) == ID_BUTTON002 ||
				LOWORD(wParam) == ID_BUTTON003 || LOWORD(wParam) == ID_BUTTON004)
		{	//計算ボタンが押された場合

			//エディットウィンドウ000,001からテキスト取得
			GetWindowText(hWndEdit000, &aData000[0], 256);
			GetWindowText(hWndEdit001, &aData001[0], 256);

			//文字列を数値に変換
			nData000 = atoi(&aData000[0]);
			nData001 = atoi(&aData001[0]);

			//[+]を押した場合
			if (LOWORD(wParam) == ID_BUTTON001)
			{
				nAnswer = nData000 + nData001;
			}
			else if (LOWORD(wParam) == ID_BUTTON002)
			{	//[-]を押した場合
				nAnswer = nData000 - nData001;
			}
			else if (LOWORD(wParam) == ID_BUTTON003)
			{	//[*]を押した場合
				nAnswer = nData000 * nData001;
			}
			else if (LOWORD(wParam) == ID_BUTTON004)
			{	//[/]を押した場合
				//数値入力欄2に0が入っていた場合
				if (nData001 == 0)
				{
					//答えを0にする
					nAnswer = 0;
				}
				else
				{
					//計算をする
					nAnswer = nData000 / nData001;
				}
			}
			//変数にテキストで設定
			wsprintf(&aAnswer[0], "%d", nAnswer);
			//エディット002ウィンドウ(出力欄)にテキスト設定
			SetWindowText(hWndEdit002, &aAnswer[0]);
		}
		else if (LOWORD(wParam) == ID_BUTTON005)
		{	//消去ボタン1を押された場合
			//数値入力欄1を消去する
			SetWindowText(hWndEdit000, "");
		}
		else if (LOWORD(wParam) == ID_BUTTON006)
		{	//消去ボタン2を押された場合
			//数値入力欄2を消去する
			SetWindowText(hWndEdit001, "");
		}
		else if (LOWORD(wParam) == ID_BUTTON007)
		{	//全消去ボタン2を押された場合
			//数値入力欄をすべて消去する
			SetWindowText(hWndEdit000, "");
			SetWindowText(hWndEdit001, "");
			SetWindowText(hWndEdit002, "");
		}
		break;

		//×ボタンを押された際のメッセージ
	case WM_CLOSE:
		//メッセージを表示
		nID = MessageBox(hWnd, "やめる？", "やめますか？", MB_YESNO);
		//YESを選択された場合
		if (nID == IDYES)
		{
			//ウィンドウを破棄(WM_DESTROYを発生)
			DestroyWindow(hWnd);
		}
		else
		{
			//強制的に抜ける
			return 0;
		}
		break;

		//タイマーの際のメッセージ
	case WM_TIMER:
		//頂点の数繰り返す
		for (nCount = 0; nCount < 3; nCount++)
		{
			//X座標が画面端に行ったとき
			if (aPosX[nCount] <= 0 || aPosX[nCount] >= SCREEN_WIDTH - 10)
			{
				//画面外に行ったときに画面端に移動する
				if (aPosX[nCount] >= SCREEN_WIDTH - 10)
				{
					aPosX[nCount] = SCREEN_WIDTH - 10;
				}
				else if (aPosX[nCount] >= 0)
				{
					aPosX[nCount] = 0;
				}
				//移動の方向を逆方向にする
				aSpeedX[nCount] *= -1;
			}

			//Y座標が画面端に行ったとき
			if (aPosY[nCount] <= 0 || aPosY[nCount] >= SCREEN_HEIGHT - 30)
			{
				//画面外に行ったときに画面端に移動する
				if (aPosY[nCount] >= SCREEN_HEIGHT - 30)
				{
					aPosY[nCount] = SCREEN_HEIGHT - 30;
				}
				else if (aPosY[nCount] >= 0)
				{
					aPosY[nCount] = 0;
				}
				//移動の方向を逆方向にする
				aSpeedY[nCount] *= -1;
			}
			//X座標をaSpeedX分動かす
			aPosX[nCount] += aSpeedX[nCount];
			//Y座標をaSpeedY分動かす
			aPosY[nCount] += aSpeedY[nCount];
		}
		nRed +=2 ;		//赤の色を変化する
		nGreen += 3;	//緑の色を変化する
		nBlue += 5;		//青の色を変化する
		nColorCount++;	//色の変わった回数をカウントする

		//無効領域の強制発生(WM_PAINTメッセージを発生)
		InvalidateRect(hWnd, &rect, FALSE);
		break;

		//画面描画メッセージ
	case WM_PAINT:
		//描画開始
		hDC = BeginPaint(hWnd, &ps);
		//色の変わった回数が[RESET_COLOR]になったら
		if (nColorCount == RESET_COLOR)
		{
			//塗りつぶし処理
			FillRect(hDC, &rect, NULL);
			//カウントを0にする
			nColorCount = 0;
		}
		//ペンを生成する
		hPen000 = CreatePen(PS_SOLID, 1, RGB(nRed, nBlue, nGreen));
		//ペンを選択する
		hPenOld = (HPEN)SelectObject(hDC, hPen000);

		//線分描画 (三角形)
		MoveToEx(hDC, aPosX[0], aPosY[0], NULL);
		LineTo(hDC, aPosX[1], aPosY[1]);
		LineTo(hDC, aPosX[2], aPosY[2]);
		LineTo(hDC, aPosX[0], aPosY[0]);

		//ペンを選択する
		SelectObject(hDC, hPenOld);

		//ペンを破棄する
		DeleteObject(hPen000);

		//描画終了
		EndPaint(hWnd, &ps);
		break;

		//右クリックを押した際のメッセージ
	case WM_LBUTTONDOWN:
		//ウィンドウにフォーカスを合わせる
		SetFocus(hWnd);
		break;

	default:
		break;
	}
		//規定の処理を提供
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}