// ===================================================================
//
// クライアント処理 [ client.cpp ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS		// winsockの警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "manager.h"
#include "renderer.h"
#include "client.h"
#include "chatTab.h"
#include "chatText.h"
#include "picture.h"
#include <sys/stat.h>
#include "connectUI.h"
#include "basemode.h"
#include "result.h"
#include "tutorial.h"
#include "timer.h"
#include "Decoding.h"

// ===================================================================
// マクロ定義
// ===================================================================
#define VERSION_WINSOCK		(2)					// winsockのバージョン

#define LINK_IPADDRESS		("data/TEXT/IP.txt")								// IPアドレスのパス
#define LINK_SENDTEXT		("data/SAVEDATA/SendText.txt")						// 送信用テキストのパス
#define LINK_RECVTEXT		("data/SAVEDATA/RecvText.txt")						// 受信用テキストのパス
#define LINK_SENDPICTURE	("data/SAVEDATA/PictureTextures/PicTex.txt")		// 送信用ピクチャのパス
#define LINK_RECVPICTURE	("data/SAVEDATA/PictureTextures/RecvPicTex.txt")	// 受信用ピクチャのパス

#define SIZE_SEND_TEXT		(9)					// SEND_TEXTのサイズ
#define SIZE_SEND_PICTURE	(12)				// SEND_PICTUREのサイズ
#define SIZE_SPACE			(2)					// 文字列のスペースのサイズ

#define NUM_CMPCHAR			(6)					// 受信時のテキストかピクチャかの比較用

// ===================================================================
// 静的メンバ変数の初期化
// ===================================================================
bool CClient::m_bAccept					= false;
bool CClient::m_bAcceptOther			= false;
SOCKET CClient::m_socket				= NULL;
CClient::SOCKINFO CClient::m_sockInfo	= {};

