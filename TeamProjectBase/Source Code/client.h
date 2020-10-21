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
	static void ErrorReport(int err);	// エラーレポート

	static bool &GetAccepting(void) { return m_bAccepting; }
	static void SetAccepting(bool bAccept) { m_bAccepting = bAccept; }

private:
	static bool m_bAccepting;			// クライアント受け入れのフラグ
};

#endif
