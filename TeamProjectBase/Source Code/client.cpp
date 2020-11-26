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

// ===================================================================
// マクロ定義
// ===================================================================
#define IPADDRESS_SERVER	("172.16.11.199")	// サーバのIPアドレス
#define PORT_SERVER			(12345)				// サーバのポート番号
#define VERSION_WINSOCK		(2)					// winsockのバージョン
#define LINK_SENDPICTURE	("data/SAVEDATA/PictureTextures/PicTex.txt")	// 送信用ピクチャのパス

#define SIZE_SEND_TEXT		(9)					// SEND_TEXTのサイズ
#define SIZE_SEND_PICTURE	(12)				// SEND_PICTUREのサイズ
#define SIZE_SPACE			(2)					// 文字列のスペースのサイズ

#define NUM_CMPCHAR			(6)					// 受信時のテキストかピクチャかの比較用

// ===================================================================
// 静的メンバ変数の初期化
// ===================================================================
bool CClient::m_bConnecting = false;
SOCKET CClient::m_socket = NULL;

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

	// ソケットの作成
	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT_SERVER);
	server.sin_addr.S_un.S_addr = inet_addr(IPADDRESS_SERVER);

	while (!m_bConnecting)
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
		m_bConnecting = true;
	}

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
		if (!m_bConnecting)
		{
			break;
		}

		char cData[256];
		memset(cData, 0, sizeof(cData));

		// 受信待ち
		if (recv(m_socket, cData, sizeof(cData), 0) <= 0)
		{
			// 受信していない
			continue;
		}

		// ピクチャ受信
		if (strncmp(cData, "SEND_PICTURE", NUM_CMPCHAR) == 0)
		{
			RecvPicture(cData);
		}
		// テキスト受信
		else if (strncmp(cData, "SEND_TEXT", NUM_CMPCHAR) == 0)
		{
			RecvText(cData);
		}
	}
}

// ===================================================================
// サーバーの初期化
// ===================================================================
HRESULT CClient::InitClient(void)
{
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
	if (m_bConnecting)
		m_bConnecting = false;
	// winsock2の終了処理
	WSACleanup();
}

// ===================================================================
// テキストの送信
// ===================================================================
void CClient::SendText(void)
{
	if (!m_bConnecting)
	{
#ifdef _DEBUG
		printf("サーバーに接続されていません\n");
#endif
		//return;
	}

	// 文字列を格納するメモリを確保
	char *cSendText = new char[SIZE_CHATTEXT];
	char *cSendSize = new char[2];

	// メモリリセット
	memset(cSendText, 0, sizeof(cSendText));
	memset(cSendSize, 0, sizeof(cSendSize));

	// 事前にデータを格納・計算
	strcpy(cSendText, CChatTab::GetSendText()->GetChatText().c_str());
	sprintf(cSendSize, "%d", strlen(cSendText));

	// 送信用文字列のメモリ確保
	char *cSendAll = new char[SIZE_SEND_TEXT + strlen(cSendSize) + SIZE_CHATTEXT + SIZE_SPACE];
	memset(cSendAll, 0, sizeof(cSendAll));

	// 送信データをまとめる
	strcpy(cSendAll, "SEND_TEXT");
	strcat(cSendAll, " ");
	strcat(cSendAll, cSendSize);
	strcat(cSendAll, " ");
	strcat(cSendAll, cSendText);

#ifdef _DEBUG
	// 送信データをデバッグで表示
	printf("テキスト送信 > %s [%d字]\n", cSendAll, strlen(cSendAll));
#endif

	// データ送信
	send(m_socket, cSendAll, strlen(cSendAll), 0);

	// 履歴に追加
	CChatTab::CreateKeep(CChatBase::OWNER_OWN, cSendText);

	// 記入した文字列をリセット
	CChatTab::GetSendText()->GetChatText().clear();

	// メモリ破棄
	delete[] cSendAll;
	delete[] cSendSize;
	delete[] cSendText;
}

