//------------------------------------------------------------------------------
//
//タイマー表示処理  [timer.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _TIMER_H_
#define _TIMER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------

//前方宣言
class CMultiNumber;

class CTimer
{
public:
	enum TIMER_TYPE
	{
		MINUTE = 0,			//分
		SECOND,				//秒
		COMMA,				//，
		MAX,
	};
	CTimer();
	~CTimer();
	void UpdateTimer();															//更新
	void ChangeNumber();														//ディスプレイの数値更新
	void ShowDebugInfo();														//デバッグ情報表記
	static std::unique_ptr<CTimer> Create(D3DXVECTOR3 const &TimerCenterPos,int const nTimer,D3DXMATRIX *pMtx);			//生成処理

	static int GetTimer() { return m_nTimer; };									//タイマー取得
	void SetStop(bool bStop) { m_bStop = bStop; };								//タイマーストップ
private:
	std::vector<std::shared_ptr<CMultiNumber>> m_pMultiNumber;					//ナンバーのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture;										//テクスチャのポインタ

	static int m_nTimer;														//タイマー 秒で管理
	int m_nCntFlame;															//カウント フレーム
	bool m_bStop;																//ストップ
};

#endif