// ===================================================================
// IPアドレスのロード
// ===================================================================
HRESULT CClient::LoadIP(void)
{
	// 変数宣言
	FILE *pFile;
	char cReadText[MAX_TEXT];
	char cHeadText[MAX_TEXT] = "\0";
	char cAddress[64] = "\0";
	u_short port = 0;

	// ファイルを開く
	pFile = fopen(LINK_IPADDRESS, "r");

#ifdef _DEBUG
	std::cout << "IPアドレス・ポート番号の取得を開始\n";
#endif

	// 失敗
	if (!pFile)
	{
#ifdef _DEBUG
		std::cout << "ファイルオープンに失敗\n";
#endif
		return E_FAIL;
	}

	// スクリプトがくるまで繰り返す
	while (strcmp(cHeadText, "SCRIPT") != 0)
	{
		fgets(cReadText, sizeof(cReadText), pFile);
		sscanf(cReadText, "%s", &cHeadText);
	}
	// スクリプトが来たら
	if (strcmp(cHeadText, "SCRIPT") == 0)
	{
		// エンドスクリプトがくるまで繰り返す
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			strcpy(cHeadText, "");
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
			// 改行
			if (strcmp(cHeadText, "\n") == 0)
				continue;
			// IPアドレス
			if (strcmp(cHeadText, "IPアドレス") == 0)
				sscanf(cReadText, "IPアドレス : %s", cAddress);
			// ポート番号
			if (strcmp(cHeadText, "ポート番号") == 0)
				sscanf(cReadText, "ポート番号 : %hd", &port);
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	// 情報を格納
	strcpy(m_sockInfo.ip_Addr, cAddress);
	m_sockInfo.port = port;

#ifdef _DEBUG
	std::cout << "取得完了\n";
	std::cout << "IPアドレス : " << m_sockInfo.ip_Addr << std::endl;
	std::cout << "ポート番号 : " << m_sockInfo.port << std::endl;
#endif

	// 成功
	return S_OK;
}

// ===================================================================
// メイン関数
// ===================================================================
int CClient::ConnectServer(void)
{
	struct sockaddr_in server;
	int err = 0;

	// winsockの初期化
	if (InitClient() == E_FAIL)
	{
		// エラーによる強制終了
		return 0;
	}

	// IPアドレスを取得
	LoadIP();

	// ソケットの作成
	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(m_sockInfo.port);
	server.sin_addr.S_un.S_addr = inet_addr(m_sockInfo.ip_Addr);

	while (!m_bAccept)
	{
		// 接続
		err = connect(m_socket, (struct sockaddr *)&server, sizeof(server));

		// エラー発生
		if (err == -1)
		{
#ifdef _DEBUG
			printf("接続中…\n");
#endif
			// 再接続
			continue;
		}

		// 接続完了
#ifdef _DEBUG
		printf("接続完了\n");
#endif
		m_bAccept = true;
	}
	

	// 受信待ち
	std::thread threadRecv(CClient::WaitRecieve);
	threadRecv.detach();

	return 0;
}

// ===================================================================
// 受信待ち処理
// ===================================================================
void CClient::WaitRecieve(void)
{
	while (1)
	{
		// 接続しない
		if (!m_bAccept)
		{
			break;
		}

		char cData[16600];
		memset(cData, 0, sizeof(cData));

		// 受信待ち
		if (recv(m_socket, cData, 16600, 0) <= 0)
		{
			// 受信していない
			continue;
		}

		if (cData[0] == ORDER_GUEST_BE_ACCEPT)
		{
			printf("ゲストとの接続を確認\n");
			m_bAcceptOther = true;
		}
		// テキスト受信
		if (cData[0] == ORDER_SENDTEXT)
		{
			RecvText(cData);
		}
		// ピクチャ受信
		if (cData[0] == ORDER_SENDPICTURE)
		{
			RecvPicture(cData);
		}
		// 選択受信
		if (cData[0] == ORDER_SENDSELECT)
		{
			RecvSelect(cData);
		}
		// 待ち状態受信
		if (cData[0] == ORDER_SENDWAIT)
		{
			RecvWait();
		}
		// リトライ受信
		if (cData[0] == ORDER_RETRY)
		{
			RecvRetry();
		}
		// 終了受信
		if (cData[0] == ORDER_END_GAME)
		{
			RecvEndGame();
		}
		// ゲームオーバー受信
		if (cData[0] == ORDER_GAMEOVER)
		{
			RecvGameOver(cData);
		}
		// ゲームクリア受信
		if (cData[0] == ORDER_GAMECLEAR)
		{
			RecvGameClear(cData);
		}
	}
}

// ===================================================================
// サーバーの初期化
// ===================================================================
HRESULT CClient::InitClient(void)
{
	m_bAccept = false;
	m_bAcceptOther = false;

	WSADATA wsaData;	// winsockのデータ

	// サーバ・クライアント共通で開始時のみ、必ず行う
	int err = WSAStartup(MAKEWORD(VERSION_WINSOCK, 0), &wsaData);

	// エラー無し
	if (err == 0)
	{
		// 成功
		return S_OK;
	}

#ifdef _DEBUG
	// エラーメッセージ
	ErrorReport();
#endif

	// winsock2の終了処理
	WSACleanup();
	// 失敗
	return E_FAIL;
}

// ===================================================================
// サーバーの終了
// ===================================================================
void CClient::UninitClient(void)
{
	// 接続終了
	if (m_bAccept)
		m_bAccept = false;
	// winsock2の終了処理
	WSACleanup();
}

// ===================================================================
// テキストの送信
// ===================================================================
void CClient::SendText(void)
{
	if (!m_bAccept)
	{
#ifdef _DEBUG
		printf("サーバーに接続されていません\n");
#endif
		//return;
	}
	
	FILE *fp = fopen(LINK_SENDTEXT, "wb");
	char *buffer;
	struct stat stbuf;

	// ファイルを開く
	if (!fp)
	{
		// エラー
		printf("テキスト読み込みエラー\n");
		return;
	}

	// ファイルにデータを書き込み
	fwrite(CChatTab::GetSendText()->GetChatText().c_str(),	// 書き込む文字列
		CChatTab::GetSendText()->GetChatText().size(),		// 書き込むサイズ
		1,													// 書き込むデータの数
		fp);												// 書き込み先

	// ファイルを閉じる
	fclose(fp);

	// ファイルサイズの取得
	stat(LINK_SENDTEXT, &stbuf);
	printf("ファイルサイズ : %.1f byte (%d bit) \n", (float)stbuf.st_size / 8, (int)stbuf.st_size);

	// 文字列 + 命令 + サイズ + null文字 のメモリ数分格納
	buffer = new char[stbuf.st_size + 5 + 1];

	fp = fopen(LINK_SENDTEXT, "rb");
	// ファイルを開く
	if (!fp)
	{
		// エラー
		printf("テキスト読み込みエラー\n");
		return;
	}

	// 文字列のアドレスを取得
	fread(&buffer[5], stbuf.st_size, 1, fp);
	buffer[5 + stbuf.st_size] = '\0';

	// ファイルを閉じる
	fclose(fp);

	// 文字列送信命令
	buffer[0] = ORDER_SENDTEXT;

	// ファイルサイズを格納
	int *pData = (int*)&buffer[1];
	*pData = stbuf.st_size;

#ifdef _DEBUG
	// 送信データをデバッグで表示
	printf("テキスト送信 > %s [%d字]\n", &buffer[5], (int)stbuf.st_size);
#endif
	// 送信
	send(m_socket, buffer, 5 + stbuf.st_size, 0);

	// 履歴に追加
	CChatTab::AddRecvKeep(CChatBase::OWNER_OWN, &buffer[5]);

	// 記入した文字列をリセット
	CChatTab::GetSendText()->GetChatText().clear();

	delete[] buffer;
}

// ===================================================================
// ピクチャの送信
// ===================================================================
void CClient::SendPicture(void)
{
	if (!m_bAccept)
	{
#ifdef _DEBUG
		printf("サーバーに接続されていません\n");
#endif
		//return;
	}

	FILE *fp;
	char *buffer;
	struct stat stbuf;

	// ファイルサイズの取得
	stat(LINK_SENDPICTURE, &stbuf);
	printf("ファイルサイズ : %.1f byte (%d bit) \n", (float)stbuf.st_size / 8, (int)stbuf.st_size);

	// 文字列 + 命令 + サイズ + null文字 のメモリ数分格納
	buffer = new char[stbuf.st_size + 5 + 1];

	fp = fopen(LINK_SENDPICTURE, "rb");
	// ファイルを開く
	if (!fp)
	{
		// エラー
		printf("テキスト読み込みエラー\n");
		return;
	}

	// 文字列のアドレスを取得
	fread(&buffer[5], stbuf.st_size, 1, fp);
	buffer[5 + stbuf.st_size] = '\0';

	// ファイルを閉じる
	fclose(fp);

	// 文字列送信命令
	buffer[0] = ORDER_SENDPICTURE;

	// ファイルサイズを格納
	int *pData = (int*)&buffer[1];
	*pData = stbuf.st_size;

#ifdef _DEBUG
	// 送信データをデバッグで表示
	printf("ピクチャ送信 > %s [%d字]\n", &buffer[5], (int)stbuf.st_size);
#endif
	// 送信
	send(m_socket, buffer, 5 + stbuf.st_size + 1, 0);

	// 空のピクチャを生成
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (FAILED(CManager::GetRenderer()->GetDevice()->CreateTexture(128, 128, 1, 0, D3DFMT_A32B32G32R32F,
		D3DPOOL_MANAGED, &pTexture, NULL)))
	{
		std::cout << "ピクチャ生成失敗\n";
	}
	// ピクチャの情報を格納
	CString link;
	link = LINK_SENDPICTURE;
	CPicture::Reading(pTexture, link);
	// 履歴に追加
	CChatTab::AddRecvPicture(CChatBase::OWNER_OWN, pTexture);

	delete[] buffer;
}

// ===================================================================
// テキストの受信
// ===================================================================
void CClient::RecvText(char *data)
{
	// サイズ
	int *pSize = (int*)&data[1];

	char *cTextData = new char[*pSize + 1];
	memcpy(cTextData, &data[5], *pSize);
	cTextData[*pSize] = '\0';

	FILE *fp = fopen(LINK_RECVTEXT, "wb");

	// ファイルを開く
	if (!fp)
	{
		// エラー
		printf("テキスト読み込みエラー\n");
		return;
	}

	fwrite(cTextData, *pSize, 1, fp);

	// ファイルを閉じる
	fclose(fp);

#ifdef _DEBUG
	// 送信データをデバッグで表示
	printf("テキスト受信 > %s [%d字]\n", cTextData, *pSize);
#endif
	// 履歴に追加
	CChatTab::AddRecvKeep(CChatBase::OWNER_GUEST, cTextData);

	delete[] cTextData;
}

// ===================================================================
// ピクチャの受信
// ===================================================================
void CClient::RecvPicture(char *data)
{
	// サイズ
	int *pSize = (int*)&data[1];

	char *cPicData = new char[*pSize + 1];
	memcpy(cPicData, &data[5], *pSize);
	cPicData[*pSize] = '\0';

	FILE *fp = fopen(LINK_RECVPICTURE, "wb");

	// ファイルを開く
	if (!fp)
	{
		// エラー
		printf("テキスト読み込みエラー\n");
		return;
	}

	fwrite(cPicData, *pSize, 1, fp);

	// ファイルを閉じる
	fclose(fp);

#ifdef _DEBUG
	// 送信データをデバッグで表示
	printf("ピクチャ受信 > %s [%d字]\n", cPicData, *pSize);
#endif

	// 格納用
	LPDIRECT3DTEXTURE9 pTexture = NULL;

	if (FAILED(CManager::GetRenderer()->GetDevice()->CreateTexture(128, 128, 1, 0, D3DFMT_A32B32G32R32F,
		D3DPOOL_MANAGED, &pTexture, NULL)))
	{
		throw E_FAIL;
	}

	CString link;
	link = LINK_RECVPICTURE;
	CPicture::Reading(pTexture, link);

	CChatTab::AddRecvPicture(CChatBase::OWNER_GUEST, pTexture);

	delete[] cPicData;
}

// ===================================================================
// 選択の送信
// ===================================================================
void CClient::SendSelect(int nSelect)
{
	char buffer[2];

	// 選択送信命令
	buffer[0] = ORDER_SENDSELECT;
	buffer[1] = nSelect;

	// 送信
	send(m_socket, buffer, 2, 0);
}

// ===================================================================
// 選択の受信
// ===================================================================
void CClient::RecvSelect(char *data)
{
	int *pSelect = (int*)&data[1];
	// 選択を格納
	CConnectUI::RecvGuestSelect(*pSelect);
}

// ===================================================================
// 待ち状態の送信
// ===================================================================
void CClient::SendWait(void)
{
	char buffer[1];

	// 待ち状態命令
	buffer[0] = ORDER_SENDWAIT;

	// 送信
	send(m_socket, buffer, 1, 0);
}

// ===================================================================
// 待ち状態の受信
// ===================================================================
void CClient::RecvWait(void)
{
	// サーバー接続
	if (CManager::GetMode() == CManager::MODE_CONNECT_SERVER)
		CConnectUI::RecvGuestWait();
	// チュートリアル
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		CTutorial::GuestEndTutorial();
}

// ===================================================================
// リトライの受信
// ===================================================================
void CClient::SendRetry(void)
{
	char buffer[1];

	buffer[0] = ORDER_RETRY;

	send(m_socket, buffer, 1, 0);
}

// ===================================================================
// リトライの受信
// ===================================================================
void CClient::RecvRetry(void)
{
	if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		CResult *pResult = (CResult*)CManager::GetBaseMode();
		pResult->SetGuestMode((int)ORDER_RETRY);
	}
}

