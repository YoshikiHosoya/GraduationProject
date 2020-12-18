// ===================================================================
//
// 接続モード処理 [ connectMode.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CONNECTMODE_H_
#define _CONNECTMODE_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "basemode.h"

// ===================================================================
// マクロ定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CConnectBase;
class CChatBase;

class CConnectMode : public CBaseMode
{
public:
	CConnectMode();
	~CConnectMode();
	HRESULT Init(HWND hWnd);								//初期化
	void Update();											//更新
	void Draw();											//描画
	void ShowDebugInfo();									//デバッグ情報表記

private:
	std::shared_ptr<CConnectBase> m_pConnectBase;
	S_ptr<CChatBase> m_pChatBase;							// チャットのポインタ
};

#endif
