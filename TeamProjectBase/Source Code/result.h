//------------------------------------------------------------------------------
//
//リザルト処理  [result.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_
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

class CResult : public CBaseMode
{
public:
	CResult();
	~CResult();

	enum RESULT_STATE
	{
		UP_BAR_IN,
		RESULT_DISP,
		PLAYTIME_DISP,
		STAY,
		SELECT,
		FADE,
	};


	enum RESULT_POLYGON_TYPE
	{
		LOGO = 0,
		UP_BAR,
		JUDGE,

		PLAYTIME,
		SEMICOLON,
		COMMMA,
		UNDER_BAR,

		SELECTION_WINDOW,
		RETRY,
		END,
		MAX,
	};



	HRESULT Init(HWND hWnd);			//初期化
	void Update();						//更新
	void Draw();						//描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	void UpdateState();
	void Collision();
	void SetState(RESULT_STATE state);

private:
	int m_nCntState;	//カウンタ

	S_ptr<CTimer> m_pTimer;
	RESULT_STATE m_state;

	Vec<S_ptr<CUI_Base>> m_pPolygonList;
};

#endif