// ===================================================================
// ゲームオーバーの送信
// ===================================================================
void CClient::SendGameOver(void)
{
	char buffer[2];

	buffer[0] = ORDER_GAMEOVER;
	buffer[1] = CTimer::GetClearFlame();
	printf("time : %d\n", CTimer::GetClearFlame());

	send(m_socket, buffer, 2, 0);
}

// ===================================================================
// ゲームオーバーの受信
// ===================================================================
void CClient::RecvGameOver(char *data)
{
	int *pTime = (int*)&data[1];
	// タイムを設定
	CTimer::SetClearFrame(*pTime);
	// 解説モードを終了
	CDecoding::SetGameEndState(CDecoding::GAMEEND_MISS);
}

// ===================================================================
// ゲームクリアの送信
// ===================================================================
void CClient::SendGameClear(void)
{
	char buffer[2];

	buffer[0] = ORDER_GAMECLEAR;
	buffer[1] = CTimer::GetClearFlame();
	printf("time : %d\n", CTimer::GetClearFlame());

	send(m_socket, buffer, 2, 0);
}

// ===================================================================
// ゲームクリアの受信
// ===================================================================
void CClient::RecvGameClear(char *data)
{
	int *pTime = (int*)&data[1];
	// タイムを設定
	CTimer::SetClearFrame(*pTime);
	// 解説モードを終了
	CDecoding::SetGameEndState(CDecoding::GAMEEND_CLEAR);
}

