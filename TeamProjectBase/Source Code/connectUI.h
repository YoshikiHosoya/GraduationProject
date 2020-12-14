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
class CMouse;

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
		CONNECTUI_ONLY_CAUTION_LEVEL,	// レベル選択の注意
		CONNECTUI_ONLY_MAX
	} CONNECTUITYPE_ONLY;

	typedef enum
	{	// 接続モードのフロー
		CONNECTFLOW_CONNECTING = 0,	// 接続中
		CONNECTFLOW_CONNECTED,		// 接続完了
		CONNECTFLOW_SELECT_MODE,	// モード選択
		CONNECTFLOW_SELECT_LEVEL,	// 難易度選択
		CONNECTFLOW_END
	} CONNECTFLOW_TYPE;

	typedef enum
	{	// フローの状態管理
		FLOWSTATE_BEGIN,	// 開始時
		FLOWSTATE_NORMAL,	// 通常時
		FLOWSTATE_END,		// 終了時
		FLOWSTATE_WAIT,		// 待ち状態
	} FLOW_STATE;

	typedef struct
	{	// UIの情報
		D3DXVECTOR2 pos;			// 座標
		D3DXVECTOR2 size;			// サイズ
		D3DXCOLOR color;			// カラー
	} CONNECTUI_INFO;

	typedef enum
	{
		PLAYER_ONE = 0,	// プレイヤー1
		PLAYER_TWO,		// プレイヤー2
		PLAYER_MAX		// 最大プレイヤー
	} NUM_PLAYER;

	typedef enum
	{	// 選択しているモード
		SELECTMODE_NONE = 0,	// 何もない
		SELECTMODE_REMOVE,		// 解除
		SELECTMODE_SOLVE,		// 解読
		SELECTMODE_MAX
	} SELECTMODE;

	typedef enum
	{	// 選択しているレベル
		SELECTLEVEL_NONE = 0,	// 何もない
		SELECTLEVEL_EASY,		// イージー
		SELECTLEVEL_NORMAL,		// ノーマル
		SELECTLEVEL_HARD,		// ハード
		SELECTLEVEL_MAX
	} SELECTLEVEL;

	CConnectUI();						// コンストラクタ
	~CConnectUI();						// デストラクタ

	HRESULT Init(void);					// 初期化
	void Update(void);					// 更新
	void Draw(void);					// 描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	static CConnectUI * Create(void);	// 生成
	static HRESULT Load(void);			// ロード

	static void SetGuestMode(SELECTMODE mode);
	static void SetGuestLevel(SELECTLEVEL level);
	static void EnableGuestWait(void) { m_bGuestWait = true; }

private:
	static void ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);
	static void SetBothInfo(CONST_STRING str, CONST_STRING type);
	static void SetOnlyInfo(CONST_STRING str, CONST_STRING type);

	static CPolygon2D *CreateBothUI(int nPlayer, CONNECTUITYPE_BOTH type);
	CPolygon2D *CreateOnlyUI(CONNECTUITYPE_ONLY type);
	static void DeleteBothUI(int nPlayer, CONNECTUITYPE_BOTH type);
	void DeleteOnlyUI(CONNECTUITYPE_ONLY type);

	void Connecting(void);									// 接続中の処理
	void CheckConnect(void);
	void Connected(void);									// 接続完了時の処理
	void SelectMode(void);									// モードの選択処理
	void SelectLevel(void);									// レベルの選択処理

	void Begin(CONNECTFLOW_TYPE flow);						// フローごとの開始時の処理
	void End(CONNECTFLOW_TYPE flow);						// フローごとの終了時の処理
	void Wait(CONNECTFLOW_TYPE flow);						// フローごとの待ち状態の処理

	void LoadIconAnim(void);								// ロードアイコンのアニメーション処理
	void ButtonAnim(void);									// ボタンのアニメーション処理
	void CheckSameMode(void);								// 同じモードを選択していないか確認処理
	void ClickDecide(CMouse *pMouse);						// 決定ボタンを押す

	static CPolygon2D *m_pUIBoth[2][CONNECTUI_BOTH_MAX];	// 両方で使うUIのポインタ
	static CPolygon2D *m_pUIOnly[CONNECTUI_ONLY_MAX];		// 一つだけ使うUIのポインタ
	static CONNECTUI_INFO m_UIInfoBoth[2][CONNECTUI_BOTH_MAX];	// 両方で使うUIの情報
	static CONNECTUI_INFO m_UIInfoOnly[CONNECTUI_ONLY_MAX];		// 一つだけ使うUIの情報

	int m_nCntAnim;
	int m_nCntPattern;
	int m_nCntState;

	static CONNECTFLOW_TYPE m_flow;				// 接続モードのフロー
	FLOW_STATE m_state;							// フローの状態

	bool m_bConnect[PLAYER_MAX];				// 接続のフラグ
	static int m_nSelectMode[PLAYER_MAX];		// プレイヤーが選択中の番号
	static int m_nSelectLevel[PLAYER_MAX];		// プレイヤーが選択中の番号
	static bool m_bGuestWait;					// ゲストの待ち状態のフラグ
};

#endif
