//------------------------------------------------------------------------------
//
//ボムの外装処理  [bomb_exterior.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _BOMB_EXTERIOR_H_
#define _BOMB_EXTERIOR_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CScene3D;
class CSceneX;

class CBomb_Exterior
{
	//外装のタイプ
	enum EXTERIOR_TYPE
	{
		NONE = 0,		//何もなし
		BATTERY,		//バッテリー
		MAX,			//
	};


public:
	CBomb_Exterior();
	virtual ~CBomb_Exterior();
	static U_ptr<CBomb_Exterior> CreateBombExterior(D3DXMATRIX *pBombMtx);

	Vec<S_ptr<CSceneX>> GetBatteryPtrList() { return m_pBatteryList; };
	int GetBatteryNum() { return m_nBatteryNum; };

private:
	Vec<S_ptr<CSceneX>> m_pBatteryList;		//バッテリーのリスト
	Vec<EXTERIOR_TYPE> m_ExteriorList;		//外装に何をつけるかのリスト
	int m_nBatteryNum;						//バッテリー数

	void SetExterior();						//外装に何を何個つけるか決める
	void Create(D3DXMATRIX *pBombMtx);		//生成関数

	void CalcExteriorPosition(int const nCnt, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);

};
#endif