// ===================================================================
// ピクチャの送信
// ===================================================================
void CClient::SendPicture(void)
{
	if (!m_bConnecting)
	{
#ifdef _DEBUG
		printf("サーバーに接続されていません\n");
#endif
		//return;
	}

	// 変数宣言
	CLoadFile Load;
	CLoadFile::LOADRESULT result;	// 結果

	// 読み込みファイルの情報を作成
	if ((result = Load.LoadFileIntoString(LINK_SENDPICTURE)) != CLoadFile::LR_SUCCESS)
	{
		// エラー
#ifdef _DEBUG
		printf("ピクチャの情報を読み込めません\n");
#endif
		// ファイルデータの破棄
		Load.DeleteFileData();
		return;
	}

	int nPixel = strlen(Load.m_pFileData);

	// 文字列を格納するメモリを確保
	char *cSendPicture = new char[Load.m_nuFileSize];
	char *cSendSize = new char[5];
	memset(cSendPicture, 0, sizeof(cSendPicture));
	memset(cSendSize, 0, sizeof(cSendSize));

	// 事前にデータを格納・計算
	strcpy(cSendPicture, Load.m_pFileData);
	sprintf(cSendSize, "%d", nPixel);

	// 送信用文字列のメモリ確保
	char *cSendAll = new char[SIZE_SEND_PICTURE + strlen(cSendSize) + nPixel + SIZE_SPACE];
	memset(cSendAll, 0, sizeof(cSendAll));

	// 送信データをまとめる
	strcpy(cSendAll, "SEND_PICTURE");
	strcat(cSendAll, " ");
	strcat(cSendAll, cSendSize);
	strcat(cSendAll, " ");
	strcat(cSendAll, cSendPicture);

#ifdef _DEBUG
	// 送信データをデバッグで表示
	printf("ピクチャ送信 > %s [%dPixel]\n", cSendAll, strlen(cSendAll));
#endif

	// データ送信
	send(m_socket, cSendAll, strlen(cSendAll), 0);

	// ファイルデータの破棄
	Load.DeleteFileData();

	// 空のピクチャを生成
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (FAILED(CManager::GetRenderer()->GetDevice()->CreateTexture(128, 128, 1, 0, D3DFMT_A32B32G32R32F,
		D3DPOOL_MANAGED, &pTexture, NULL)))
	{
		throw E_FAIL;
	}
	// ピクチャの情報を格納
	CString link;
	link = LINK_SENDPICTURE;
	CPicture::Reading(pTexture, link);
	// 履歴に追加
	CChatTab::AddPicture(CChatBase::OWNER_OWN, pTexture);
}

// ===================================================================
// テキストの受信
// ===================================================================
void CClient::RecvText(char *data)
{
	// 文字数を取得
	int nLen;
	char cText[65];
	memset(cText, 0, sizeof(cText));
	sscanf(data, "SEND_TEXT %d %s", &nLen, cText);
	if (nLen <= 0)
		return;

#ifdef _DEBUG
	// テキストがあれば表示
	printf("テキストを受信 > %s [%d字]\n", cText, nLen);
#endif

	// 末尾にnull文字
	cText[nLen] = '\0';

	// チャットにテキストを追加
	CChatTab::RecvChatText(cText);
}

// ===================================================================
// ピクチャの受信
// ===================================================================
void CClient::RecvPicture(char *data)
{
	// 文字数を取得
	int nLen;
	sscanf(data, "SEND_TEXT %d", &nLen);
	if (nLen <= 0)
		return;

	char *cPicData = new char[nLen + 1];
	memset(cPicData, 0, sizeof(cPicData));
	sscanf(data, "SEND_TEXT %d %s", &nLen, cPicData);

#ifdef _DEBUG
	// テキストがあれば表示
	printf("ピクチャを受信 > %s [%dPixel]\n", cPicData, nLen);
#endif

	// 格納用
	LPDIRECT3DTEXTURE9 pTexture = NULL;

	if (FAILED(CManager::GetRenderer()->GetDevice()->CreateTexture(128, 128, 1, 0, D3DFMT_A32B32G32R32F,
		D3DPOOL_MANAGED, &pTexture, NULL)))
	{
		throw E_FAIL;
	}

	// ファイルを上書き
	FILE *pFile;
	pFile = fopen(LINK_SENDPICTURE, "w");
	fprintf(pFile, "%s", cPicData);
	fclose(pFile);

	delete[] cPicData;

	CString link;
	link = LINK_SENDPICTURE;
	CPicture::Reading(pTexture, link);

	CChatTab::AddPicture(CChatBase::OWNER_GUEST, pTexture);
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