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
class CPolygon2D;

class CConnectMode : public CBaseMode
{
public:
	typedef enum
	{
		CONNECTUI_BACK_0,	// 背景1
		CONNECTUI_BACK_1,	// 背景2
		CONNECTUI_STATE_0,	// 状態1
		CONNECTUI_STATE_1,	// 状態2
		CONNECTUI_MAX
	} CONNECTUITYPE;

	CConnectMode();
	~CConnectMode();
	HRESULT Init(HWND hWnd);								//初期化
	void Update();											//更新
	void Draw();											//描画
	void ShowDebugInfo();									//デバッグ情報表記

private:
	CPolygon2D *m_pConnectUI[CONNECTUI_MAX];
	//static D3DXVECTOR3 m_posUI[CONNECTUI_MAX];
	//static D3DXVECTOR3 m_sizeUI[CONNECTUI_MAX];
};

#endif
