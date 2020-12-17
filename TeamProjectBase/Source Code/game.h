//------------------------------------------------------------------------------
//
//ゲーム処理  [game.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "basemode.h"
#include "Bomb.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CBomb;
class CChatBase;
class CPicture;
class CTablet;
class CGame : public CBaseMode
{
public:
	//ゲームの状態遷移
	enum STATE
	{
		STATE_NONE = -1,				//始まってない
		STATE_READY,					//開始前の状態
		STATE_NORMAL,					//通常状態　進行中
		STATE_PAUSE,					//ポーズ状態
		STATE_GAMEOVER,					//ゲームオーバー
		STATE_GAMECLEAR,				//ゲームクリア

		STATE_DEBUG,
		STATE_MAX
	};

	//ゲーム中の視線
	enum GAZE
	{
		GAZE_DEFAULT,	//デフォルトの視線
		GAZE_BOMB,		//爆弾を見てる
		GAZE_MODULE		//モジュールを見てる
	};


	CGame();
	~CGame();
	virtual HRESULT Init(HWND hWnd);						//初期化
	virtual void Update();									//更新
	virtual void Draw();									//描画
	virtual void ShowDebugInfo();							//デバッグ情報表記
	void UpdateState();										//ステートに応じた処理

	void SetState(STATE state);								//ゲームの状態設定
	void SetGaze(GAZE gaze);								//視線設定

	STATE GetState() { return m_State; };					//ゲームの状態取得
	GAZE GetGaze() { return m_Gaze; };						//現在の視線取得
	CBomb *GetBombPtr() { return m_pBomb.get(); };			//ボムのポインタ取得
	CPicture *GetPicture(void) { return m_pPicture.get(); }	// ピクチャポインタの取得
	CTablet * GetTablet(void) { return m_pTablet.get(); }	// タブレットポインタの取得
	CChatBase *GetChatBase() { return m_pChatBase.get(); };	// チャットベースポインタ取得

	static bool GetClearFlag() { return m_bClear; };			//クリアフラグ取得
	static void SetDifficulty(CBomb::DIFFICULTY Diff) { m_bDifficulty = Diff; }	// 難易度の設定
protected:

private:
	STATE            m_State;								//ゲームステート
	GAZE             m_Gaze;								//視線
	int              m_nCntState;							//ステートのカウンタ
	S_ptr<CBomb>     m_pBomb;								//ボムのポインタ
	S_ptr<CChatBase> m_pChatBase;							// チャットのポインタ
	S_ptr<CPicture>  m_pPicture;							// ピクチャポインタ
	S_ptr<CTablet>   m_pTablet;								// タブレットポインタ

	static bool m_bClear;									//クリアフラグ
	static CBomb::DIFFICULTY m_bDifficulty;					// ゲームの難易度
};

#endif