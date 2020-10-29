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
#include "client.h"

// ===================================================================
// マクロ定義
// ===================================================================
#define IPADDRESS_SERVER	("172.16.11.199")	// サーバのIPアドレス
#define PORT_SERVER			(12345)				// サーバのポート番号
#define VERSION_WINSOCK		(2)					// winsockのバージョン

// ===================================================================
// 静的メンバ変数の初期化
// ===================================================================
bool CClient::m_bConnecting = false;
char CClient::m_cSendText[256] = {};
SOCKET CClient::m_socket = NULL;

// ===================================================================
// メイン関数
// ===================================================================
int CClient::main(void)
{
	struct sockaddr_in server;
	char buf[32];

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

	// サーバに接続
	connect(m_socket, (struct sockaddr *)&server, sizeof(server));

	while (1)
	{
		// 接続しない
		if (!m_bConnecting)
			break;

		// サーバからデータを受信
		memset(buf, 0, sizeof(buf));
		// 接続待ち
		int n = recv(m_socket, buf, sizeof(buf), 0);
		// 受信したデータを表示
		if (n > 0)
			printf("%d, %s\n", n, buf);
	}
	// winsock2の終了処理
	WSACleanup();

	return 0;
}

// ===================================================================
// サーバーの初期化
// ===================================================================
HRESULT CClient::InitClient(void)
{
	WSADATA wsaData;	// winsockのデータ
	int err;			// エラー番号

	// 要素の初期化
	m_bConnecting = true;

	// サーバ・クライアント共通で開始時のみ、必ず行う
	err = WSAStartup(MAKEWORD(VERSION_WINSOCK, 0), &wsaData);

	// エラー無し
	if (err == 0)
	{
		// 成功
		return S_OK;
	}

	// エラーメッセージ
	ErrorReport(err);
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
	m_bConnecting = false;
	// winsock2の終了処理
	WSACleanup();
}

// ===================================================================
// エラーメッセージ
// ===================================================================
void CClient::ErrorReport(int err)
{
	printf("！！！エラー発生！！！\n");

	printf("ErrorCode : %d\n", WSAGetLastError());

	switch (err)
	{
		// ネットワークサブシステムがネットワークへの接続を準備できていない
	case WSASYSNOTREADY:
		printf("ネットワークサブシステムがネットワークへの接続を準備できていません。\n");
		break;
		// 要求されたwinsockのバージョンがサポートされていない
	case WSAVERNOTSUPPORTED:
		printf("要求されたwinsockのバージョンがサポートされていません。\n");
		break;
		// ブロッキング操作の実行中であるか、 またはサービスプロバイダがコールバック関数を処理している
	case WSAEINPROGRESS:
		printf("ブロッキング操作の実行中であるか、 またはサービスプロバイダがコールバック関数を処理しています。\n");
		break;
		// winsockが処理できる最大プロセス数に達した
	case WSAEPROCLIM:
		printf("winsockが処理できる最大プロセス数に達しました。\n");
		break;
		// 第二引数であるlpWSAData は有効なポインタではない
	case WSAEFAULT:
		printf("第二引数である lpWSAData は有効なポインタではありません。\n");
		break;
	}

	// キー入力待ち
	printf("終了します。Enterキーを入力してください。\n");
	getchar();
}

// ===================================================================
// テキストの送信
// ===================================================================
void CClient::Send(char * cSendText)
{
	strcpy(m_cSendText, cSendText);
	// テキスト送信
	send(m_socket, m_cSendText, strlen(m_cSendText), 0);
	strcpy(m_cSendText, "");
}
