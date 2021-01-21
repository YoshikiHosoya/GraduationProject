// ===================================================================
//
// サーバ処理 [ main.cpp ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "server.h"
#include <string.h>
#include <errno.h>

// ===================================================================
// マクロ定義
// ===================================================================
#define PORT_SERVER			(12345)			// サーバのポート番号
#define MAX_BACKLOG			(2)				// 接続要求できる最大の数
#define VERSION_WINSOCK		(2)				// winsockのバージョン

#define NUM_CMPCHAR			(6)				// 比較する文字数

#define LINK_SENDTEXT		("data/SendText.txt")					// 送信用テキストのパス
#define LINK_SENDPICTURE	("data/SendPicture.txt")				// 送信用ピクチャのパス

// ===================================================================
// 静的メンバ変数の初期化
// ===================================================================
SOCKET CServer::m_socket = NULL;
std::vector<SOCKET> CServer::m_PlayerSocket = {};
bool CServer::m_bCloseServer = false;
bool CServer::m_bCloseSocket = false;
bool CServer::m_bAccept[2] = {};

// ===================================================================
// メイン関数
// ===================================================================
int CServer::main()
{
	printf("サーバーを開始\n");
	
	while (!m_bCloseServer)
	{
		// winsockの初期化
		if (InitServer() == E_FAIL)
		{
			// エラーによる強制終了
			return 0;
		}

		struct sockaddr_in client;
		int len;

		// TCPクライアントからの接続要求を待てる状態にする
		listen(m_socket, MAX_BACKLOG);

		// TCPクライアントからの接続要求を受け付ける
		len = sizeof(client);

		SOCKET sock[2];

		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// 接続確認
			sock[nCnt] = accept(m_socket, (struct sockaddr *)&client, &len);

			// エラー
			if (sock[nCnt] == -1)
			{
				// エラーレポート表示
				SocketErrorReport(errno);
				return 0;
			}

			// 接続したプレイヤーを表示
			printf("player[%d] と接続完了\n", nCnt);
			// ソケットを設定
			m_PlayerSocket.push_back(sock[nCnt]);
			m_bAccept[nCnt] = true;
		}

		for (int nPlayer = 0; nPlayer < MAX_PLAYER; nPlayer++)
		{
			// ゲストとの接続完了
			char cOrder[1];
			cOrder[0] = ORDER_GUEST_BE_ACCEPT;
			// 送信
			send(m_PlayerSocket[nPlayer], cOrder, 1, 0);
		}

		if (!m_bCloseSocket)
		{
			// 別スレッドで処理
			std::thread thread(CServer::Update);
			thread.detach();

			// サーバー終了までループ
			while (!m_bCloseSocket) {}
		}

		// サーバーの終了
		UninitServer();
	}

	return 0;
}

// ===================================================================
// サーバーの初期化
// ===================================================================
HRESULT CServer::InitServer(void)
{
	WSADATA wsaData;
	int err;
	m_bCloseSocket = false;
	m_bCloseServer = false;
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_bAccept[nCnt] = false;
	}

	// サーバ・クライアント共通で開始時のみ、必ず行う
	err = WSAStartup(MAKEWORD(VERSION_WINSOCK, 0), &wsaData);

	// エラー無し
	if (err != 0)
	{
		// コンソールに表示
		printf("サーバーの初期化失敗\n");
		// エラーメッセージ
		InitErrorReport(err);
		// winsock2の終了処理
		WSACleanup();
		// 失敗
		return E_FAIL;
	}

	// コンソールに表示
	printf("サーバーの初期化完了\n");

	struct sockaddr_in addr;	// ソケットに格納するためのアドレス

	// ソケットの作成
	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	// 接続待ちするIPアドレスとポートをソケットに設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_SERVER);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	// ソケットをバインド
	bind(m_socket, (struct sockaddr *)&addr, sizeof(addr));

	// 成功
	return S_OK;
}

// ===================================================================
// エラーメッセージ
// ===================================================================
void CServer::InitErrorReport(int err)
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
// エラーメッセージ
// ===================================================================
void CServer::SocketErrorReport(int err)
{
	printf("！！！エラー発生！！！\n");
	printf("ErrorCode : %d\n", err);

	switch (err)
	{
		// ネットワークが不通
	case ENETDOWN:
		printf("ネットワークが不通になっています。\n");
		break;
		// プロトコルエラー
	case EPROTO:
		printf("プロトコルエラーが発生しました\n");
		break;
		// 指定されたプロトコルが使用できない
	case ENOPROTOOPT:
		printf("指定されたプロトコルが使用できませんでした。\n");
		break;
		// 指定されたソケットではサポートされていない操作
	case EOPNOTSUPP:
		printf("この操作は指定されたソケットではサポートされていません。\n");
		break;
		// ホストに到達できていない
	case EHOSTUNREACH:
		printf("ホストに到達できていません。\n");
		break;
	}
}

