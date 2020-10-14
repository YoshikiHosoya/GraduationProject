//------------------------------------------------------------------------------
//
//基底モード処理  [basemode.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

#ifndef _MODE_H_
#define _MODE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CBaseMode
{
public:
	CBaseMode() {};
	virtual ~CBaseMode() {};

	virtual HRESULT Init(HWND hWnd) = 0;				//初期化
	virtual void Uninit() = 0;							//終了
	virtual void Update() = 0;							//更新
	virtual void Draw() = 0;							//描画
	virtual void ShowDebugInfo() = 0;					//デバッグ情報表記
	void DebugCommand();								//デバッグコマンド
	static void BaseLoad(HWND hWnd);					//全ての場面で使われるであろう素材の生成
	static void BaseUnload();							//全ての場面で使われるであろう素材の破棄
private:
};

#endif