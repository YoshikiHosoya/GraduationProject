//------------------------------------------------------------------------------
//
//タイトル処理  [title.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_
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
class CScene2D;
class CBomb;

class CTitle : public CBaseMode
{
public:
	//状態
	enum STATE
	{
		STATE_NONE = -1,				//無し
		STATE_START,					//最初の演出
		STATE_NORMAL,					//通常画面
		STATE_BLACKOUT,					//暗転
		STATE_FADE,						//フェード
		STATE_GAMEEND,					//ゲーム終了
	};
	CTitle();							//コンストラクタ
	~CTitle();							//デストラクタ

	HRESULT Init(HWND hWnd);			//初期化
	void Update();						//更新
	void Draw();						//描画
	void ShowDebugInfo(){};				//デバッグ情報表記

	void Collision();					//当たり判定処理
	void StateUpdate();					//ステート更新
	void SetState(STATE state);			//ステート切り替え

private:
	STATE m_titlestate;					//ステート
	int m_nCntState;					//ステートのカウント

	S_ptr<CScene2D> m_pStart;			//スタート
	S_ptr<CScene2D> m_pEnd;				//エンド


	S_ptr<CBomb>     m_pBomb;			//ボムのポインタ


};

#endif