// ===================================================================
// サーバーの強制終了
// ===================================================================
void CServer::UninitServer(void)
{
	for (int nCnt = 0; nCnt < (int)m_PlayerSocket.size(); nCnt++)
	{
		// ソケットの終了
		if (m_PlayerSocket[nCnt])
		{
			closesocket(m_PlayerSocket[nCnt]);
		}
	}

	// メモリのクリア
	m_PlayerSocket.clear();	

	printf("サーバーをクリーンアップ\n");
	// winsock2の終了処理
	WSACleanup();
}

// ===================================================================
// 更新
// ===================================================================
void CServer::Update(void)
{
	fd_set fds, readfds;
	// fd_setの初期化
	FD_ZERO(&readfds);
	// selectで待つ読み込みソケットとして playerSocket を登録
	for (int nPlayer = 0; nPlayer < MAX_PLAYER; nPlayer++)
	{
		FD_SET(m_PlayerSocket[nPlayer], &readfds);
	}

	// サーバーの終了までループ
	while (!m_bCloseSocket)
	{
		// 読み込み用fd_setの初期化 
		// (selectで毎回上書きされるので、こちらも毎回初期化する)
		memcpy(&fds, &readfds, sizeof(fd_set));

		// fdsに設定されたソケットが読み込み可能になるまで待つ
		select(0, &fds, NULL, NULL, NULL);

		for (int nPlayer = 0; nPlayer < MAX_PLAYER; nPlayer++)
		{
			if (!m_bAccept[nPlayer])
				continue;

			// ソケットを読み込めなければ、処理しない
			if (!FD_ISSET(m_PlayerSocket[nPlayer], &fds))
				continue;

			// 受信処理
			Recv(nPlayer);
		}

		if (!m_bAccept[0] && !m_bAccept[1])
			m_bCloseSocket = true;
	}
}

// ===================================================================
// 受信処理
// ===================================================================
void CServer::Recv(int nPlayer)
{
	int nPacket = 0;
	int nLen = 0;
	// 受信用
	char cData[16600];
	memset(cData, 0, sizeof(cData));

	// ループ
	while (1)
	{
		// ソケット破棄後は処理しない
		if (m_PlayerSocket.size() <= 0)
			return;

		// 受信
		nLen = recv(m_PlayerSocket[nPlayer], &cData[nPacket], 16600, 0);
		// 受信していない
		if (nLen <= 0)
			return;

		// パケット全受信でカウント終了
		if (CountPacket(&cData[0], nLen, &nPacket))
			break;
	}

	// 指示毎の処理
	Order(nPlayer, cData);

	// パケットリセット
	nPacket = 0;
}

// ===================================================================
// パケットのカウント
// ===================================================================
bool CServer::CountPacket(char *order, int nLen, int * nPacket)
{
	// テキスト・ピクチャはパケットを統合する
	if (order[0] == ORDER_SENDTEXT || order[0] == ORDER_SENDPICTURE)
	{
		// 受信したパケ数を総パケ数に加算
		if (*nPacket < *(int*)&order[1])
		{
			*nPacket += nLen;
		}

		// 現在の受信状況
		if (order[0] == ORDER_SENDTEXT)
			printf("%dbyte受信 ( %d / %dbyte )\n", nLen, *nPacket, *(int*)&order[1] + 5);
		else
			printf("%dbyte受信 ( %d / %dbyte )\n", nLen, *nPacket, *(int*)&order[1] + 6);

		// 総パケ数が目標に達した
		if (*nPacket >= *(int*)&order[1])
		{
			return true;
		}
		return false;
	}
	// 他の指示は即完了
	else
	{
		*nPacket += nLen;
		// 現在の受信状況
		printf("%dbyte受信 ( %d / %dbyte )\n", nLen, *nPacket, strlen(order));
		// 存在の確認
		if (nLen >= 1)
		{
			return true;
		}
		return false;
	}
}

