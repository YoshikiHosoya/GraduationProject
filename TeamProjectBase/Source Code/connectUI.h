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
		CONNECTUI_BACK_00,		// 1Pの背景
		CONNECTUI_BACK_01,		// 2Pの背景
		CONNECTUI_LOADICON_00,	// ロードアイコン
		CONNECTUI_LOADICON_01,	// ロードアイコン
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
	void UpdateCntAnim(void);			// アニメーションカウンタの更新
	void CreateBackTab(void);			// 背景タブの生成
	void CreateLoadIcon(void);			// ロードアイコンの生成
	void LoadIconAnim(CPolygon2D *pUI);		// ロードアイコンのアニメーション
	static CPolygon2D *m_pConnectUI[CONNECTUI_MAX];
	static D3DXVECTOR2 m_UIPos[CONNECTUI_MAX];
	static D3DXVECTOR2 m_UISize[CONNECTUI_MAX];

	int m_nCntAnim;
	int m_nCntPattern;
};

#endif
