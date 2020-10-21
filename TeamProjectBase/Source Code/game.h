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
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
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
	virtual HRESULT Init(HWND hWnd);								//初期化
	virtual void Uninit();											//終了
	virtual void Update();											//更新
	virtual void Draw();											//描画
	virtual void ShowDebugInfo();									//デバッグ情報表記

	void SetGamestate(STATE gamestate);						//ゲームの状態設定
	STATE GetGamestate() { return m_gamestate; };			//ゲームの状態取得

protected:

private:
	STATE m_gamestate;									//ゲームステート
	int m_nCntState;									//ステートのカウンタ
};

#endif