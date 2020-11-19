//------------------------------------------------------------------------------
//
//解読処理  [Decoding.h]
//Author:Ikuto Sekine
//
//------------------------------------------------------------------------------
#ifndef _DECODING_H_
#define _DECODING_H_

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
class CChatBase;
class CPicture;
class CTablet;
class CDecodingManager;
class CDecoding : public CBaseMode
{
public:
	// 状態遷移
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

	// 視線
	enum GAZE
	{
		GAZE_DEFAULT,	//デフォルトの視線
	};

	// 役割
	enum ROLE
	{
		ROLE_DECODING = 0,		// 解読
		ROLE_BOMBRELEASE,		// 爆弾解除
		ROLE_MAX				// 最大
	};

	CDecoding();											// コンストラクタ
	~CDecoding();											// デストラクタ
	virtual HRESULT Init(HWND hWnd);						//初期化
	virtual void Update();									//更新
	virtual void Draw();									//描画
	virtual void ShowDebugInfo();							//デバッグ情報表記
	void UpdateState();										//ステートに応じた処理

	void SetState(STATE state);								//ゲームの状態設定
	void SetGaze(GAZE gaze);								//視線設定

	STATE GetState() { return m_State; };					//ゲームの状態取得
	GAZE GetGaze() { return m_Gaze; };						//現在の視線取得
	CPicture *GetPicture(void) { return m_pPicture.get(); }	// ピクチャポインタの取得
	CTablet * GetTablet(void) { return m_pTablet.get(); }	// タブレットポインタの取得
	CDecodingManager * GetDecodingManager(void) { return m_pDecodingManager.get(); }
protected:

private:
	STATE            m_State;								// ゲームステート
	GAZE             m_Gaze;								// 視線
	int              m_nCntState;							//ステートのカウンタ
	S_ptr<CChatBase> m_pChatBase;							// チャットのポインタ
	S_ptr<CPicture>  m_pPicture;							// ピクチャポインタ
	S_ptr<CTablet>   m_pTablet;								// タブレットポインタ
	S_ptr<CDecodingManager> m_pDecodingManager;				// 解読書マネージャー
};

#endif