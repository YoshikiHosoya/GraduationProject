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

	void CreateUI();
	void UpdateState();
	void Collision();

private:
	int m_nCntState;	//カウンタ
	TUTORIL_TYPE m_state;

	Vec<S_ptr<CScene2D>> m_pPolygonList;
};

#endif