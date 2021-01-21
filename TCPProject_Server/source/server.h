// ===================================================================
//
// サーバー処理 [ server.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _SERVER_H_
#define _SERVER_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"

// ===================================================================
// マクロ定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CServer
{
public:
	typedef enum
	{
		ORDER_NONE = 0,			// 指示なし
		ORDER_GUEST_BE_ACCEPT,	// 接続成功

		ORDER_SENDTEXT,			// テキスト送信
		ORDER_SENDPICTURE,		// ピクチャ送信

		ORDER_SENDSELECT,		// 選択送信
		ORDER_SENDWAIT,			// 待ち状態の送信

		ORDER_GAMEOVER,			// ゲームオーバー
		ORDER_GAMECLEAR,		// ゲームクリア
		ORDER_END_GAME,			// ゲーム終了
		ORDER_RETRY,			// リトライ
		ORDER_CLOSED_SOCKET,	// 接続終了

		ORDER_MAX
	} CLIENT_ORDER;

	CServer() {};	// コンストラクタ
	~CServer() {};		// デストラクタ

	static int main(void);						// メイン処理
	static HRESULT InitServer(void);			// 初期化
	static void InitErrorReport(int err);		// エラーレポートの表示
	static void SocketErrorReport(int err);		// エラーレポートの表示
	static void UninitServer(void);				// 強制終了

	static void Update(void);

	static void SetClose(bool bClose) { m_bCloseServer = bClose; }

private:
	static void Recv(int nPlayer);
	static bool CountPacket(char *order, int nLen, int *nPacket);
	static void Order(int nPlayer, char* data);
	static void ReSendText(int nPlayer, char* data);
	static void ReSendPicture(int nPlayer, char* data);
	static void ReSendCommand(int nPlayer, char* data);
	static void ReSendGameEnd(int nPlayer, char* data);
	static void ReSendResult(int nPlayer, char * data);
	static void ClosePlayerSocket(int nPlayer);
	static bool m_bCloseServer;					// サーバーの終了フラグ
	static bool m_bCloseSocket;					// ソケットの終了フラグ
	static SOCKET m_socket;						// 自分のソケット
	static std::vector<SOCKET> m_PlayerSocket;	// 2人のソケットを格納する変数

	static bool m_bAccept[2];
};

#endif
