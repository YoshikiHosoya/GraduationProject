// ===================================================================
//
// 接続のUI処理 [ connectUI.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CONNECTUI_H_
#define _CONNECTUI_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "connectBase.h"

// ===================================================================
// マクロ定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CPolygon2D;

class CConnectUI
{
public:
	typedef enum
	{
		CONNECTUI_LOADICON,	// ロードアイコン
		CONNECTUI_MAX
	} CONNECTUITYPE;

	CConnectUI();
	~CConnectUI();

	HRESULT Init(void);			// 初期化
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void ShowDebugInfo() {};		//デバッグ情報表記

	static CConnectUI * Create(void);	// 生成
private:
	void CreateUI(void);		// UI生成
	void LoadAnim(CPolygon2D *pUI);		// ロードアイコンのアニメーション
	CPolygon2D *m_pConnectUI[CONNECTUI_MAX];
};

#endif
