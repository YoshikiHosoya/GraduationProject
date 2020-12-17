//------------------------------------------------------------------------------
//
//UIのベース  [UI_Base.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _UI_Base_H_
#define _UI_Base_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CUI_Base : public CScene2D
{
public:

	enum APPEAR_PATTERN
	{
		NORMAL,						//通常
		RIGHT_TO_CENTER,			//右からセンターに出現
		TRANSPARENT_TO_APPEAR,		//透明から徐々に見えるようになる
	};

	CUI_Base();
	~CUI_Base();
	virtual HRESULT Init()						override;				//初期化
	virtual void Update()						override;				//更新
	virtual void Draw()							override;				//描画
	virtual void ShowDebugInfo()				override;				//デバッグ情報表記

	static S_ptr<CUI_Base> Create(D3DXVECTOR3 const & pos, D3DXVECTOR3 const & size, D3DXCOLOR const & col, LPDIRECT3DTEXTURE9 const tex, CScene::OBJTYPE const objtype, APPEAR_PATTERN pattern, int nTotalTime);

	void SetAppearPattern(APPEAR_PATTERN pattern, int nTotalTime);		//パターン設定

private:
	APPEAR_PATTERN m_pattern;											//出現パターン
	int m_nCnt;															//カウンター
	int m_nTotalTime;													//最大時間
	float m_fChangeValue;												//変化するパターン
};

#endif