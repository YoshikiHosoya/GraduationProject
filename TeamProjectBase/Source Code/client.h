// ===================================================================
//
// クライアント処理 [ client.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CLIENT_H_
#define _CLIENT_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "Mylibrary.h"

// ===================================================================
// マクロ定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CClient
{
public:
	typedef enum
	{
		CLIENT_CONNECT,	// 接続
		CLIENT_CLOSE,	// 接続を閉じる
	} CLIENTSTATE;

	CClient() {};	// コンストラクタ
	~CClient() {};		// デストラクタ

	static int ConnectServer(void);		// サーバー接続処理
	static void WaitRecieve(void);		// 受信待ち処理
	static HRESULT InitClient(void);	// 初期化
	static void UninitClient(void);		// 強制終了

#ifdef _DEBUG
	static void ErrorReport(void);		// エラーレポート
#endif

	static bool &GetAccepting(void)					{ return m_bConnecting; }		// 接続の取得
	static CLIENTSTATE &GetClientState(void)		{ return m_state; }				// 状態の取得
	static void SetAccepting(bool bConnect)			{ m_bConnecting = bConnect; }	// 接続の設定
	static void SetClientState(CLIENTSTATE state)	{ m_state = state; }			// 状態の設定
	static void SendText(char* cSendText);											// 文章の送信
	static void RecvText(void);														// 文章の受信
	static void SendPicture(void);													// ピクチャの送信
	static void RecvPicture(void);													// ピクチャの受信

private:
	static CLIENTSTATE m_state;			// クライアントの状態
	static bool m_bConnecting;			// クライアント受け入れのフラグ
	static SOCKET m_socket;				// 通信に使うソケット
};

#endif