// ===================================================================
// 指示毎の処理
// ===================================================================
void CServer::Order(int nPlayer, char * data)
{
	// 指示分け
	switch (data[0])
	{
	case ORDER_SENDPICTURE:		ReSendPicture(nPlayer, data); break;
	case ORDER_SENDTEXT:		ReSendText(nPlayer, data); break;
	case ORDER_SENDSELECT:		ReSendCommand(nPlayer, data); break;
	case ORDER_SENDWAIT:		ReSendCommand(nPlayer, data); break;
	case ORDER_CLOSED_SOCKET:	ClosePlayerSocket(nPlayer); break;
	case ORDER_END_GAME:		ReSendResult(nPlayer, data); break;
	case ORDER_GAMEOVER:		ReSendGameEnd(nPlayer, data); break;
	case ORDER_GAMECLEAR:		ReSendGameEnd(nPlayer, data); break;
	case ORDER_RETRY:			ReSendResult(nPlayer, data);  break;
	}
}

// ===================================================================
// ピクチャの受信
// ===================================================================
void CServer::ReSendPicture(int nPlayer, char* data)
{
	// サイズ
	int *pSize = (int*)&data[1];
	// ピクチャのデータを格納
	char *cPicData = new char[*pSize + 1];
	memcpy(cPicData, &data[5], *pSize);

	FILE *fp;

	// ファイルを開く
	if ((fp = fopen(LINK_SENDPICTURE, "wb")) == NULL)
	{
		// エラー
		printf("テキスト読み込みエラー\n");
		delete[] cPicData;
		return;
	}

	fwrite(cPicData, *pSize, 1, fp);

	// ファイルを閉じる
	fclose(fp);

	if (nPlayer == 0)
	{
		printf("player[0] > player[1] へピクチャ送信\n");
		send(m_PlayerSocket[1], data, *pSize + 5 + 1, 0);
	}
	else
	{
		printf("player[1] > player[0] へピクチャ送信\n");
		send(m_PlayerSocket[0], data, *pSize + 5 + 1, 0);
	}

	delete[] cPicData;
}

// ===================================================================
// テキストの受信
// ===================================================================
void CServer::ReSendText(int nPlayer, char* data)
{
	// サイズ
	int *pSize = (int*)&data[1];

	char *cTextData = new char[*pSize];
	memcpy(cTextData, &data[5], *pSize);

	FILE *fp;

	// ファイルを開く
	if ((fp = fopen(LINK_SENDTEXT, "wb")) == NULL)
	{
		// エラー
		printf("テキスト読み込みエラー\n");
		delete[] cTextData;
		return;
	}

	fwrite(cTextData, *pSize, 1, fp);

	// ファイルを閉じる
	fclose(fp);

	if (nPlayer == 0)
	{
		printf("player[0] > player[1] へテキスト送信\n");
		send(m_PlayerSocket[1], data, *pSize + 5, 0);
	}
	else
	{
		printf("player[1] > player[0] へテキスト送信\n");
		send(m_PlayerSocket[0], data, *pSize + 5, 0);
	}

	delete[] cTextData;
}

// ===================================================================
// 選択の受信
// ===================================================================
void CServer::ReSendCommand(int nPlayer, char * data)
{
	if (nPlayer == 0)
	{
		printf("player[0] > player[1] へオーダー送信\n");
		send(m_PlayerSocket[1], data, 2, 0);
	}
	else
	{
		printf("player[1] > player[0] へオーダー送信\n");
		send(m_PlayerSocket[0], data, 2, 0);
	}
}

// ===================================================================
// ゲーム完了の受信
// ===================================================================
void CServer::ReSendGameEnd(int nPlayer, char * data)
{
	if (nPlayer == 0)
	{
		printf("player[0] > player[1] へゲーム完了を送信\n");
		send(m_PlayerSocket[1], data, 5, 0);
	}
	else
	{
		printf("player[1] > player[0] へゲーム完了を送信\n");
		send(m_PlayerSocket[0], data, 5, 0);
	}
}

// ===================================================================
// ゲームの終了
// ===================================================================
void CServer::ReSendResult(int nPlayer, char * data)
{
	if (nPlayer == 0)
	{
		printf("player[0] > player[1] へリザルトの選択を送信\n");
		send(m_PlayerSocket[1], data, 2, 0);
	}
	else
	{
		printf("player[1] > player[0] へリザルトの選択を送信\n");
		send(m_PlayerSocket[0], data, 2, 0);
	}
}

// ===================================================================
// ソケットの終了
// ===================================================================
void CServer::ClosePlayerSocket(int nPlayer)
{
	printf("player[%d] のソケットを終了\n", nPlayer);
	// プレイヤーソケットを破棄
	if (m_PlayerSocket[nPlayer])
		closesocket(m_PlayerSocket[nPlayer]);
	m_bAccept[nPlayer] = false;
}
