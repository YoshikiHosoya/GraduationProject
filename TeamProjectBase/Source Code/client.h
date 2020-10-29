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

// ===================================================================
// マクロ定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CClient
{
public:
	CClient() {};	// コンストラクタ
	~CClient() {};		// デストラクタ

	static int main(void);				// メイン処理
	static HRESULT InitClient(void);	// 初期化
	static void UninitClient(void);		// 強制終了
	static void ErrorReport(void);	// エラーレポート

	static bool &GetAccepting(void) { return m_bConnecting; }
	static void SetAccepting(bool bConnect) { m_bConnecting = bConnect; }
	static void Send(char* cSendText);
private:
	static bool m_bConnecting;			// クライアント受け入れのフラグ
	static char m_cSendText[256];		// 送るテキスト
	static SOCKET m_socket;				// 通信に使うソケット
};

#endif
