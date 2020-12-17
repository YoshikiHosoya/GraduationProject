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
		ORDER_NONE = 0,	// 指示なし
		ORDER_GUEST_BE_ACCEPT,	// 接続完了
		ORDER_SENDTEXT,			// テキスト送信
		ORDER_SENDPICTURE,		// ピクチャ送信
		ORDER_SENDSELECT,		// 選択を送信
		ORDER_SENDWAIT,			// 待ち状態の送信
	} SERVER_ORDER;

	typedef enum
	{
		CLIENT_CONNECT,	// 接続
		CLIENT_CLOSE,	// 接続を閉じる
	} CLIENTSTATE;

	typedef enum
	{
		SEND_OBJECT,	// 送信する物のタイプ
		SEND_DATA,		// 送信するデータ
		SEND_MAX
	} SENDINFO;

	typedef struct
	{	// ソケット情報
		char ip_Addr[64];	// IPアドレス
		u_short port;		// ポート番号
	} SOCKINFO;

	CClient() {};	// コンストラクタ
	~CClient() {};		// デストラクタ

	static HRESULT LoadIP(void);		// IPアドレスのロード
	static int ConnectServer(void);		// サーバー接続処理
	static void WaitRecieve(void);		// 受信待ち処理
	static HRESULT InitClient(void);	// 初期化
	static void UninitClient(void);		// 強制終了

#ifdef _DEBUG
	static void ErrorReport(void);		// エラーレポート
#endif

	static bool &GetAccept(void)					{ return m_bAccept; }		// 接続の取得
	static bool &GetAcceptOther(void)				{ return m_bAcceptOther; }	// もう一方のプレイヤーの接続の取得
	static CLIENTSTATE &GetClientState(void)		{ return m_state; }			// 状態の取得
	static void SetAccepting(bool bAccept)			{ m_bAccept = bAccept; }	// 接続の設定
	static void SetClientState(CLIENTSTATE state)	{ m_state = state; }		// 状態の設定

	static void SendText(void);													// 文章の送信
	static void SendPicture(void);												// ピクチャの送信
	static void RecvText(char *data);											// 文章の受信
	static void RecvPicture(char *data);										// ピクチャの受信
	static void SendSelect(int nSelect);										// 選択の送信
	static void RecvSelect(char *data);											// 選択の受信
	static void SendWait(void);													// 待ち状態の送信
	static void RecvWait(void);													// 待ち状態の受信

private:
	static CLIENTSTATE m_state;			// クライアントの状態
	static bool m_bAccept;			// クライアント受け入れのフラグ
	static bool m_bAcceptOther;		// もう一方のプレイヤーの接続フラグ
	static SOCKET m_socket;				// 通信に使うソケット
	static SOCKINFO m_sockInfo;			// ソケットの情報
};

#endif
