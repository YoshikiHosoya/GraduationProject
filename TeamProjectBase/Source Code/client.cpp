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
#include "picture.h"

// ===================================================================
// マクロ定義
// ===================================================================
#define IPADDRESS_SERVER	("172.16.11.199")	// サーバのIPアドレス
#define PORT_SERVER			(12345)				// サーバのポート番号
#define VERSION_WINSOCK		(2)					// winsockのバージョン
#define LINK_SENDPICTURE	("data/SAVEDATA/PictureTextures/PicTex.txt")	// 送信用ピクチャのパス

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
	char buf[256] = {};					// テキストを格納する変数
	size_t bufSize = sizeof(buf);	// サイズを格納する変数

	while (1)
	{
		// 接続しない
		if (!m_bConnecting)
		{
			break;
		}

		// メモリブロックをリセット
		memset(buf, 0, bufSize);

		// 接続待ち
		if (recv(m_socket, buf, bufSize, 0) == -1)
		{
#ifdef _DEBUG
			// エラーレポート
			ErrorReport();
#endif
			continue;
		}

		// テキストの受信
		if (strlen(buf) > 0)
			RecvText(buf);

		RecvPicture();
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
void CClient::SendText(char * cSendText)
{
	if (!m_bConnecting)
	{
#ifdef _DEBUG
		printf("サーバーに接続されていません\n");
#endif
		return;
	}

	char Text[65];
	strcpy(Text, cSendText);
	int nLen = strlen(Text);

#ifdef _DEBUG
	printf("送信 > %s [%d字]\n", Text, nLen);
#endif
	// テキスト送信
	if (send(m_socket, Text, nLen, 0) == -1)
	{
#ifdef _DEBUG
		ErrorReport();
#endif
	}
}

// ===================================================================
// テキストの受信
// ===================================================================
void CClient::RecvText(char * cRecvText)
{
	int nLen = strlen(cRecvText);
#ifdef _DEBUG
	printf("受信 > %s [%d字]\n", cRecvText, nLen);
#endif
	// チャットにテキストを追加
	CChatTab::RecvChatText(cRecvText);
}

// ===================================================================
// ピクチャの送信
// ===================================================================
void CClient::SendPicture(void)
{
	// 接続していない
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
	}

#ifdef _DEBUG
	printf("ピクチャ送信 > [%d byte]\n", Load.m_nuFileSize);
#endif

	// ピクチャ送信を伝える
	char sendTitle[16] = "SEND_PICTURE";
	send(m_socket, sendTitle, strlen(sendTitle), 0);

	// テキスト送信
	if (send(m_socket, Load.m_pFileData, Load.m_nuFileSize, 0) == -1)
	{
		// エラーレポート
#ifdef _DEBUG
		ErrorReport();
#endif
		// ファイルデータの破棄
		//Load.DeleteFileData();
		//return;
	}

	LPDIRECT3DTEXTURE9 pTexture = NULL;

	if (FAILED(CManager::GetRenderer()->GetDevice()->CreateTexture(128, 128, 1, 0, D3DFMT_A32B32G32R32F,
		D3DPOOL_MANAGED, &pTexture, NULL)))
	{
		throw E_FAIL;
	}

	CString link;
	link = LINK_SENDPICTURE;
	CPicture::Reading(pTexture, link);

	CChatTab::AddPicture(CChatBase::OWNER_OWN, pTexture);

	// ファイルデータの破棄
	Load.DeleteFileData();
}

// ===================================================================
// ピクチャの受信
// ===================================================================
void CClient::RecvPicture(void)
{
	// 格納用
	LPDIRECT3DTEXTURE9 pTexture = NULL;

	if (FAILED(CManager::GetRenderer()->GetDevice()->CreateTexture(128, 128, 1, 0, D3DFMT_A32B32G32R32F,
		D3DPOOL_MANAGED, &pTexture, NULL)))
	{
		throw E_FAIL;
	}

	//CString link;
	//link = LINK_SENDPICTURE;
	//CPicture::Reading(pTexture, link);

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