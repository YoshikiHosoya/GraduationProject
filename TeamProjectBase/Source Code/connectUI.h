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
#include "Mylibrary.h"

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
	{	// 1P2P両方で使うUI
		CONNECTUI_BOTH_BACK,				// 背景
		CONNECTUI_BOTH_LOADICON,			// ロードアイコン
		CONNECTUI_BOTH_STATE_CONNECTING,	// 接続中
		CONNECTUI_BOTH_STATE_CONNECTED,		// 接続完了
		CONNECTUI_BOTH_STATE_SELECTING,		// 選択中
		CONNECTUI_BOTH_STATE_SELECTED,		// 選択完了
		CONNECTUI_BOTH_SELECT_MODE,			// 選択 モード
		CONNECTUI_BOTH_SELECT_LEVEL,		// 選択 レベル
		CONNECTUI_BOTH_MODE_REMOVE,			// モード 解除
		CONNECTUI_BOTH_MODE_SOLVE,			// モード 解読
		CONNECTUI_BOTH_LEVEL_EASY,			// 難易度 イージー
		CONNECTUI_BOTH_LEVEL_NORMAL,		// 難易度 ノーマル
		CONNECTUI_BOTH_LEVEL_HARD,			// 難易度 ハード
		CONNECTUI_BOTH_MAX
	} CONNECTUITYPE_BOTH;

	typedef enum
	{	// 一つだけ使うUI
		CONNECTUI_ONLY_BUTTON_DESIDE,	// 決定ボタン
		CONNECTUI_ONLY_COMMENT_REMOVE,	// コメント 解除
		CONNECTUI_ONLY_COMMENT_SOLVE,	// コメント 解読
		CONNECTUI_ONLY_COMMENT_EASY,	// コメント イージー
		CONNECTUI_ONLY_COMMENT_NORMAL,	// コメント ノーマル
		CONNECTUI_ONLY_COMMENT_HARD,	// コメント ハード
		CONNECTUI_ONLY_CAUTION_MODE,	// モード選択の注意
		CONNECTUI_ONLY_MAX
	} CONNECTUITYPE_ONLY;

	typedef enum
	{	// 接続モードのフロー
		CONNECTFLOW_CONNECTING,		// 接続中
		CONNECTFLOW_CONNECTED,		// 接続完了
		CONNECTFLOW_SELECT_MODE,	// モード選択
		CONNECTFLOW_SELECT_LEVEL,	// 難易度選択
	} CONNECTFLOW;

	typedef struct
	{	// UIの情報
		D3DXVECTOR2 pos;			// 座標
		D3DXVECTOR2 size;			// サイズ
		D3DXCOLOR color;			// カラー
	} CONNECTUI_INFO;

	CConnectUI();						// コンストラクタ
	~CConnectUI();						// デストラクタ

	HRESULT Init(void);					// 初期化
	void Update(void);					// 更新
	void Draw(void);					// 描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	static CConnectUI * Create(void);	// 生成
	static HRESULT Load(void);			// ロード

private:
	static void ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);
	static void SetBothInfo(CONST_STRING str, CONST_STRING type);
	static void SetOnlyInfo(CONST_STRING str, CONST_STRING type);

	CPolygon2D *CreateBothUI(int nPlayer, CONNECTUITYPE_BOTH type);
	CPolygon2D *CreateOnlyUI(CONNECTUITYPE_ONLY type);

	void Connecting(void);									// 接続中の処理
	void ConnectPlayer(int nPlayer);						// 接続完了時の処理
	void Connected(void);									// 接続完了時の処理
	void SelectMode(void);									// モードの選択処理

	void Anim(void);										// アニメーション関数
	void UpdateCntAnim(void);								// アニメーションカウンタの更新

	static CPolygon2D *m_pUIBoth[2][CONNECTUI_BOTH_MAX];	// 両方で使うUIのポインタ
	static CPolygon2D *m_pUIOnly[CONNECTUI_ONLY_MAX];		// 一つだけ使うUIのポインタ
	static CONNECTUI_INFO m_UIInfoBoth[2][CONNECTUI_BOTH_MAX];	// 両方で使うUIの情報
	static CONNECTUI_INFO m_UIInfoOnly[CONNECTUI_ONLY_MAX];		// 一つだけ使うUIの情報

	int m_nCntAnim;
	int m_nCntPattern;
	int m_nCntState;

	CONNECTFLOW m_flow;					// 接続モードのフロー
};

#endif