// ===================================================================
// ゲーム終了の送信
// ===================================================================
void CClient::SendEndGame(void)
{
	char buffer[1];

	buffer[0] = ORDER_END_GAME;

	send(m_socket, buffer, 1, 0);
}

// ===================================================================
// ゲーム終了の受信
// ===================================================================
void CClient::RecvEndGame(void)
{
	if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		CResult *pResult = (CResult*)CManager::GetBaseMode();
		pResult->SetGuestMode((int)ORDER_END_GAME);
	}
}

// ===================================================================
// 接続終了の送信
// ===================================================================
void CClient::SendEndAccept(void)
{
	char buffer[1];

	// 待ち状態命令
	buffer[0] = ORDER_CLOSED_SOCKET;

	// 送信
	send(m_socket, buffer, 1, 0);
}

#ifdef _DEBUG
// ===================================================================
// エラーメッセージ
// ===================================================================
void CClient::ErrorReport(void)
{
	// エラーコードを取得
	int err = WSAGetLastError();
	printf("！！！エラー発生！！！\n");
	printf("ErrorCode : %d\n", err);

	// エラーの原因を表示
	switch (err)
	{
	case WSASYSNOTREADY:
		printf("ネットワークサブシステムがネットワークへの接続を準備できていません。\n");
		break;
	case WSAVERNOTSUPPORTED:
		printf("要求されたwinsockのバージョンがサポートされていません。\n");
		break;
	case WSAEINPROGRESS:
		printf("ブロッキング操作の実行中であるか、 またはサービスプロバイダがコールバック関数を処理しています。\n");
		break;
	case WSAEPROCLIM:
		printf("winsockが処理できる最大プロセス数に達しました。\n");
		break;
	case WSAEFAULT:
		printf("第二引数である lpWSAData は有効なポインタではありません。\n");
		break;
	case EBADF:
		printf("ソケットが無効ソケット記述子です。\n");
		break;
	case ECONNRESET:
		printf("接続はピアによって強制的にクローズされました。\n");
		break;
	case EFAULT:
		printf("bufおよびlenパラメータを使用すると、\n");
		printf("呼び出し元のアドレス・スペースの外側のストレージにアクセスすることになります。\n");
		break;
	case EINTR:
		printf("recv()の呼び出しは、データの使用可能になる前に\n");
		printf("キャッチされたシグナルによって割り込まれました。\n");
		break;
	case EINVAL:
		printf("要求が無効か、またはサポートされていません。\n");
		printf("MSG_OOBフラグが設定され、アウト・オブ・バンドのデータが使用できません。\n");
		break;
	case EIO:
		printf("ネットワークまたはトランスポートで障害が発生しました。\n");
		break;
	case ENOTCONN:
		printf("接続されていない接続指向ソケットに対する受信を試みています。\n");
		break;
	case ENOTSOCK:
		printf("記述子はファイル用であり、ソケット用ではありません。\n");
		break;
	case EOPNOTSUPP:
		printf("指定したフラグは、このソケット・タイプばたはプロトコルの場合はサポートされていません。\n");
		break;
	case ETIMEDOUT:
		printf("接続の設定中またはアクティブな接続での伝送のタイムアウトのために、\n");
		printf("接続がタイムアウトになりました。\n");
		break;
	case EWOULDBLOCK:
		printf("socketは、非ブロック・モードになっているため、データの読み取りはできません。\n");
		printf("または、SO_RCVTIMEOタイムアウト値に達したためデータは使用できませんでした。");
		break;
	case WSAEINTR:
		printf("ブロッキングWindows Sockets 1.1呼び出しが WSACancelBlockingCall を通じてキャンセルされました。\n");
		break;
	case WSAEACCES:
		printf("setsockeoptオプションSO_BROADCASTが有効でないため、ダイアグラムソケットのブロードキャストへの\n");
		printf("接続試行が失敗しました。\n");
		break;
	case WSAEINVAL:
		printf("パラメータsはリスニングソケットです。\n");
		break;
	case WSAEWOULDBLOCK:
		printf("そのソケットは非ブロッキングとしてマークされており、接続が直ちに完了しませんでした。\n");
		break;
	case WSAEALREADY:
		printf("非ブロッキングconnect呼び出しがして入れたソケットで実行中です。\n");
		break;
	case WSAENOTSOCK:
		printf("sパラメータで指定されたディスクリプタはソケットではありません。\n");
		break;
	case WSAEAFNOSUPPORT:
		printf("指定されたファミリのアドレスをこのソケットで使用することはできません。\n");
		break;
	case WSAEADDRINUSE:
		printf("ソケットのローカルアドレスが既に使用されており、ソケットがアドレスを再利用可能となるように\n");
		printf("SO_REUSEADDRソケットオプションがつけられていません。\n");
		printf("このエラーは通常bind関数を実行している際に発生しますが、bindが部分的なワイルドカードアドレス(ADDR_ANY)を\n");
		printf("指定し、かつ、特定のアドレスがこの関数の時点でコミットされる必要がある場合にこの関数まで先延ばしされる\n");
		printf("可能性があります。\n");
		break;
	case WSAEADDRNOTAVAIL:
		printf("リモートアドレスが有効ではありません。\n");
		break;
	case WSAENETDOWN:
		printf("ネットワークサブシステムが失敗しました。\n");
		break;
	case WSAENETUNREACH:
		printf("現時点でこのホストからネットワークに到達することができません。\n");
		break;
	case WSAENOBUFS:
		printf("利用可能なバッファ空間がありません。ソケットを接続できません。\n");
		break;
	case WSAEISCONN:
		printf("指定されたソケットは既に接続されています(コネクション指向のソケットのみ)。\n");
		break;
	case WSAETIMEDOUT:
		printf("接続試行は接続が確立することなくタイムアウトしました。\n");
		break;
	case WSAECONNREFUSED:
		printf("接続の試行が強く拒否されました。\n");
		break;
	case WSAEHOSTUNREACH:
		printf("到達不能なホストに対してソケット操作が試行されました。\n");
		break;
	case WSANOTINITIALISED:
		printf("この関数を呼び出す前にWSAStartup関数の呼び出しが成功していなければなりません。\n");
		break;
	}
}
#endif