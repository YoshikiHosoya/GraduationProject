//------------------------------------------------------------------------------
//
//チュートリアル処理  [tutorial.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
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
class CUI_Base;
class CTimer;

class CTutorial : public CBaseMode
{
public:
	CTutorial();
	~CTutorial();

	enum TUTORIL_TYPE
	{
		TUTORIAL_NONE = 0,
		TUTORIAL_REMOVE,	// 解除
		TUTORIAL_SOLVE,		// 解読
	};

	HRESULT Init(HWND hWnd);			//初期化
	void Update();						//更新
	void Draw();						//描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	void CreateUI();					//ＵＩ生成
	void UpdateState();					//ステート更新
	void Collision();					//判定

	void PageChange();					//ページ変更
	void Ready();						//準備完了

	static void GuestEndTutorial() { m_bGuestFlag = true; }		// ゲストの準備完了

private:
	int m_nCntState;					//カウンタ
	int m_nPage;						//ページ
	TUTORIL_TYPE m_type;				//タイプ

	bool m_bTutorialEndFlag;			//終了フラグ
	static bool m_bGuestFlag;			// ゲストの終了フラグ

	S_ptr<CScene2D> m_pReady;
	S_ptr<CScene2D> m_pTutorialPolygon;
	S_ptr<CScene2D> m_pNextPage;

};

#endif